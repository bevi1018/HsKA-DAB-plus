/* -*- c++ -*- */
/* 
 * Copyright 2018 HsKA
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

#include <gnuradio/io_signature.h>
#include "Frequency_Deinterleaver_impl.h"

namespace gr
{
	namespace HsKA_DAB_plus 
	{
		Frequency_Deinterleaver::sptr Frequency_Deinterleaver::make(int32_t vector_length, int32_t fft_length)
		{
			// Instanz des Blocks erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new Frequency_Deinterleaver_impl(vector_length, fft_length));
		}

		Frequency_Deinterleaver_impl::Frequency_Deinterleaver_impl(int32_t vector_length, int32_t fft_length)
			: gr::block("Frequency_Deinterleaver",
						gr::io_signature::make(1, 1, vector_length * sizeof(gr_complex)),
						gr::io_signature::make(1, 1, vector_length * sizeof(gr_complex))),
			  m_fft_length(fft_length),
			  m_vector_length(vector_length)
		{
		}

		Frequency_Deinterleaver_impl::~Frequency_Deinterleaver_impl()
		{
		}

		void Frequency_Deinterleaver_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden.
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int32_t Frequency_Deinterleaver_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Augsgangsdaten für einfachere Handhabung casten
			const gr_complex *in = (const gr_complex *) input_items[0];
			gr_complex *out = (gr_complex *) output_items[0];

			// Jeden verfügbaren Datenvektor verarbeiten
			for(int32_t k = 0; k < noutput_items; ++k)
			{
				process_single_vector(&in[k * m_vector_length], &out[k * m_vector_length]);
			}

			// Es wurden noutput_items Vektoren verarbeitet
			consume_each (noutput_items);
			
			// Es werden noutput_items Vektoren ausgegeben
			return noutput_items;
		}

		void Frequency_Deinterleaver_impl::process_single_vector(gr_complex const *in, gr_complex *out)
		{
			int32_t permutation = 0;
			int32_t output_index = 0;

			for(int32_t i = 0; i < m_fft_length; ++i)
			{
				// Liegt der aktuelle Permutationsindex im gültigen Bereich?
				if((permutation >= (m_fft_length - m_vector_length) / 2) && (permutation <= (m_fft_length + m_vector_length) / 2) && (permutation != m_fft_length / 2))
				{	
					// input_index speichern
					int32_t input_index = permutation - (m_fft_length - m_vector_length) / 2;
					
					// Wenn der Permutationsindex über der Hälfte liegt, muss 1 abgezogen werden (DC-Anteil)
					if(permutation > m_fft_length / 2)
						input_index--;

					// Daten kopieren
					out[output_index] = in[input_index];
					output_index++;
				}

				// Für fft_length = 2048:
				// permutation(k+1) = (13*permutation(k) + 511) mod 2048
				// Neuer Permutationsindex speichern
				permutation = (13 * permutation + 511) % m_fft_length;
			}
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

