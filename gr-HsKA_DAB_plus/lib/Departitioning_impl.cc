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
- Datum: 04.09.18
--------------------------------------------------
Informationen
- Block: Departitioning
- Beschreibung: Dieser Block trennt den Fast Information Channel von dem Main Service Channel. Beim Main Service Channel wird nur der aktuell eingestellte Subchannel ausgegeben
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "Departitioning_impl.h"

#include <HsKA_DAB_plus/FIB_Info.h>

#define CU_SIZE 64

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		Departitioning::sptr Departitioning::make(char *databaseID, int32_t vector_length, int32_t debug_enable)
		{
			// Instanz des Blocks erzeugen und GnuRadio zur Verfügung stellen
			return gnuradio::get_initial_sptr(new Departitioning_impl(databaseID, vector_length, debug_enable));
		}

		Departitioning_impl::Departitioning_impl(char *databaseID, int32_t vector_length, int32_t debug_enable)
			: gr::block("Departitioning", 
						gr::io_signature::make2(2, 2, 2 * vector_length * sizeof(float), sizeof(uint8_t)),
						gr::io_signature::make3(3, 3, vector_length * 2 * 3 / 4 * sizeof(float), sizeof(float), sizeof(uint8_t))),
			  m_database(shared_database::open_database(databaseID)),
			  m_vector_length(vector_length),
			  m_write_index(0),
			  m_remaining_fic_count(0),
			  m_symbol_counter(0),
			  m_cif_length(vector_length * 2 * 72 / 4), // 18 mal Symbollänge
			  m_fic_length(vector_length * 2 * 3 / 4),
			  m_debug_enable(debug_enable)
		{
			m_cif_buffer = new float[m_cif_length];
			m_fic_buffer = new float[3 * m_fic_length]; // Einer der 4 FIC-Blöcke kann direkt ausgegeben werden

			set_min_output_buffer(416 * CU_SIZE * sizeof(float));
		}

		Departitioning_impl::~Departitioning_impl()
		{
			// CIF-Speicher löschen
			if(m_cif_buffer)
			{
				delete [] m_cif_buffer;
				m_cif_buffer = 0;
			}

			// FIC-Speicher löschen
			if(m_fic_buffer)
			{
				delete [] m_fic_buffer;
				m_fic_buffer = 0;
			}
		}

		void Departitioning_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es wird immer nur ein Symbol eingelesen
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = 1;
			}
		}

		int32_t Departitioning_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Ein- und Ausgangsdaten zur einfacheren Handhabung casten
			const float *in = (const float *) input_items[0];
			const uint8_t *sync_in = (const uint8_t *) input_items[1];

			float *fic_block_out = (float *) output_items[0];
			float *subch_out = (float *) output_items[1];
			uint8_t *subch_sync = (uint8_t *) output_items[2];
			
			static bool is_synced = false;

			// Müssen noch gespeicherte FIC-Blöcke ausgegeben werden?
			if(m_remaining_fic_count > 0)
			{
				int start_index = (3 - m_remaining_fic_count) * m_fic_length;

				// 1 FIC-Block kopieren
				memcpy(fic_block_out, &m_fic_buffer[start_index], m_fic_length * sizeof(float));

				--m_remaining_fic_count;

				if(m_debug_enable != 0)
					printf("Departitioning: FIC Block (%d)\n", (4 - m_remaining_fic_count));

				// Signalisieren, dass 1 FIC-Block ausgegeben wurde.
				produce(0, 1);
				return WORK_CALLED_PRODUCE;
			}

			// Ansonsten neue Daten verarbeiten
			for(int i = 0; i < ninput_items[0]; ++i)
			{
				if(sync_in[i] != 0)
				{
					m_symbol_counter = 1;
					m_write_index = 0;
					m_remaining_fic_count = 0;
					
					is_synced = true;
					
					if(m_debug_enable != 0)
						printf("Departitioning: Sync received (%d)\n", (int)(sync_in[i]));
				}
				else if(!is_synced)
					continue;
				else
				{
					m_symbol_counter++;
				}

				// 1 Symbol in den Block-Buffer kopieren
				memcpy(&m_cif_buffer[m_write_index * m_vector_length * 2], &in[i * m_vector_length * 2], m_vector_length * 2 * sizeof(float));
				++m_write_index;

				//printf("write_index: %d\n", write_index);
				//printf("symbol_counter: %d\n", symbol_counter);

				// Wenn die ersten 3 Symbole empfangen wurden -> FIC partitionieren
				if(m_symbol_counter == 3)
				{
					// Die Blöcke 2-4 zwischenspeichern
					memcpy(m_fic_buffer, &m_cif_buffer[m_fic_length], 3 * m_fic_length * sizeof(float));
					m_remaining_fic_count = 3;

					// 1. FIC-Block kopieren
					memcpy(fic_block_out, m_cif_buffer, m_fic_length * sizeof(float));

					// Write-Index zurücksetzen, damit der nachfolgende CIF-Block mit Index 0 startet
					m_write_index = 0;
					
					if(m_debug_enable != 0)
						printf("Departitioning: FIC Block (1)\n");

					// Signalisieren, dass 1 FIC-Block ausgegeben wurde.
					produce(0, 1);

					// Signalisieren, dass i + 1 Symbole eingelesen wurden
					consume_each(i + 1);
					return WORK_CALLED_PRODUCE;
				}

				// Wenn der Block-Buffer komplett gefüllt ist, kann ein Subchannel ausgegeben werden (symbol_counter ist >= 21)
				if(m_write_index >= 18)
				{
					uint16_t start_address = 0;
					uint16_t subchannel_size = 0;
					uint32_t current_subchannel_id = 0;

					SubChannelOrga subch_orga;
					if(m_database->check_and_get_variable<uint32_t>(DB_SELECTED_SUBCHANNEL_ID, &current_subchannel_id))
					{
						if(m_debug_enable != 0)
							printf("Current Subchannel ID: %d\n", current_subchannel_id);
						
						if(m_database->check_and_get_variable<SubChannelOrga>(DB_SUBCHANNEL_ORGA_ID(current_subchannel_id), &subch_orga))
						{
							start_address = subch_orga.start_address;
							subchannel_size = subch_orga.sub_channel_size;
							
							if(m_debug_enable != 0)
								printf("Subchannel Startaddress: 0x%04hX, Size: %hd CUs\n", start_address, subchannel_size);

							// Den jeweiligen Subchannel des CIF-Blocks in des Ausgangsbuffer schreiben
							memcpy(subch_out, &m_cif_buffer[start_address * CU_SIZE], subchannel_size * CU_SIZE * sizeof(float));

							// Wenn dies der erste CIF-Block ist -> sync = 1
							memset(subch_sync, 0, subchannel_size * CU_SIZE * sizeof(uint8_t));
							subch_sync[0] = 1;

							if(m_debug_enable != 0)
								printf("Subchannel from CIF Block %d written.\n", (m_symbol_counter - 3) / 18);

							// Signalisieren, dass 1 CIF-Block ausgegeben wurde.
							produce(1, subchannel_size * CU_SIZE);
							produce(2, subchannel_size * CU_SIZE);
						}
					}

					// Write-Index zurücksetzen, damit der nachfolgende CIF-Block mit Index 0 startet
					m_write_index = 0;

					// Signalisieren, dass i + 1 Symbole eingelesen wurden
					consume_each(i + 1);
					return WORK_CALLED_PRODUCE;
				}
			}

			// Signalisieren, dass alle Symbole eingelesen wurden
			consume_each(ninput_items[0]);
			return WORK_CALLED_PRODUCE;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

