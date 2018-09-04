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
- Block: Frequency_Deinterleaver
- Beschreibung: Vertauschen der Symbole über die Frequenzrichtung. Position des Unterträgers wird nach dem Muster in [1, S.110] neu geordnet.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Header_File.h"

#include <gnuradio/io_signature.h>
#include "Frequency_Deinterleaver_impl.h"

namespace gr {
  namespace HsKA_DAB_plus {

	Frequency_Deinterleaver::sptr
	Frequency_Deinterleaver::make(int N, int fft_length)
	{
	return gnuradio::get_initial_sptr(new Frequency_Deinterleaver_impl(N, fft_length));
	}

	Frequency_Deinterleaver_impl::Frequency_Deinterleaver_impl(int N, int fft_length)
	: gr::block("Frequency_Deinterleaver", gr::io_signature::make(1, 1, N*sizeof(gr_complex)), gr::io_signature::make(1, 1, N*sizeof(gr_complex)))
	{
		this->fft_length = fft_length;
		vector_length = N;
	}

	Frequency_Deinterleaver_impl::~Frequency_Deinterleaver_impl()
	{
	}

	void
	Frequency_Deinterleaver_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
	{
		for(int i = 0; i < ninput_items_required.size(); ++i)
		{
			ninput_items_required[i] = noutput_items;
		}
	}

	int
	Frequency_Deinterleaver_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
	{
	const gr_complex *in = (const gr_complex *) input_items[0];
	gr_complex *out = (gr_complex *) output_items[0];

	for(int k = 0; k < noutput_items; ++k)
	{
		process_single_vector(&in[k*vector_length], &out[k*vector_length]);
	}

	consume_each (noutput_items);
	return noutput_items;
	}
	
	void 
	Frequency_Deinterleaver_impl::process_single_vector(gr_complex const *in, gr_complex *out)
	{
	int permutation = 0;
	int output_index = 0;
	int input_index = 0;

	for(int i = 0; i < fft_length; ++i)
	{
		if(permutation >= (fft_length-vector_length)/2 && permutation <= fft_length-(fft_length-vector_length)/2 && permutation != fft_length/2)
		{	
			output_index = permutation-fft_length/2+vector_length/2;
			if(permutation > fft_length/2)
			{
				output_index--;
			}	

			out[input_index] = in[output_index];
			input_index++;
		}

		// Für fft_length = 2048:
		// permutation(k+1) = (13*permutation(k) + 511) mod 2048
		permutation = (13 * permutation + 511) % (fft_length);
	}
	}

  } /* namespace HsKA_DAB_plus */
} /* namespace gr */

