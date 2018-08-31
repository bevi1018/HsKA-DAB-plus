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

#ifndef INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_IMPL_H

#include <HsKA_DAB_plus/OFDM_coarse_frequency_correction.h>

namespace gr {
  namespace HsKA_DAB_plus {

	class OFDM_coarse_frequency_correction_impl : public OFDM_coarse_frequency_correction
	{
	private:
	void correlate_energy(const gr_complex *symbol);

	int d_fft_length;
	int d_num_carriers;
	int d_cp_length;
	int d_symbol_num;
	int d_zeros_on_left;
	int d_freq_offset;
	int d_delta_f;

	public:
	OFDM_coarse_frequency_correction_impl(int fft_length, int num_carriers, int cp_length);
	~OFDM_coarse_frequency_correction_impl();

	void forecast (int noutput_items, gr_vector_int &ninput_items_required);
	int general_work(int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
	};

  } // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_IMPL_H */

