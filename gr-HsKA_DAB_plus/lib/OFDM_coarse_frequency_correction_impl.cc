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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Header_File.h"

#include <gnuradio/io_signature.h>
#include "OFDM_coarse_frequency_correction_impl.h"

namespace gr {
  namespace HsKA_DAB_plus {

	OFDM_coarse_frequency_correction::sptr
	OFDM_coarse_frequency_correction::make(int fft_length, int num_carriers, int cp_length)
	{
	return gnuradio::get_initial_sptr(new OFDM_coarse_frequency_correction_impl(fft_length, num_carriers, cp_length));
	}

	OFDM_coarse_frequency_correction_impl::OFDM_coarse_frequency_correction_impl(int fft_length, int num_carriers, int cp_length)
	: gr::block("OFDM_coarse_frequency_correction", gr::io_signature::make2(2, 2, fft_length*sizeof(gr_complex), sizeof(byte)), gr::io_signature::make2(2, 2, num_carriers*sizeof(gr_complex), sizeof(byte))),
	  d_fft_length(fft_length), d_num_carriers(num_carriers), d_cp_length(cp_length), d_symbol_num(0), d_freq_offset(0), d_delta_f(0)
	{
		d_zeros_on_left = (d_fft_length-d_num_carriers)/2;
	}

	OFDM_coarse_frequency_correction_impl::~OFDM_coarse_frequency_correction_impl()
	{
	}

	void
	OFDM_coarse_frequency_correction_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
	{
		for(int i=0; i < ninput_items_required.size(); ++i)
		{
			ninput_items_required[i] = noutput_items;
		}
	}

	void
	OFDM_coarse_frequency_correction_impl::correlate_energy(const gr_complex *symbol)
	{
	unsigned int i, index;
	double sum=0, max=0;

	/* energy based correlation - note that DAB uses a zero central carrier -
	* we always sum up the energy for all carriers except DC; this is however
	* only done for the first offset; for the others, the diff is calculated
	*/

	/* offset 0 */
	for(i=0; i<d_num_carriers+1; i++)
	{
		if (i != d_num_carriers/2)
		sum += (double)norm(symbol[i]);
	}
	max = sum;
	index = 0;
	/* other offsets */
	for(i=1; i<d_fft_length-d_num_carriers; i++)
	{
		/* diff on left side */
		sum -= (double)norm(symbol[i-1]);
		/* diff for zero carrier */
		sum += (double)norm(symbol[i+d_num_carriers/2-1]);
		sum -= (double)norm(symbol[i+d_num_carriers/2]);
		/* diff on rigth side */
		sum += (double)norm(symbol[i+d_num_carriers]);
		/* new max found? */
		if (sum > max) 
		{
			max = sum;
			index = i;
		}
	}
	d_freq_offset = index;
	}

	int
	OFDM_coarse_frequency_correction_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
	{
	const gr_complex *iptr = (const gr_complex *) input_items[0];
	const char *frame_start = (const char *) input_items[1];
	gr_complex *optr = (gr_complex *) output_items[0];
	char *frame_start_out = (char *) output_items[1];

	unsigned int i;
	gr_complex phase_offset_correct;

	if (frame_start[0])
	{
		frame_start_out[0] = 1;
		correlate_energy(iptr);
		d_delta_f = d_freq_offset + d_num_carriers/2 - d_fft_length/2;
		d_symbol_num = 0;
	}
	else
	{
		frame_start_out[0] = 0;
		d_symbol_num++;
	}

	// correct phase offset from removing cp
	// could be done after diff phasor, then it would be the same offset for each symbol; but its hardly much of an overhead

	phase_offset_correct = gr_expj(-(2*PI)*(float)d_delta_f*(float)d_cp_length/(float)d_fft_length * (float)d_symbol_num);

	for (i=0; i<d_num_carriers/2; i++)
	{
		optr[i] = iptr[d_freq_offset+i]*phase_offset_correct;
	}
	for (i=d_num_carriers/2; i<d_num_carriers; i++)
	{
		optr[i] = iptr[d_freq_offset+i+1]*phase_offset_correct;
	}
	
	consume(0,1);
	consume(1,1);
	
	produce(0,1);
	produce(1,1);
	return WORK_CALLED_PRODUCE;
	}

  } /* namespace HsKA_DAB_plus */
} /* namespace gr */

