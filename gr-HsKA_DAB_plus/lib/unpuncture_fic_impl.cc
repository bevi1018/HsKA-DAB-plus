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
#include "unpuncture_fic_impl.h"

#include <HsKA_DAB_plus/ArraySize.h>
#include <HsKA_DAB_plus/puncturing_vectors.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		unpuncture_fic::sptr unpuncture_fic::make(int32_t vector_length)
		{
			// Instanz des Blocks erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new unpuncture_fic_impl(vector_length));
		}

		/*
		* The private constructor
		*/
		unpuncture_fic_impl::unpuncture_fic_impl(int32_t vector_length)
			: gr::block("unpuncture_fic",
						gr::io_signature::make(1, 1, 2*vector_length*3/4*sizeof(float)),
						gr::io_signature::make(1, 1, sizeof(float))),
			  m_vector_length(vector_length)
		{
		}

		/*
		* Our virtual destructor.
		*/
		unpuncture_fic_impl::~unpuncture_fic_impl()
		{
		}

		void unpuncture_fic_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden mindestens so viele Eingangsdaten wie Daten ausgegeben werden sollen
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		void unpuncture_fic_impl::unpuncture(int out_index, int *in_index, const uint8_t *puncturing_vector, int32_t puncturing_vector_length, const float *in, float *out)
		{	
			if(puncturing_vector[out_index % puncturing_vector_length] != 0)
			{
				out[out_index] = in[*in_index];
				++(*in_index);
			}
			else
			{
				out[out_index] = 0;
			}
		}

		int32_t unpuncture_fic_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			const float *in = (const float *) input_items[0];	
			float *out      = (float *) output_items[0];

			int block_index = 0;
			int in_index = 0;

			for(int i = 0; i < m_vector_length * 2 + 24; ++i)
			{
				// Ein Block besteht aus 128 Bits.
				block_index = i / 128;

				// Die ersten 21 Blöcke werden mit PI=16 depunktiert, die restlichen 3 Blöcke werden mit PI=15 depunktiert.
				// Die Tail-Bits werden mit dem Tail-Vektor depunktiert.
				if(block_index < 21)
					unpuncture(i, &in_index, puncturing_vector_16, array_size(puncturing_vector_16), in, out);
				else if(i < m_vector_length * 2)
					unpuncture(i, &in_index, puncturing_vector_15, array_size(puncturing_vector_15), in, out);
				else
					unpuncture(i, &in_index, puncturing_vector_tail, array_size(puncturing_vector_tail), in, out);
			}

			// Es wird nur ein Vektor eingelesen
			consume(0,1);

			// Es wird ein Stream ausgegeben
			produce(0, m_vector_length * 2 + 24);

			return WORK_CALLED_PRODUCE;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

