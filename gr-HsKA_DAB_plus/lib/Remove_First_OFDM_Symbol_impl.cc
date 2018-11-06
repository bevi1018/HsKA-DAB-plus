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

#include <gnuradio/io_signature.h>
#include "Remove_First_OFDM_Symbol_impl.h"

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		Remove_First_OFDM_Symbol::sptr Remove_First_OFDM_Symbol::make(int32_t vector_length)
		{
			// Instanz des Blocks erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new Remove_First_OFDM_Symbol_impl(vector_length));
		}

		Remove_First_OFDM_Symbol_impl::Remove_First_OFDM_Symbol_impl(int32_t vector_length)
			: gr::block("Remove_First_OFDM_Symbol",
						gr::io_signature::make2(2, 2, vector_length * sizeof(gr_complex), sizeof(uint8_t)),
						gr::io_signature::make2(2, 2, vector_length * sizeof(gr_complex), sizeof(uint8_t))),
						m_vector_length(vector_length), m_last_was_phase_ref(false)
		{
		}

		Remove_First_OFDM_Symbol_impl::~Remove_First_OFDM_Symbol_impl()
		{
		}

		void Remove_First_OFDM_Symbol_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden.
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int Remove_First_OFDM_Symbol_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			const gr_complex *in	= (const gr_complex *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			gr_complex *out			= (gr_complex *) output_items[0];
			uint8_t *sync_out		= (uint8_t *) output_items[1];
			
			if(sync_in[0] != 0)
			{
				m_last_was_phase_ref = true;
				consume_each(1);
				return 0;
			}
			else
			{
				memcpy(out, in, m_vector_length * sizeof(gr_complex));
				
				if(m_last_was_phase_ref)
					sync_out[0] = 1;
				else
					sync_out[0] = 0;
				
				m_last_was_phase_ref = false;
				
				consume_each(1);
				return 1;
			}
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

