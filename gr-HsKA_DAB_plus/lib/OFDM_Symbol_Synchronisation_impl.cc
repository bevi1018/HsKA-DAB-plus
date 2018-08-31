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

#include "Header_File.h"

#include <gnuradio/io_signature.h>
#include "OFDM_Symbol_Synchronisation_impl.h"

namespace gr {
  namespace HsKA_DAB_plus {

	OFDM_Symbol_Synchronisation::sptr
	OFDM_Symbol_Synchronisation::make(int N, int L, float SNR, int Averaging)
	{
	return gnuradio::get_initial_sptr(new OFDM_Symbol_Synchronisation_impl(N, L, SNR, Averaging));
	}

	OFDM_Symbol_Synchronisation_impl::OFDM_Symbol_Synchronisation_impl(int N, int L, float SNR, int Averaging)
	: gr::block("OFDM_Symbol_Synchronisation", gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(byte)), gr::io_signature::make3(3, 3, N*sizeof(gr_complex), sizeof(short), sizeof(byte))),
	  Epsilon_moving_sum(L), gamma_moving_sum(L), estimated_epsilon_moving_average(Averaging), estimated_theta_moving_average(76)
	{
		this->N = N; // OFDM Symboldauer ohne Guard Interval in samples
		this->L = L; // Guard Interval in samples
		rho = pow(10,SNR/20.0f); // Umrechung logarithmisches SNR in linear
		rho = rho/(rho + 1.0f); // Berechnung von rho, siehe [2]

		// Zwischenvariablen, siehe [2]
		Epsilon = 0.0;
		gamma = gr_complex(0,0);
		Lambda = 0.0;
		
		find_maximum = FLOAT_NEG_MAX;

		// estimation variables (aquisition mode)
		theta_maximum_likelihood = 0; // Schätzwert für Symbolanfang in samples
		epsilon_maximum_likelihood = 0.0f; // Schätzwert für den Phasenoffset (fein)
		gamma_at_time_instant_theta = gr_complex(0,0);

		phi = 0.0; // Phasenkorrekturvariable für die verworfenen samples

		symbol_counter = 0;
	}

	OFDM_Symbol_Synchronisation_impl::~OFDM_Symbol_Synchronisation_impl()
	{
	}

	void
	OFDM_Symbol_Synchronisation_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
	{
		for(int i=0; i < ninput_items_required.size(); ++i)
		{
			ninput_items_required[i] = (2*N+L)*noutput_items;
		}
	}

	int
	OFDM_Symbol_Synchronisation_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
	{
	const gr_complex *in = (const gr_complex *) input_items[0];
	const byte *sync_in = (const byte *) input_items[1];
	gr_complex *out = (gr_complex *) output_items[0];
	short *theta_out = (short *) output_items[1];
	byte *sync_out = (byte *) output_items[2];

	if(ninput_items[0] < 2*N+L) // keine Verarbeitung, wenn zuwenig Eingangsdaten zur Verfügung stehen
	{
		return 0;
	}
	else if(noutput_items < 1) // keine Verarbeitung, wenn nicht genug Speicherplatz für den Ausgabevektor zur Verfügung steht
	{
		return 0;
	}
	else
	{
		// Important Initializations
		gamma_at_time_instant_theta = gr_complex(0,0);
		find_maximum = FLOAT_NEG_MAX;
		theta_maximum_likelihood = 0;

		for(int i=0; i<N+L; i++) // Suche den OFDM Symbolanfang in N+L samples
		{	
			if(sync_in[i] != 0) // Falls mit dem Super Frame gestartet wird, verwerfe alle vorherigen Eingangsdaten
			{
				symbol_counter = 0;
				consume(0,i+1);
				consume(1,i+1);
				//printf("Super Frame Start!\n");
				phi += epsilon_maximum_likelihood*(float)(i+1)/(float)(N); // Anpassung der Phase, da samples verworfen wurden und die Phase sich dabei weiter gedreht hat.
				return WORK_CALLED_PRODUCE;
			}
			else if(symbol_counter > 76) // Nach 76 Symbolen verlassen der Verarbeitung ohne erneute Schätzung des Symbolanfangs und der Phase
			{
				continue;
			}
				
			
			Epsilon = Epsilon_moving_sum.filter(abs(in[i]*conj(in[i]) + in[i+N]*conj(in[i+N]))); // sum over L samples; |r(k)|²+|r(k+N)|²
			gamma = gamma_moving_sum.filter(in[i]*conj(in[i+N])); // sum over L samples; r(k)*r*(k+N)
			Lambda = 2*abs(gamma)*cos(-1*epsilon_maximum_likelihood + arg(gamma)) - rho*Epsilon;

			if(Lambda > find_maximum) // Suche nach dem Maximum der Funktion
			{
				find_maximum = Lambda;
				theta_maximum_likelihood = i; // estimated time instant (theta) of OFDM Symbol Start, cyclic prefix already taken into account
				gamma_at_time_instant_theta = gamma;
			}
		}

		if(symbol_counter < 76)
		{
			epsilon_maximum_likelihood = estimated_epsilon_moving_average.filter(arg(gamma_at_time_instant_theta))/estimated_epsilon_moving_average.getLength();
			//theta_maximum_likelihood = (int)(estimated_theta_moving_average.filter(theta_maximum_likelihood)/76 + 0.5f); // Filterung des Schätzwertes vom Symbolbeginn unnötig
			//printf("epsilon: %f, Theta: %d\n", epsilon_maximum_likelihood, theta_maximum_likelihood);

			for(int j=0; j<=N-1; j++)
			{
				float argument = epsilon_maximum_likelihood*(float)(j+theta_maximum_likelihood)/(float)N + phi; // Phasenkorrekturterm: Phase pro Unterträger (fein) + Phase über Guard Interval und Null Symbol
				out[j] = in[j+theta_maximum_likelihood]*gr_expj(argument); // Ausgabe und Korrektur vom OFDM Symbol, nur N samples ohne Guard Interval
			}

			theta_out[0] = theta_maximum_likelihood; // Ausgabe Symbolanfangsschätzung in samples

			produce(0,1);
			produce(1,1);
			produce(2,1);	
		}

		phi += epsilon_maximum_likelihood*(float)(N+L)/(float)N; // Anpassung der Phase, da samples verworfen wurden und die Phase sich dabei weiter gedreht hat.

		// Eindeutigkeitsbereich von 2*PI definieren
		while(phi >= PI){
			phi -= 2*PI;
		}
		while(phi < -PI){
			phi += 2*PI;
		}

		// Synchronisationsignal erstellen
		if(symbol_counter == 0)
		{
			sync_out[0] = 1;
		}
		else
		{
			sync_out[0] = 0;
		}

		symbol_counter++;

	consume(0,N+L);
	consume(1,N+L);
	return WORK_CALLED_PRODUCE;
	}
	}

  } /* namespace HsKA_DAB_plus */
} /* namespace gr */

