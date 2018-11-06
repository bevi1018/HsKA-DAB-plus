/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "OFDM_coarse_frequency_correction_impl.h"

#include <gnuradio/expj.h>

#include <boost/math/constants/constants.hpp>

#define PI boost::math::constants::pi<float>()

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		OFDM_coarse_frequency_correction::sptr OFDM_coarse_frequency_correction::make(int32_t fft_length, int32_t num_carriers, int32_t cp_length, int32_t debug_enable)
		{
			// Instanz des Blocks erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new OFDM_coarse_frequency_correction_impl(fft_length, num_carriers, cp_length, debug_enable));
		}

		OFDM_coarse_frequency_correction_impl::OFDM_coarse_frequency_correction_impl(int32_t fft_length, int32_t num_carriers, int32_t cp_length, int32_t debug_enable)
			: gr::block("OFDM_coarse_frequency_correction", 
						gr::io_signature::make2(2, 2, fft_length * sizeof(gr_complex), sizeof(uint8_t)),
						gr::io_signature::make2(2, 2, num_carriers * sizeof(gr_complex), sizeof(uint8_t))),
			  m_fft_length(fft_length), 
			  m_num_carriers(num_carriers),
			  m_cp_length(cp_length),
			  m_symbol_num(0),
			  m_freq_offset(0),
			  m_delta_f(0),
			  m_debug_enable(debug_enable)
		{
		}

		OFDM_coarse_frequency_correction_impl::~OFDM_coarse_frequency_correction_impl()
		{
		}

		void OFDM_coarse_frequency_correction_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Werte ausgegeben, wie eingelesen wurde.
			for(int i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		void OFDM_coarse_frequency_correction_impl::correlate_energy(const gr_complex *symbol)
		{
			double sum = 0;
			
			// Bei der gleitenden Summe wird der DC-Anteil nicht mitgenommen, um eine genauere Schätzung des Offsets zu erhalten.
			
			// Zuerst werden die ersten m_num_carriers Unterträger aufsummiert.
			for(uint32_t i = 0; i < m_num_carriers+1; i++)
			{
				if (i != (m_num_carriers / 2))
					sum += norm(symbol[i]);
			}
			
			double max = sum;
			uint32_t index = 0;
			
			// Danach wird der Bereich zyklisch um ein Unterträger verschoben.
			for(uint32_t i = 1; i < (m_fft_length - m_num_carriers); i++)
			{
				// Es muss nur die Differenz zur vorherigen Summe aufaddiert werden
				// Linker und rechter Rand
				sum += norm(symbol[i + m_num_carriers]) - norm(symbol[i - 1]);
				// DC-Anteil
				sum += norm(symbol[i + m_num_carriers / 2 - 1]) - norm(symbol[i + m_num_carriers / 2]);
				
				// Ist die aktuelle Summe größer als die vorherige Summe?
				if (sum > max) 
				{
					// Wenn ja, dann Summe und Index speichern.
					max = sum;
					index = i;
				}
			}
			
			// Den Frequenzoffset als Unterträgeroffset speichern
			m_freq_offset = index;
			
			// Die Anzahl der Nullträger auf einer Seite abziehen -> m_delta_f = 0, wenn kein Frequenzfehler vorhanden
			m_delta_f = m_freq_offset - (m_fft_length - m_num_carriers)/2;
		}

		int32_t OFDM_coarse_frequency_correction_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Ausgangsdaten zur einfacheren Handhabung casten
			const gr_complex *in	= (const gr_complex *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			gr_complex *out			= (gr_complex *) output_items[0];
			uint8_t *sync_out		= (uint8_t *) output_items[1];

			// Ist das aktuelle Symbol, das erste Symbol im Frame?
			if (sync_in[0] != 0)
			{
				// Sync des Ausgangs auf 1 setzen
				sync_out[0] = 1;
				
				// Schätzung des Frequenzoffsets
				correlate_energy(in);
				
				// Debugausgabe
				if(m_debug_enable != 0)
					printf("Geschätzter Frequenzoffset: %d, %d\n", m_freq_offset, m_delta_f);
				
				// Symbolzähler auf 0 setzen
				m_symbol_num = 0;
			}
			else
			{
				// Sync des Ausgangs auf 0 setzen
				sync_out[0] = 0;
				
				// Symbolzähler um 1 erhöhen
				m_symbol_num++;
			}

			// correct phase offset from removing cp
			// could be done after diff phasor, then it would be the same offset for each symbol; but its hardly much of an overhead
			
			///!!!! Muss das wirklich gemacht werden?
			gr_complex phase_offset_correct = gr_expj(-2.0f * PI * (float)m_delta_f * (float)m_cp_length / (float)m_fft_length * (float)m_symbol_num);

			// Die Bins ohne die Nullträger in den Ausgangsbuffer kopieren.
			for (int32_t i = 0; i < m_num_carriers/2; i++)
			{
				out[i] = in[m_freq_offset+i] * phase_offset_correct;
			}

			for (int32_t i = m_num_carriers/2; i < m_num_carriers; i++)
			{
				out[i] = in[m_freq_offset+i+1] * phase_offset_correct;
			}

			consume_each(1);
			return 1;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

