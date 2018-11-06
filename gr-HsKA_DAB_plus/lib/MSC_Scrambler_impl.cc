/* -*- c++ -*- */
/* 
 * Copyright 2018 HsKA.
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
#include "MSC_Scrambler_impl.h"

namespace gr
{
	namespace HsKA_DAB_plus 
	{
		MSC_Scrambler::sptr MSC_Scrambler::make()
		{
			// Neue Instanz erzeugen und GnuRadio zur Verfuegung stellen
			return gnuradio::get_initial_sptr(new MSC_Scrambler_impl());
		}

		/*
		* The private constructor
		*/
		MSC_Scrambler_impl::MSC_Scrambler_impl()
			: gr::block("MSC_Scrambler", 
						gr::io_signature::make3(3, 3, sizeof(uint8_t), sizeof(uint8_t), sizeof(int32_t)),
						gr::io_signature::make(2, 2, sizeof(uint8_t)))
		{
			m_latch = 0xFFFFFFFF;
			m_remaining_count = 0;
			m_data = 0;
			m_sync = false;
			m_bit_counter = 0;
				
			// Bit in Bytes
			set_relative_rate(1.0f/8.0f);
		}

		/*
		* Our virtual destructor.
		*/
		MSC_Scrambler_impl::~MSC_Scrambler_impl()
		{
		}

		void MSC_Scrambler_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = 8 * noutput_items;
			}  
		}

		int MSC_Scrambler_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			const uint8_t *in		= (const uint8_t *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			const int32_t *k		= (const int32_t *) input_items[2];
			uint8_t *out 			= (uint8_t *) output_items[0];	
			uint8_t *sync_out		= (uint8_t *) output_items[1];	
			
			uint32_t output_count = 0;
			
			for(int32_t i = 0; i < noutput_items; ++i)
			{
				if(sync_in[i] != 0)
				{
					m_latch = 0xFFFFFFFF;
					
					m_remaining_count = (k[i] - 6) / 8;
					m_bit_counter = 0;
					
					m_sync = true;
				}
				
				if(m_remaining_count > 0)
				{
					// Pseudorandombit generieren
					uint8_t prbs = (m_latch >> 8) ^ (m_latch >> 4);
					m_latch <<= 1;
					m_latch |= (prbs & 1);

					// Ausgangsdaten erzeugen
					m_data <<= 1;
					m_data |= (prbs ^ in[i]) & 0x01;
					
					++m_bit_counter;

					if(m_bit_counter >= 8)
					{
						m_bit_counter = 0;
						out[output_count] = m_data;
						
						if(m_sync)
							sync_out[output_count] = 1;
						else
							sync_out[output_count] = 0;
						
						m_sync = false;
				
						++output_count;
						--m_remaining_count;
					}
				}
			}

			consume_each(noutput_items);
			return output_count;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

