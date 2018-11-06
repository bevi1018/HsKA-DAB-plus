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
#include "time_deinterleaver_impl.h"

namespace gr
{
	namespace HsKA_DAB_plus 
	{
		// Interleavingindices
		const int time_deinterleaver_impl::interleaving_indices[] = { 0, 8, 4, 12, 2, 10, 6, 14, 1, 9, 5, 13, 3, 11, 7, 15 };
		
		time_deinterleaver::sptr time_deinterleaver::make(int32_t vector_length)
		{
			// Neues Objekt erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new time_deinterleaver_impl(vector_length));
		}

		/*
		* The private constructor
		*/
		time_deinterleaver_impl::time_deinterleaver_impl(int32_t vector_length)
			: gr::block("time_deinterleaver", 
						gr::io_signature::make2(2, 2, sizeof(float), sizeof(uint8_t)), 
						gr::io_signature::make2(2, 2, sizeof(float), sizeof(uint8_t)))
		{
			// Vektorlänge speichern
			m_vector_length = vector_length;
				
			// Read index zurücksetzen
			m_read_index = 0;

			// Interleaverbuffer erzeugen
			m_interleave_buffer = new float *[16];
			
			for(int32_t i = 0; i < 16; ++i)
			{
				m_interleave_buffer[i] = new float[vector_length];
			}
				
			// Festlegen, dass mindestens ein ganzer Vektor verarbeitet wrid
			set_output_multiple(vector_length);
		}

		time_deinterleaver_impl::~time_deinterleaver_impl()
		{
			// Speicher des Interleaverbuffer freigeben
			if(m_interleave_buffer)
			{
				for(int32_t i = 0; i < 16; ++i)
				{
					delete [] (m_interleave_buffer[i]);
				}

				delete [] m_interleave_buffer;
				m_interleave_buffer = 0;
			}
		}

		void time_deinterleaver_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int32_t time_deinterleaver_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Ausgangsdaten zur einfacheren Handhabung casten
			const float *in			= (const float *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			float *out				= (float *) output_items[0];
			uint8_t *sync_out		= (uint8_t *) output_items[1];
			
			// Re-sync
			if(sync_in[0] == 0)
			{
				uint32_t discarded_count = 1;
				
				while(discarded_count < noutput_items)
				{
					if(sync_in[discarded_count] != 0)
						break;
					
					++discarded_count;
				}
				
				printf("Time Deinterleaver: Resyncing!\n");
				
				consume_each(discarded_count);
				return 0;
			}
			
			// Eingangsdaten an die jeweilige Position schreiben
			for(int32_t i = 0; i < m_vector_length; ++i)
			{
				// Schreibindex berechnen
				int write_index = m_read_index + 16 - interleaving_indices[i % 16];

				// Schreibindex auf den richtigen Wertebereich bringen
				if(write_index >= 16)
					write_index -= 16;

				// Jeweiliges bit in den Interleaverbuffer schreiben
				m_interleave_buffer[write_index][i] = in[i];
			}

			// Der Ausgangsvektor kann dann am aktuelle Leseindex gelesen werden.
			memcpy(out, m_interleave_buffer[m_read_index], m_vector_length * sizeof(float));
			memset(sync_out, 0, m_vector_length * sizeof(uint8_t));
			sync_out[0] = 1;

			// Leseindex inkrementieren
			++m_read_index;
			if(m_read_index >= 16)
				m_read_index = 0;

			// Gnuradio Bescheid geben, dass ein Vektor verarbeitet wurde
			consume_each(m_vector_length);
			return m_vector_length;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

