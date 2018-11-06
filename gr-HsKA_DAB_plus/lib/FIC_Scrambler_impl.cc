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
#include "FIC_Scrambler_impl.h"

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		FIC_Scrambler::sptr FIC_Scrambler::make()
		{
			// Instanz des Blocks erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new FIC_Scrambler_impl());
		}

		/*
		* The private constructor
		*/
		FIC_Scrambler_impl::FIC_Scrambler_impl()
			: gr::block("FIC_Scrambler", 
						gr::io_signature::make(1, 1, sizeof(uint8_t)),
						gr::io_signature::make(1, 1, 256/8*sizeof(uint8_t)))
		{
			set_output_multiple(768/8);
		}

		/*
		* Our virtual destructor.
		*/
		FIC_Scrambler_impl::~FIC_Scrambler_impl()
		{
		}

		void FIC_Scrambler_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden mindestens so viele Eingangsdaten wie Daten ausgegeben werden sollen
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = 774*noutput_items/3;
			}
		}

		int32_t FIC_Scrambler_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Ausgangsdaten zum einfacheren Handling casten
			const uint8_t *in = (const uint8_t *) input_items[0];
			uint8_t *out      = (uint8_t *) output_items[0];

			uint32_t latch = 0xFFFFFFFF;
			uint8_t prbs = 0;

			for(int32_t i = 0; i < 768/8; i++)
			{	
				uint8_t data = 0;
				for(int32_t j = 0; j < 8; j++)
				{
					// Pseudorandombit generieren
					prbs = (latch >> 8) ^ (latch >> 4);
					latch <<= 1;
					latch |= (prbs & 1);

					// Ausgangsdaten erzeugen
					data <<= 1;
					data |= (prbs^in[i*8 + j]) & 0x01;
				}
				
				// Datum ausgeben
				out[i] = data;
			}

			// 774 Bit ergeben 3 FIB
			consume(0,774);
			produce(0,3);

			return WORK_CALLED_PRODUCE;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

