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
- Block: DQPSK_Demodulation
- Beschreibung: DQPSK Demodulation findet durch die Multiplikation des komplex konjugierten Wertes mit einem OFDM Symbol Verzögerung statt. D.h. DQPSK über die zeitliche
Richtung, also Symbole nacheinander auf dem Unterträger und nicht in Frequenzrichtung. Ändert sich der Symbolanfang theta, so verschiebt sich die Phasebeziehung. Für die
korrekte Relation muss ein Symbol korrigiert werden, da ansonsten die Phase bei den äußeren Unterträgern den Eindeutigkeitsbereich überschreiten kann.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Header_File.h"

#include <gnuradio/io_signature.h>
#include "DQPSK_Demodulation_impl.h"

namespace gr {
  namespace HsKA_DAB_plus {

	DQPSK_Demodulation::sptr
	DQPSK_Demodulation::make(int N, int fft_length)
	{
	return gnuradio::get_initial_sptr(new DQPSK_Demodulation_impl(N, fft_length));
	}

	DQPSK_Demodulation_impl::DQPSK_Demodulation_impl(int N, int fft_length)
	: gr::block("DQPSK_Demodulation", gr::io_signature::make3(3, 3, N*sizeof(gr_complex), sizeof(short), sizeof(byte)), gr::io_signature::make2(2, 2, N*sizeof(gr_complex), sizeof(byte)))
	{
		vector_length = N; // Anzahl der Unterträger
		this ->fft_length = fft_length;
		last_sample = new gr_complex[N]; // Speicherung des letzten OFDM Symbols

		for(int i = 0; i < N; ++i)
		{
			last_sample[i] = gr_complex(1, 0);
		}
	}

	DQPSK_Demodulation_impl::~DQPSK_Demodulation_impl()
	{
	}

	void
	DQPSK_Demodulation_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
	{
		for(int i = 0; i < ninput_items_required.size(); ++i)
		{
			ninput_items_required[i] = noutput_items;
		}
	}

	int
	DQPSK_Demodulation_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
	{
	const gr_complex *in = (const gr_complex *) input_items[0];
	const short *theta_in = (const short *) input_items[1];
	const byte *sync_in = (const byte *) input_items[2];
	gr_complex *out = (gr_complex *) output_items[0];
	byte *sync_out = (byte *) output_items[1];

	static short last_theta = 0;

	for(int i = 0; i < vector_length; ++i)
	{
		// Demodulation durch Multiplikation des komplex konjugierten letzten Symbols auf demselben Unterträger 
		// und Phasenkorrektur pro Unterträger, falls der Symbolanfang sich geändert hat
		out[i] = in[i] * conj(last_sample[i])*gr_expj(-2*PI*(i-vector_length/2)*(float)(theta_in[0]-last_theta)/(float)(fft_length));
		last_sample[i] = in[i];
	}
	last_theta = theta_in[0];
	sync_out[0] = sync_in[0];

	consume_each (1);
	return 1;
	}

  } /* namespace HsKA_DAB_plus */
} /* namespace gr */

