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
#include "unpuncture_msc_impl.h"

#include <HsKA_DAB_plus/puncturing_vectors.h>
#include <HsKA_DAB_plus/UEP.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		unpuncture_msc::sptr unpuncture_msc::make(char *databaseID, uint32_t debug_enable)
		{
			// Instanz erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new unpuncture_msc_impl(databaseID, debug_enable));
		}

		unpuncture_msc_impl::unpuncture_msc_impl(char *databaseID, uint32_t debug_enable)
			: gr::block("unpuncture_msc_blocks",
						gr::io_signature::make2(2, 2, sizeof(float), sizeof(uint8_t)),
						gr::io_signature::make3(3, 3, sizeof(float), sizeof(uint8_t), sizeof(int32_t))),
			  m_database(shared_database::open_database(databaseID)),
			  m_debug_enable(debug_enable)
		{
			// Mindestanzahl an Outputdaten festlegen
			set_output_multiple(6936);
				  
			// Relative Rate festlegen
			//set_relative_rate(4);  

		    // Punktierungsvektoren zur einfacheren Handhabung in ein Array speichern
			puncture[0] = puncturing_vector_1;
			puncture[1] = puncturing_vector_2;
			puncture[2] = puncturing_vector_3;
			puncture[3] = puncturing_vector_4;
			puncture[4] = puncturing_vector_5;
			puncture[5] = puncturing_vector_6;
			puncture[6] = puncturing_vector_7;
			puncture[7] = puncturing_vector_8;
			puncture[8] = puncturing_vector_9;
			puncture[9] = puncturing_vector_10;
			puncture[10] = puncturing_vector_11;
			puncture[11] = puncturing_vector_12;
			puncture[12] = puncturing_vector_13;
			puncture[13] = puncturing_vector_14;
			puncture[14] = puncturing_vector_15;
			puncture[15] = puncturing_vector_16;
			puncture[16] = puncturing_vector_17;
			puncture[17] = puncturing_vector_18;
			puncture[18] = puncturing_vector_19;
			puncture[19] = puncturing_vector_20;
			puncture[20] = puncturing_vector_21;
			puncture[21] = puncturing_vector_22;
			puncture[22] = puncturing_vector_23;
			puncture[23] = puncturing_vector_24;
			puncture[24] = puncturing_vector_tail;
		}

		/*
		* Our virtual destructor.
		*/
		unpuncture_msc_impl::~unpuncture_msc_impl()
		{
		}

		void unpuncture_msc_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int32_t unpuncture_msc_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{	
			// Ein- und Ausgangsdaten zur einfacheren Handhabung casten
			const float *in			= (const float *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			float *out				= (float *) output_items[0];
			uint8_t *sync_out		= (uint8_t *) output_items[1];
			int32_t *viterbi_param	= (int32_t *) output_items[2];

			
			int32_t form = 1;
			int32_t option = 0;

			int32_t table_index = 0;

			int32_t protection_level = 2;
			int32_t subchannelsize = 54;

			int32_t n = 0;
			int32_t bitrate = 0;
			int32_t input_index = 0;
			int32_t output_index = 0;
			
			// Re-sync
			if(sync_in[0] == 0)
			{
				uint32_t discarded_count = 1;
				
				while(discarded_count < noutput_items)
				{
					if(sync_in[discarded_count] != 0)
						break;
					
					++discarded_count;
				}
				
				if(m_debug_enable != 0)
					printf("Unpuncture MSC: Resyncing! Discarded: %d\n", discarded_count);
				
				consume_each(discarded_count);
				return 0;
			}

			if(form == 1)
			{ // EEP
				if(option == 0)
				{ // protection profile A
					n = subchannelsize/6; 
					bitrate = n * 8;	

					switch(protection_level)
					{
					case 0: // 1-A -- coderate 1/4
						for(int32_t i = 0; i < 4*subchannelsize*64; ++i, ++output_index)
						{	
							if(i == 0)
							{
								sync_out[output_index]  = 1;
								viterbi_param[output_index] = (subchannelsize*128 + 24)/4;
							}
							else
							{
								sync_out[output_index]  = 0;
								viterbi_param[output_index] = 0;
							}

							if(i/128 < 6*n-3)
							{
								unpuncture(i, &input_index, puncturing_vector_24, 32, in, out);
							}
							else
							{
								unpuncture(i, &input_index, puncturing_vector_23, 32, in, out);	
							}
						}
							
						for(int32_t j = 0; j < 24; ++j, ++output_index)
						{
							unpuncture(j+4*subchannelsize*64, &input_index, puncturing_vector_tail, 24, in, out);
							sync_out[output_index]  = 0;
							viterbi_param[output_index] = 0;
						}
						break;
							
					case 1: // 2-A -- coderate 3/8
						for(int32_t i = 0; i < (8/3)*subchannelsize*64; ++i, ++output_index)
						{
							if(i == 0)
							{
								sync_out[output_index]  = 1;
								viterbi_param[output_index] = (subchannelsize*128 + 24)/4;
							}
							else
							{
								sync_out[output_index]  = 0;
								viterbi_param[output_index] = 0;
							}

							if(n != 0)
							{
								if(i/128 < 2*n-3)
								{
									unpuncture(i, &input_index, puncturing_vector_13, 32, in, out);
								}
								else
								{
									unpuncture(i, &input_index, puncturing_vector_23, 32, in, out);	
								}
							}
							else
							{
								if(i/128 < 5)
								{
									unpuncture(i, &input_index, puncturing_vector_13, 32, in, out);
								}
								else
								{
									unpuncture(i, &input_index, puncturing_vector_12, 32, in, out);	
								}
							}
						}
							
						for(int32_t j = 0; j < 24; ++j, ++output_index)
						{
							unpuncture(j+(8/3)*subchannelsize*64, &input_index, puncturing_vector_tail, 24, in, out);
							sync_out[output_index]  = 0;
							viterbi_param[output_index] = 0;
						}
						break;
							
					case 2: // 3-A -- coderate 1/2
						for(int32_t i = 0; i < 2*subchannelsize*64; ++i, ++output_index)
						{
							if(i == 0)
							{
								sync_out[output_index]  = 1;
								viterbi_param[output_index] = (subchannelsize*128 + 24)/4;
							}
							else
							{
								sync_out[output_index]  = 0;
								viterbi_param[output_index] = 0;
							}

							if(i/128 < 6*n-3)
							{
								unpuncture(i, &input_index, puncturing_vector_8, 32, in, out);
							}
							else
							{
								unpuncture(i, &input_index, puncturing_vector_7, 32, in, out);	
							}						
						}
							
						for(int32_t j = 0; j < 24; ++j, ++output_index)
						{
							unpuncture(j+2*subchannelsize*64, &input_index, puncturing_vector_tail, 24, in, out);
							sync_out[output_index]  = 0;
							viterbi_param[output_index] = 0;
						}
						break;
							
					case 3: // 4-A -- coderate 3/4
						for(int32_t i = 0; i < (4/3)*subchannelsize*64; ++i, ++output_index)
						{
							if(i == 0)
							{
								sync_out[output_index]  = 1;
								viterbi_param[output_index] = (subchannelsize*128 + 24)/4;
							}
							else
							{
								sync_out[output_index]  = 0;
								viterbi_param[output_index] = 0;
							}

							if(i/128 < 4*n-3)
							{
								unpuncture(i, &input_index, puncturing_vector_3, 32, in, out);
							}
							else
							{
								unpuncture(i, &input_index, puncturing_vector_2, 32, in, out);	
							}		
						}
							
						for(int32_t j = 0; j < 24; ++j, ++output_index)
						{
							unpuncture(j+(4/3)*subchannelsize*64, &input_index, puncturing_vector_tail, 24, in, out);
							sync_out[output_index]  = 0;
							viterbi_param[output_index] = 0;
						}
						break;
					default:
						break;
					}
				}
				else if(option == 1)
				{ // protection profile B
					n = subchannelsize/24; 
					bitrate = n * 32;				

					switch(protection_level)
					{
					case 0: // 1-B -- coderate 4/9
						for(int32_t i = 0; i < (9/4)*subchannelsize*64; ++i, ++output_index)
						{
							if(i == 0)
							{
								sync_out[output_index]  = 1;
								viterbi_param[output_index] = (subchannelsize*128 + 24)/4;
							}
							else
							{
								sync_out[output_index]  = 0;
								viterbi_param[output_index] = 0;
							}

							if(i/128 < 24*n-3)
							{
								unpuncture(i, &input_index, puncturing_vector_10, 32, in, out);
							}
							else
							{
								unpuncture(i, &input_index, puncturing_vector_9, 32, in, out);	
							}		
						}
							
						for(int32_t j = 0; j < 24; ++j, ++output_index)
						{
							unpuncture(j+(9/4)*subchannelsize*64, &input_index, puncturing_vector_tail, 24, in, out);
							sync_out[output_index]  = 0;
							viterbi_param[output_index] = 0;
						}
						break;
							
					case 1: // 2-B -- coderate 4/7
						for(int32_t i = 0; i < (7/4)*subchannelsize*64; ++i, ++output_index)
						{
							if(i == 0)
							{
								sync_out[output_index]  = 1;
								viterbi_param[output_index] = (subchannelsize*128 + 24)/4;	
							}
							else
							{
								sync_out[output_index]  = 0;
								viterbi_param[output_index] = 0;
							}

							if(i/128 < 24*n-3)
							{
								unpuncture(i, &input_index, puncturing_vector_6, 32, in, out);
							}
							else
							{
								unpuncture(i, &input_index, puncturing_vector_5, 32, in, out);	
							}		
						}
							
						for(int32_t j = 0; j < 24; ++j, ++output_index)
						{
							unpuncture(j+(7/4)*subchannelsize*64, &input_index, puncturing_vector_tail, 24, in, out);
							sync_out[output_index]  = 0;
							viterbi_param[output_index] = 0;
						}
						break;
							
					case 2: // 3-B -- coderate 4/6
						for(int32_t i = 0; i < (6/4)*subchannelsize*64; ++i, ++output_index)
						{
							if(i == 0)
							{
								sync_out[output_index]  = 1;
								viterbi_param[output_index] = (subchannelsize*128 + 24)/4;
							}
							else
							{
								sync_out[output_index]  = 0;
								viterbi_param[output_index] = 0;
							}

							if(i/128 < 24*n-3)
							{
								unpuncture(i, &input_index, puncturing_vector_4, 32, in, out);
							}
							else
							{
								unpuncture(i, &input_index, puncturing_vector_3, 32, in, out);	
							}		
						}
							
						for(int32_t j = 0; j < 24; ++j, ++output_index)
						{
							unpuncture(j+(6/4)*subchannelsize*64, &input_index, puncturing_vector_tail, 24, in, out);
							sync_out[output_index]  = 0;
							viterbi_param[output_index] = 0;
						}
						break;
							
					case 3: // 4-B -- coderate 4/5
						for(int32_t i = 0; i < (5/4)*subchannelsize*64; ++i, ++output_index)
						{
							if(i == 0)
							{
								sync_out[output_index]  = 1;
								viterbi_param[output_index] = (subchannelsize*128 + 24)/4;
							}
							else
							{
								sync_out[output_index]  = 0;
								viterbi_param[output_index] = 0;
							}

							if(i/128 < 24*n-3)
							{
								unpuncture(i, &input_index, puncturing_vector_2, 32, in, out);
							}
							else
							{
								unpuncture(i, &input_index, puncturing_vector_1, 32, in, out);	
							}		
						}
							
						for(int32_t j = 0; j < 24; ++j, ++output_index)
						{
							unpuncture(j+(5/4)*subchannelsize*64, &input_index, puncturing_vector_tail, 24, in, out);
							sync_out[output_index]  = 0;
							viterbi_param[output_index] = 0;
						}
						break;
							
					default:
						break;
					}
				}
				else
				{
					return 0;
				}
			}
			else
			{ // UEP
				subchannelsize = uep_table8[table_index][1];
				protection_level = uep_table8[table_index][2];
				bitrate = uep_table8[table_index][3];

				int32_t L = 0;
				int32_t table14_index = 0;
				int32_t table15_index = 0;
				int32_t k = 0;

				for(int32_t i = 0; i < 14; ++i)
				{
					if(uep_table14[i][0] == bitrate)
					{
						L = uep_table14[i][2];
						table14_index = i;
					}
				}

				for(int32_t i = 0; i < 64; ++i)
				{
					if(uep_table15[i][0] == bitrate && uep_table15[i][1] == protection_level)
					{	
						table15_index = i;
					}
				}

				for(int32_t i = 0; i < L*128-uep_table15[table15_index][10]; ++i, ++output_index)
				{
					if(i == 0)
					{
						sync_out[output_index]  = 1;
						viterbi_param[output_index] = (L*128-uep_table15[table15_index][10] + 24)/4; 
					}
					else
					{
						sync_out[output_index]  = 0;
						viterbi_param[output_index] = 0;
					}

					if(k < uep_table15[table15_index][2]) // L1
					{
						unpuncture(i, &input_index, puncture[uep_table15[table15_index][6]], 32, in, out);
					}
					else if(k < uep_table15[table15_index][3] + uep_table15[table15_index][2]) // L2
					{
						unpuncture(i, &input_index, puncture[uep_table15[table15_index][7]], 32, in, out);
					}
					else if(k < uep_table15[table15_index][4] + uep_table15[table15_index][3] + uep_table15[table15_index][2]) // L3
					{
						unpuncture(i, &input_index, puncture[uep_table15[table15_index][8]], 32, in, out);
					}
					else // L4
					{
						unpuncture(i, &input_index, puncture[uep_table15[table15_index][9]], 32, in, out);
					}

					k = i/128;
				}	

				for(int32_t j = 0; j < 24; ++j, ++output_index)
				{
					unpuncture(j+L*128-uep_table15[table15_index][10], &input_index, puncturing_vector_tail, 24, in, out);
					sync_out[output_index]  = 0;
					viterbi_param[output_index] = 0;
				}
			}

			if(m_debug_enable != 0)
				printf("Unpuncture: Input: %i, Output: %i, Noutput_items: %i\n", input_index, output_index, noutput_items);

			consume_each(input_index);
			return output_index;
		}

		void unpuncture_msc_impl::unpuncture(int32_t out_index, int32_t *in_index, const uint8_t *puncturing_vector, int32_t puncturing_vector_length, const float *in, float *out)
		{	
			if(puncturing_vector[out_index % puncturing_vector_length] != 0)
			{
				out[out_index] = in[*in_index];
				++(*in_index);
			}
			else
			{
				out[out_index] = 0;
			}
		}	  
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

