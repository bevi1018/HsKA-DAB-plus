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
- Datum: 02.09.18
--------------------------------------------------
Informationen
- Block: Soft_Demapper
- Beschreibung: Softbits und Mapping nach [1, S. 110]. Zuerst alle 1536 Realteile gefolgt von den Imaginärteilen.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "Soft_Demapper_impl.h"

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		Soft_Demapper::sptr Soft_Demapper::make(int32_t vector_length)
		{
			// Instanz des Blocks erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new Soft_Demapper_impl(vector_length));
		}

		Soft_Demapper_impl::Soft_Demapper_impl(int32_t vector_length)
			: gr::block("Soft_Demapper", 
						gr::io_signature::make(1, 1, vector_length * sizeof(gr_complex)),
						gr::io_signature::make(1, 1, 2 * vector_length * sizeof(float))),
			  m_vector_length(vector_length)
		{			
		}

		Soft_Demapper_impl::~Soft_Demapper_impl()
		{
		}

		void Soft_Demapper_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden.
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int32_t Soft_Demapper_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Ausgangsdaten zur einfacheren Handhabung casten
			const gr_complex *in	= (const gr_complex *) input_items[0];
			float *out				= (float *) output_items[0];

			for(int32_t i = 0; i < m_vector_length; ++i)
			{
				// Zuerst kommen die Realteile, dann die Imaginärteile
				out[i]						= in[i].real();
				out[i + m_vector_length]	= in[i].imag();
			}

			// Es wird nur ein Vektor eingelesen
			consume_each(1);
			
			// Es wird nur ein Vektor ausgegeben
			return 1;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

