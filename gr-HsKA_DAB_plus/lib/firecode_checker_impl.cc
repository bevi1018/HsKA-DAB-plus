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
#include "firecode_checker_impl.h"

//	g(x)=(x^11+1)(x^5+x^3+x^2+x+1)=x^16+x^14+x^13+x^12+x^11+x^5+x^3+x^2+x+1
#define POLYNOM 0x782F

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		firecode_checker::sptr firecode_checker::make(int32_t debug_enable)
		{
			// Neue Instanz erzeugen und an GnuRadio weitergeben
			return gnuradio::get_initial_sptr(new firecode_checker_impl(debug_enable));
		}

		/*
		* The private constructor
		*/
		firecode_checker_impl::firecode_checker_impl(int32_t debug_enable)
			: gr::block("firecode_checker",
						gr::io_signature::make2(2, 2, sizeof(uint8_t), sizeof(uint8_t)),
						gr::io_signature::make2(2, 2, sizeof(uint8_t), sizeof(uint8_t))),
			  m_debug_enable(debug_enable),
			  m_byte_counter(0),
			  m_check_passed(false),
			  m_remaining_audio_frames(0)
		{
			// Die CRC Tabelle für den Firecode erzeugen		  
		  	for(int32_t i = 0; i < 256; ++i)
			{
				uint16_t crc = i << 8;
				
				for(int32_t bit = 0; bit < 8; ++bit)
				{
					if(crc & 0x8000)
						crc = (crc << 1) ^ POLYNOM;
					else
						crc = (crc << 1);
				}
				
				m_crc_table[i] = crc;
			}
		}

		/*
		* Our virtual destructor.
		*/
		firecode_checker_impl::~firecode_checker_impl()
		{
		}

		void firecode_checker_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int32_t firecode_checker_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Ausgangsdaten zur einfacheren Handhabung casten
			const uint8_t *in		= (const uint8_t *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			uint8_t *out			= (uint8_t *) output_items[0];
			uint8_t *sync_out		= (uint8_t *) output_items[1];
			
			// Gibt an, wieviele bytes ausgegeben werden
			int32_t output_counter = 0;
			
			// Alle Eingangsdaten durchiterieren
			for(int32_t i = 0; i < noutput_items; ++i)
			{
				// Wenn Sync empfangen wurde
				if(sync_in[i] != 0)
				{
					// Debugausgabe
					if(m_debug_enable >= 2)
						printf("Firecode: Sync received\n");
								   
					// Wenn der letze Firecode-Check erfolgreich war
					if(m_check_passed)
					{
						// Müssen noch Audioframes des aktuellen Audio-Superframes ausgegeben werden?
						if(m_remaining_audio_frames > 0)
							--m_remaining_audio_frames;
						else
						{
							// Bei Sync alle Zähler und Flags zurücksetzen
							m_byte_counter = 0;
							m_firecode = 0;
							m_check_passed = false;
						}
					}
					else
					{	
						// Bei Sync alle Zähler und Flags zurücksetzen
						m_byte_counter = 0;
						m_firecode = 0;
						m_check_passed = false;
					}
			    }
					  
				// Der Firecode muss nur von den ersten Bytes berechnet werden
			    if(m_byte_counter <= 11)
				{
					if(m_byte_counter < 2)
						// In den ersten 2 Bytes befindet sich der gesendete Firecode.
						m_received_firecode = (m_received_firecode << 8) | in[i];
					else if(m_byte_counter <= 10)
						// Aus den Bytes 2 bis 10 muss der Firecode berechnet werden
						m_firecode = (m_firecode << 8) ^ m_crc_table[((m_firecode >> 8) ^ in[i]) & 0x00FF];
					
					// Byte des Headers speichern
					m_firecode_header[m_byte_counter] = in[i];
					
					if(m_byte_counter == 11)
					{	
						if(m_debug_enable >= 2)
							printf("Firecode: Received Firecode: 0x%04X, Calculated Firecode: 0x%04X\n", m_received_firecode, m_firecode);
						
						// Ist der empfangene Firecode gleich dem berechneten Firecode?
						if(m_firecode == m_received_firecode)
						{
							// Falls der Firecode-Header nicht mehr in den Ausgangspuffer passt -> general_work() nochmal aufrufen lassen.
							if((noutput_items - i) <= 11)
							{
								consume_each(i);
								return output_counter;
							}
							
							if(m_debug_enable >= 1)
								printf("Firecode: Firecode Check passed\n");

							// Firecode-Header in den Ausgangspuffer schreiben
							memcpy(&out[output_counter], m_firecode_header, 12 * sizeof(uint8_t));
							memset(&sync_out[output_counter], 0, 12 * sizeof(uint8_t));
							sync_out[output_counter] = 1;
							output_counter += 12;
							
							m_remaining_audio_frames = 4;
							m_check_passed = true;
						}
						else
						{
							if(m_debug_enable >= 1)
								printf("Firecode: Firecode Check failed\n");
						}
					}
				}
				else
				{
					// Wenn der letzte Firecode-Check erfolgreich war, Daten ausgeben
					if(m_check_passed)
					{
						out[output_counter] = in[i];
						sync_out[output_counter] = 0;
						++output_counter;
					}
				}
				
				// Bytezähler hochzählen
				++m_byte_counter;
			}

			// Gnuradio mitteilen, wiviele Daten eingelesen und ausgegeben wurden
			consume_each (noutput_items);
			return output_counter;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

