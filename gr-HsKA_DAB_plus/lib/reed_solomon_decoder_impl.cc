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
#include "reed_solomon_decoder_impl.h"

extern "C"
{
	#include "fec/fec.h"
}

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		reed_solomon_decoder::sptr reed_solomon_decoder::make(int32_t debug_enable)
		{
			// Neue Instanz erzeugen und an GnuRadio weitergeben
			return gnuradio::get_initial_sptr(new reed_solomon_decoder_impl(debug_enable));
		}

		/*
		* The private constructor
		*/
		reed_solomon_decoder_impl::reed_solomon_decoder_impl(int32_t debug_enable)
			: gr::block("reed_solomon_decoder",
						gr::io_signature::make(2, 2, sizeof(uint8_t)),
						gr::io_signature::make(2, 2, sizeof(uint8_t))),
			  m_debug_enable(debug_enable),
			  m_block_count(1)
		{
			// Reed solomon API initialisieren (135 Paddingbytes, da verkürzter Code)
			m_rs_handle = init_rs_char(8, 0x11D, 0, 1, 10, 135);
				  
			// Der Reed Solomon Dekodierer wird immer auf 120 Bytes angewendet. Verkürzter Code: C(120, 110, t=5)
			set_output_multiple(110 * sizeof(uint8_t));
		}

		/*
		* Our virtual destructor.
		*/
		reed_solomon_decoder_impl::~reed_solomon_decoder_impl()
		{
			// Reed solomon Handle freigeben.
			free_rs_char(m_rs_handle);
		}

		void reed_solomon_decoder_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Pro 120 Eingangsbytes werden 110 Bytes ausgegeben
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items*120/110;
			}
		}

		int32_t reed_solomon_decoder_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Ausgangsdaten zur einfacheren Handhabung 
			const uint8_t *in		= (const uint8_t *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			uint8_t *out			= (uint8_t *) output_items[0];
			uint8_t *sync_out		= (uint8_t *) output_items[1];
			
			// Re-sync, damit in 120 Byte Blöcken gearbeitet werden kann
			if(sync_in[0] == 0)
			{
				uint32_t discarded_count = 1;
				
				while(discarded_count < ninput_items[0])
				{
					if(sync_in[discarded_count] != 0)
						break;
					
					++discarded_count;
				}
				
				if(m_debug_enable != 0)
					printf("Reed Solomon Decoder: Resyncing! Discarded: %d\n", discarded_count);
				
				// Es wird immer mindestens ein RS-Block verarbeitet
				m_block_count = 1;
				
				consume_each(discarded_count);
				return 0;
			}
			
			// Den nächsten Sync suchen, damit die Audio-Superframe Länge bestimmt werden kann
			bool sync_found = false;
			while((m_block_count * 120) < ninput_items[0])
			{
				for(int32_t i = 0; i < 120; ++i)
				{
					if(sync_in[m_block_count * 120 + i] != 0)
					{					
						if(m_debug_enable != 0)
							printf("Sync found: %d\n", m_block_count * 120 + i);
						
						sync_found = true;
						break;
					}
				}
				
				if(sync_found)
					break;
				
				++m_block_count;
			}	
			
			if(m_debug_enable != 0)
				printf("Block Count: %d\n", m_block_count);
			
			// Wenn kein Sync gefunden wurde, die Größe der Ein- und Ausgangspuffer ändern
			if(!sync_found)
			{
				set_output_multiple((m_block_count + 1) * 110 * sizeof(uint8_t));
				return 0;
			}
			
			// Komplettes Superframe ohne Parityblock kopieren
			memcpy(out, in, m_block_count * 110 * sizeof(uint8_t));
			memset(sync_out, 0, m_block_count * 110 * sizeof(uint8_t));
			sync_out[0] = 1;
			
			for(int32_t i = 0; i < m_block_count; ++i)
			{
				// Eingangsdaten scramblen
				for(int32_t j = 0; j < 120; ++j)
					m_rs_packet[j] = in[j * m_block_count + i];
				
				// Fehlerstellen Array + Korrekturwerte berechnen
				int32_t error_count = decode_rs_char(m_rs_handle, (data_t *)m_rs_packet, m_correction_positions, 0);

				if(error_count < 0)
				{
					if(m_debug_enable != 0)
						printf("Reed Solomon Decoder: Uncorrectable Errors!\n");
					
					// Audio Frame verwerfen
					int32_t temp_block_count = m_block_count;
					m_block_count = 1;
					consume_each (temp_block_count * 120);
					return 0;
				}
				
				// Fehler korrigieren
				for (int32_t j = 0; j < error_count; ++j) 
				{
					int32_t pos = m_correction_positions[j] - 135;
					if ((pos < 0) || (pos >= 110))
						continue;

					out[pos * m_block_count + i] = m_rs_packet[pos];
				}
			}

			// Die Größe der Ein- und Ausgangspuffer auf aktuelle Frame-Länge setzen
			set_output_multiple(m_block_count * 110 * sizeof(uint8_t));
			
			
			int32_t temp_block_count = m_block_count;
			
			// Es wird immer mindestens ein RS-Block verarbeitet
			m_block_count = 1;
			
			// GnuRadio mitteilen, wieviele Bytes eingelesen und ausgegeben werden.
			consume_each (temp_block_count * 120);
			return temp_block_count * 110;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

