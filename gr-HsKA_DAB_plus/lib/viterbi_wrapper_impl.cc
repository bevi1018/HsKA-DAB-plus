/* -*- c++ -*- */
/* 
 * Copyright 2018 HsKA.
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
#include "viterbi_wrapper_impl.h"

#include <HsKA_DAB_plus/viterbi_algorithm.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		viterbi_wrapper::sptr viterbi_wrapper::make(int32_t fsm_input_size, int32_t fsm_output_size, const std::vector<int32_t> &fsm_table, int32_t init_state, int32_t final_state, int32_t dimension, const std::vector<float> &table)
		{
			return gnuradio::get_initial_sptr(new viterbi_wrapper_impl(fsm_input_size, fsm_output_size, fsm_table, init_state, final_state, dimension, table));
		}

		/*
		* The private constructor
		*/
		viterbi_wrapper_impl::viterbi_wrapper_impl(int fsm_input_size, int fsm_output_size, const std::vector<int> &fsm_table, int init_state, int final_state, int dimension, const std::vector<float> &table)
			: gr::block("viterbi_wrapper", 
						gr::io_signature::make3(3, 3, sizeof(float), sizeof(uint8_t), sizeof(int)),
						gr::io_signature::make3(3, 3, sizeof(uint8_t), sizeof(uint8_t), sizeof(int))),
			  m_state_machine(fsm_input_size, fsm_output_size, fsm_table),
			  m_init_state(init_state),
			  m_final_state(final_state),
			  m_dimension(dimension),
			  m_table(table)
		{				
			m_max_block_size = 1;
			m_current_block_size = 1;
			set_relative_rate(1.0 / ((double)m_dimension));
			set_output_multiple(m_max_block_size);
		}

		/*
		* Our virtual destructor.
		*/
		viterbi_wrapper_impl::~viterbi_wrapper_impl()
		{
		}

		void viterbi_wrapper_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden k (m_dimension) mal soviele Eingangsdaten, wie Ausgangsdaten benoetigt.
			for(unsigned int i = 0; i < ninput_items_required.size(); i++)
			{
				ninput_items_required[i] = noutput_items * m_dimension;
			}
		}

		int viterbi_wrapper_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			const float *in			= (const float *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			const int *k_in			= (const int *) input_items[2];
			uint8_t *out			= (uint8_t *) output_items[0];
			uint8_t *sync_out		= (uint8_t *) output_items[1];
			int *k_out				= (int *) output_items[2];
			
			int input_count = ninput_items[0];
			
			bool sync_found = false;
			
			// Resynchronisierung, falls eine falsche Blocklänge übergeben wurde
			for(int i = 0; i < input_count; ++i)
			{
				if(sync_in[i] != 0)
				{
					m_current_block_size = k_in[i];
					
					// Wenn eine neue Größe festgelegt wurde, muss general_work neu aufgerufen werden,
					// damit die Änderungen übernommen werden. Um diesen Neustart zu vermeiden, wird dieser
					// nur durchgeführt, wenn die neue Blockgröße größer als die alte Blockgröße ist.
					if(m_current_block_size > m_max_block_size)
					{
						m_max_block_size = m_current_block_size;
						set_output_multiple(m_max_block_size);
						
						consume_each(i);
						return 0;	
					}
					else if(i != 0)
					{
						// Damit set_output_multiple() funktioniert, muss general_work neu aufgerufen werden,
						// sobald eine Resynchronisierung stattgefunden hat. Ansonsten wäre unter Umständen
						// der restliche Eingangspuffer zu klein, um den Viterbi auszuführen
						consume_each(i);
						return 0;	
					}
					else
					{
						sync_found = true;
						break;
					}
				}
			}			
			
			if(sync_found)
			{
				// Viterbi-Algorithmus ausführen
				gr::trellis::viterbi_algorithm_combined(m_state_machine.I(), m_state_machine.S(), m_state_machine.O(),
					m_state_machine.NS(), m_state_machine.OS(), m_state_machine.PS(),
					m_state_machine.PI(), m_current_block_size, m_init_state, m_final_state, m_dimension,
					m_table, digital::TRELLIS_EUCLIDEAN, in, out);

				memset(sync_out, 0, m_current_block_size * sizeof(uint8_t));
				sync_out[0] = 1;

				for(int i = 0; i < m_current_block_size; ++i)
					k_out[i] = m_current_block_size;

				consume_each(m_dimension * m_current_block_size);
				return m_current_block_size;
			}
			else
			{
				consume_each(input_count);
				return 0;
			}
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

