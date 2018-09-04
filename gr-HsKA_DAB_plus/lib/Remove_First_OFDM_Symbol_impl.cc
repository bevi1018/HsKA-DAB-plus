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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Header_File.h"

#include <gnuradio/io_signature.h>
#include "Remove_First_OFDM_Symbol_impl.h"

namespace gr {
  namespace HsKA_DAB_plus {

	Remove_First_OFDM_Symbol::sptr
	Remove_First_OFDM_Symbol::make(int vector_length)
	{
	return gnuradio::get_initial_sptr(new Remove_First_OFDM_Symbol_impl(vector_length));
	}

	Remove_First_OFDM_Symbol_impl::Remove_First_OFDM_Symbol_impl(int vector_length)
	: gr::block("Remove_First_OFDM_Symbol", gr::io_signature::make2(2, 2, vector_length*sizeof(gr_complex), sizeof(byte)), gr::io_signature::make2(2, 2, vector_length*sizeof(gr_complex), sizeof(byte))),
	  d_vlen(vector_length), d_start(0)
	{}

	Remove_First_OFDM_Symbol_impl::~Remove_First_OFDM_Symbol_impl()
	{
	}

	void
	Remove_First_OFDM_Symbol_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
	{
		for(int i = 0; i < ninput_items_required.size(); ++i)
		{
			ninput_items_required[i] = noutput_items;
		}
	}

	int
	Remove_First_OFDM_Symbol_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
	{
	const gr_complex *iptr = (const gr_complex *) input_items[0];
	const char *frame_start = (const char *) input_items[1];
	gr_complex *optr = (gr_complex *) output_items[0];
	char *o_frame_start = (char *) output_items[1];

	int n_consumed = 0;
	int n_produced = 0;

	for (n_consumed=0; n_consumed<ninput_items[0] && n_consumed<ninput_items[1] && n_produced<noutput_items; n_consumed++)
	{
		if (*frame_start == 1)
		{
			d_start = 1;
			iptr += d_vlen;
		}
		else
		{
			*o_frame_start++ = d_start;
			n_produced++;
			d_start = 0;
			for (unsigned int j=0; j<d_vlen; j++)
			{
				*optr++ = *iptr++;
			}
		}
		frame_start++;
	}

	// printf("ninput_items[0]: %d, ninput_items[1]: %d, noutput_items: %d, consumed: %d, produced: %d\n", ninput_items[0], ninput_items[1], noutput_items, n_consumed, n_produced);

	consume_each(n_consumed);
	return n_produced;
	}

  } /* namespace HsKA_DAB_plus */
} /* namespace gr */

