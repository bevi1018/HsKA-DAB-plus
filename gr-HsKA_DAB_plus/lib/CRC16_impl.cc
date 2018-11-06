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
#include "CRC16_impl.h"

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		CRC16::sptr CRC16::make(int32_t polynom, int32_t length, int32_t debug_enable)
		{
			return gnuradio::get_initial_sptr(new CRC16_impl(polynom, length, debug_enable));
		}

		/*
		* The private constructor
		*/
		CRC16_impl::CRC16_impl(int32_t polynom, int32_t length, int32_t debug_enable)
			: gr::block("CRC16",
						gr::io_signature::make(1, 1, length*sizeof(uint8_t)),
						gr::io_signature::make(1, 1, length*sizeof(uint8_t))),
			m_polynom(polynom),
			m_length(length),
			m_debug_enable(debug_enable)
		{
			// CRC16/FALSE-CCITT Tabelle berechnen, damit die CRC byteweise berechnet werden kann
			for(int32_t i = 0; i < 256; ++i)
			{
				uint16_t crc = i << 8;
				
				for(int32_t bit = 0; bit < 8; ++bit)
				{
					if(crc & 0x8000)
						crc = (crc << 1) ^ polynom;
					else
						crc = (crc << 1);
				}
				
				m_crc_table[i] = crc;
			}
		}

		/*
		* Our virtual destructor.
		*/
		CRC16_impl::~CRC16_impl()
		{
		}

		void CRC16_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int32_t CRC16_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			const uint8_t *in = (const uint8_t *) input_items[0];
			uint8_t *out = (uint8_t *) output_items[0];
			
			// CRC initialisieren
			uint16_t crc = 0xFFFF;
			
			// CRC16 byteweise berechnen
			for(int32_t i = 0; i < m_length-2; ++i)				
				crc = (crc << 8) ^ m_crc_table[((crc >> 8) ^ in[i]) & 0x00FF];

			
			crc = ~crc;
			uint16_t crc_received = (in[m_length - 2] << 8) | in[m_length - 1];
			
			// Wenn die Debugausgabe aktiv ist...
			if(m_debug_enable >= 2)
			{
				if(crc == crc_received)
					printf("CRC16 Check successful!\n");
				
				printf("CRC16 (calculated): 0x%04hX\n", crc);
				printf("CRC16 (received): 0x%04hX\n", crc_received);
			}
				
			if(m_debug_enable >= 1)
			{
				static uint32_t failed_count = 0;
				++failed_count;
				if(crc != crc_received)
					printf("CRC16 Check failed (%d)!\n", failed_count);
			}
			
			// Es wurde ein Vektor eingelesen
			consume(0,1);

			// Wenn die CRC gültig ist:
			if(crc == crc_received)
			{
				// Daten kopieren
				memcpy(out, in, m_length * sizeof(uint8_t));
				
				// Und signalisieren, dass ein Vektor ausgegeben wurde.
				produce(0,1);
			}

			return WORK_CALLED_PRODUCE;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

