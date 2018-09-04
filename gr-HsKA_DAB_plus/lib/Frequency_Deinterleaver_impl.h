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

#ifndef INCLUDED_HSKA_DAB_PLUS_FREQUENCY_DEINTERLEAVER_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_FREQUENCY_DEINTERLEAVER_IMPL_H

#include <HsKA_DAB_plus/Frequency_Deinterleaver.h>

namespace gr {
  namespace HsKA_DAB_plus {

	class Frequency_Deinterleaver_impl : public Frequency_Deinterleaver
	{
	private:
	int fft_length;
	int vector_length;
			
	public:
	Frequency_Deinterleaver_impl(int N, int fft_length);
	~Frequency_Deinterleaver_impl();

	void process_single_vector(gr_complex const *in, gr_complex *out);

	void forecast (int noutput_items, gr_vector_int &ninput_items_required);
	int general_work(int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
	};

  } // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_FREQUENCY_DEINTERLEAVER_IMPL_H */

