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

#ifndef INCLUDED_HSKA_DAB_PLUS_OFDM_SYMBOL_SYNCHRONISATION_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_OFDM_SYMBOL_SYNCHRONISATION_IMPL_H

#include <HsKA_DAB_plus/OFDM_Symbol_Synchronisation.h>

namespace gr {
  namespace HsKA_DAB_plus {

	class OFDM_Symbol_Synchronisation_impl : public OFDM_Symbol_Synchronisation
	{
	int N;
	int L;
	int Averaging;

	float rho;

	MovingSum<float> Epsilon_moving_sum;
	MovingSum<gr_complex> gamma_moving_sum;
	MovingSum<float> estimated_epsilon_moving_average;
	MovingSum<float> estimated_theta_moving_average;

	float Epsilon;
	gr_complex gamma;
	float Lambda;

	float find_maximum;
	int theta_maximum_likelihood;
	float epsilon_maximum_likelihood;
	gr_complex gamma_at_time_instant_theta;

	float phi;

	int symbol_counter;

	public:
	OFDM_Symbol_Synchronisation_impl(int N, int L, float SNR, int Averaging);
	~OFDM_Symbol_Synchronisation_impl();

	void forecast (int noutput_items, gr_vector_int &ninput_items_required);
	int general_work(int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
	};

  } // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_OFDM_SYMBOL_SYNCHRONISATION_IMPL_H */

