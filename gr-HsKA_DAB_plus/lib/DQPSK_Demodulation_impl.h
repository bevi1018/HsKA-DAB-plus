/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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

#ifndef INCLUDED_HSKA_DAB_PLUS_DQPSK_DEMODULATION_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_DQPSK_DEMODULATION_IMPL_H

#include <HsKA_DAB_plus/DQPSK_Demodulation.h>

namespace gr {
  namespace HsKA_DAB_plus {

	class DQPSK_Demodulation_impl : public DQPSK_Demodulation
	{
	private:
	gr_complex *last_sample;
	int vector_length;
	int fft_length;

	public:
	DQPSK_Demodulation_impl(int N, int fft_length);
	~DQPSK_Demodulation_impl();

	void forecast (int noutput_items, gr_vector_int &ninput_items_required);
	int general_work(int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
	};

  } // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_DQPSK_DEMODULATION_IMPL_H */

