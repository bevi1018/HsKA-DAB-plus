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
- Block: DQPSK_Demodulation
- Beschreibung: DQPSK Demodulation findet durch die Multiplikation des komplex konjugierten Wertes mit einem OFDM Symbol Verzögerung statt. D.h. DQPSK über die zeitliche
Richtung, also Symbole nacheinander auf dem Unterträger und nicht in Frequenzrichtung. Ändert sich der Symbolanfang theta, so verschiebt sich die Phasebeziehung. Für die
korrekte Relation muss ein Symbol korrigiert werden, da ansonsten die Phase bei den äußeren Unterträgern den Eindeutigkeitsbereich überschreiten kann.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "DQPSK_Demodulation_impl.h"

#include <gnuradio/expj.h>

#include <boost/math/constants/constants.hpp>

#define PI boost::math::constants::pi<float>()

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		DQPSK_Demodulation::sptr DQPSK_Demodulation::make(int vector_length, int fft_length)
		{
			// Instanz des Blocks erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new DQPSK_Demodulation_impl(vector_length, fft_length));
		}

		DQPSK_Demodulation_impl::DQPSK_Demodulation_impl(int vector_length, int fft_length)
			: gr::block("DQPSK_Demodulation", 
						gr::io_signature::make3(3, 3, vector_length * sizeof(gr_complex), sizeof(short), sizeof(uint8_t)),
						gr::io_signature::make2(2, 2, vector_length * sizeof(gr_complex), sizeof(uint8_t))),
			  m_vector_length(vector_length),
			  m_last_theta(0),
			  m_fft_length(fft_length)
		{			
			// Array erzeugen, welches das vorherige OFDM-Symbol beinhaltet
			m_last_sample = new gr_complex[vector_length];
			for(int i = 0; i < vector_length; ++i)
			{
				// Alle Samples mit 1 initialisieren.
				m_last_sample[i] = gr_complex(1, 0);
			}
		}

		DQPSK_Demodulation_impl::~DQPSK_Demodulation_impl()
		{
			// Array für das vorherige OFDM-Symbol löschen
			if(m_last_sample)
			{
				delete [] m_last_sample;
				m_last_sample = 0;
			}
		}

		void DQPSK_Demodulation_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden.
			for(int i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int DQPSK_Demodulation_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Ausgangsdaten zur einfacheren Handhabung casten
			const gr_complex *in = (const gr_complex *) input_items[0];
			const short *theta_in = (const short *) input_items[1];
			const uint8_t *sync_in = (const uint8_t *) input_items[2];
			gr_complex *out = (gr_complex *) output_items[0];
			uint8_t *sync_out = (uint8_t *) output_items[1];

			for(int i = 0; i < m_vector_length; ++i)
			{
				// Demodulation durch Multiplikation des komplex konjugierten letzten Symbols auf demselben Unterträger 
				// und Phasenkorrektur pro Unterträger, falls der Symbolanfang sich geändert hat
				//if(i >= (m_vector_length / 2))
				//	out[i] = in[i] * conj(m_last_sample[i]) * gr_expj(-2.0 * PI * (i + 1 - m_vector_length / 2) * (float)(theta_in[0] - m_last_theta) / (float)(m_fft_length));	
				//else
					out[i] = in[i] * conj(m_last_sample[i]) * gr_expj(-2.0 * PI * (i - m_vector_length / 2) * (float)(theta_in[0] - m_last_theta) / (float)(m_fft_length));
				m_last_sample[i] = in[i];
			}

			m_last_theta = theta_in[0];
			sync_out[0] = sync_in[0];

			consume_each(1);
			return 1;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

