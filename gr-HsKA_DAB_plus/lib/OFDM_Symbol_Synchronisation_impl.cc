/* -*- c++ -*- */
/* 
 * Copyright 2018 <HsKA>.
 * 
 * This is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 * 
 * This software is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this software; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

/*
Hochschule Karlsruhe Technik u. Wirtschaft (HsKa)
Projektarbeit Master
- Projekt: DAB+ Empfänger mit Gnu Radio
- Studenten: David Kohler, Vivian Becher
- Professor: M. Litzenburger
- Datum: 30.08.18
--------------------------------------------------
Informationen
- Block: OFDM_Symbol_Synchronisation
- Beschreibung: Schätzung des Symbolanfangs über das Guard Interval mit der Auto-Korrelationsfunktion bei einem Zeitabstand von N. Suchen nach dem Maximum dieser Funktion.
Feine Phasenkorrektur auf den einzelnen Unterträgern und Phasenkorrektur aufgrund der Verwerfung von samples. Vorgehen ist in Quelle [2] beschrieben - die Parameter
für dieses System sind im Standard zu finden.
- Quellen: [1] ETSI Standard EN 300 401; [2] 'On Synchronisation in OFDM Systems using the Cyclic Prefix' by J. van de Beek, M. Sandell and P. Börjesson
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "OFDM_Symbol_Synchronisation_impl.h"

#include <gnuradio/expj.h>
#include <limits>
#include <boost/math/constants/constants.hpp>

#define PI boost::math::constants::pi<float>()
#define FLOAT_NEG_MAX -std::numeric_limits<float>::max()

#define SYMBOL_COUNT 76

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		OFDM_Symbol_Synchronisation::sptr OFDM_Symbol_Synchronisation::make(int32_t N, int32_t L, float SNR, int32_t average_count, int32_t debug_enable)
		{
			return gnuradio::get_initial_sptr(new OFDM_Symbol_Synchronisation_impl(N, L, SNR, average_count, debug_enable));
		}

		OFDM_Symbol_Synchronisation_impl::OFDM_Symbol_Synchronisation_impl(int32_t N, int32_t L, float SNR, int32_t average_count, int32_t debug_enable)
			: gr::block("OFDM_Symbol_Synchronisation",
						gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(uint8_t)), 
						gr::io_signature::make3(3, 3, N * sizeof(gr_complex), sizeof(int16_t), sizeof(uint8_t))),
			  epsilon_moving_sum(L), 
			  gamma_moving_sum(L),
			  estimated_epsilon_moving_average(average_count)
		{
			this->N = N; // OFDM Symboldauer ohne Guard Interval in samples
			this->L = L; // Guard Interval in samples
				  
			// SNR von logarithmisch in linear umrechnen und gemäß [2] rho berechnen
			rho = pow(10, SNR / 20.0f);
			rho = rho/(rho + 1.0f);

			// Schätzwert für den Phasenoffset (fein) initialisieren
			epsilon_maximum_likelihood = 0.0f; 

			// Aktueller Phasenwinkel für die Phasenkorrektur initialisieren
			phi = 0.0;

			// Symbolzähler initialisieren
			symbol_counter = 0;
				  
			// Debugswitch setzen
			this->debug_enable = debug_enable;
		}

		OFDM_Symbol_Synchronisation_impl::~OFDM_Symbol_Synchronisation_impl()
		{
		}

		void OFDM_Symbol_Synchronisation_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
		{
			for(int i = 0; i < ninput_items_required.size(); ++i)
			{
				// Da wir ein Vektor ausgeben, brauchen wir 2*N+L Werte am Eingang
				ninput_items_required[i] = (2 * N + L) * noutput_items;
			}
		}

		int OFDM_Symbol_Synchronisation_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Eingangs- und Ausgangsdaten zur einfacheren Handhabung casten
			const gr_complex *in   = (const gr_complex *) input_items[0];
			const uint8_t *sync_in = (const uint8_t *) input_items[1];
			gr_complex *out		   = (gr_complex *) output_items[0];
			int16_t *theta_out	   = (int16_t *) output_items[1];
			uint8_t *sync_out	   = (uint8_t *) output_items[2];

			// Important Initializations
			gr_complex gamma_at_theta_max_likelihood = gr_complex(0,0);
			float lambda_maximum = FLOAT_NEG_MAX;
			int32_t theta_maximum_likelihood = 0;

			 // Suche den OFDM Symbolanfang in N+L samples
			for(int32_t i = 0; i < (N + L); ++i)
			{	
				// Falls mit dem Super Frame gestartet wird, verwerfe alle vorherigen Eingangsdaten
				if(sync_in[i] != 0)
				{
					// Symbolzähler zurücksetzen
					symbol_counter = 0;
					
					// Alle bisher verarbeiteten Werte aus dem Inputbuffer löschen
					consume(0, i + 1);
					consume(1, i + 1);
					
					// Anpassung der Phase, da Samples verworfen wurden und die Phase sich dabei weiter gedreht hat.
					phi += epsilon_maximum_likelihood * (float)(i + 1) / (float)N;
					return 0;
				}
				else if(symbol_counter > SYMBOL_COUNT) // Nach 76 Symbolen verlassen der Verarbeitung ohne erneute Schätzung des Symbolanfangs und der Phase
					continue;


				// Epsilon ist die Summe der letzen L Werten aus |r(k)|²+|r(k+N)|² Quelle: [2]
				float epsilon = epsilon_moving_sum.filter(abs(in[i] * conj(in[i]) + in[i + N] * conj(in[i + N])));
				
				// Gamma ist die Summe der letzen L Werten aus |r(k)|²+|r(k+N)|² Quelle: [2]
				gr_complex gamma = gamma_moving_sum.filter(in[i] * conj(in[i + N]));
				
				// Gamma gemäß Quelle [2] berechnen
				float lambda = 2 * abs(gamma) * cos(arg(gamma) - epsilon_maximum_likelihood) - rho * epsilon;

				// Suche nach dem Maximum von Lambda
				if(lambda > lambda_maximum)
				{
					// Neues Maximum speichern
					lambda_maximum = lambda;
					theta_maximum_likelihood = i;
					gamma_at_theta_max_likelihood = gamma;
				}
			}

			// Ist der Symbolzähler in einem gültigen Bereich?
			if(symbol_counter < SYMBOL_COUNT)
			{
				// Epsilon ist der Mittelwert des Arguments von Gamma
				epsilon_maximum_likelihood = estimated_epsilon_moving_average.filter(arg(gamma_at_theta_max_likelihood)) / estimated_epsilon_moving_average.getLength();
				
				// Wenn Debugausgabe erwünscht, Debugwerte ausgeben
				if(debug_enable != 0)
					printf("Symbol Synchronisation: epsilon: %f, Theta: %d\n", epsilon_maximum_likelihood, theta_maximum_likelihood);

				// Phasen- und Zeitoffsetkorrektur vornehmen und Daten an den Ausgangsport weitergeben
				for(int j=0; j <= N - 1; j++)
				{
					// Phasenkorrekturterm: Phase pro Unterträger (fein) + Phase über Guard Interval und Null Symbol
					float argument = epsilon_maximum_likelihood * (float)(j + theta_maximum_likelihood)/(float)N + phi;
					out[j] = in[j + theta_maximum_likelihood] * gr_expj(argument); // Ausgabe und Korrektur vom OFDM Symbol, nur N samples ohne Guard Interval
				}

				theta_out[0] = theta_maximum_likelihood; // Ausgabe Symbolanfangsschätzung in Samples

				// GnuRadio mitteilen, dass jeweils ein Wert ausgegebne wurde
				produce(0,1);
				produce(1,1);
				produce(2,1);	
			}
			else
			{
				if(debug_enable != 0)
					printf("Symbol Synchronisation: Missing Sync!\n");
			}

			// Anpassung der Phase
			phi += epsilon_maximum_likelihood*(float)(N+L)/(float)N;

			// Eindeutigkeitsbereich von +-PI definieren
			while(phi >= PI)
				phi -= 2*PI;
			while(phi < -PI)
				phi += 2*PI;
			
			// Synchronisationsignal markiert das erste Symbol im Frame
			if(symbol_counter == 0)
				sync_out[0] = 1;
			else
				sync_out[0] = 0;

			// Symbolzähler inkrementieren
			symbol_counter++;

			// Es wurden jeweils N+L Werte am Eingang verarbeitet
			consume(0,N+L);
			consume(1,N+L);
			
			// Es wurde produce() aufgerufen
			return WORK_CALLED_PRODUCE;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

