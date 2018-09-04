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
- Beschreibung: 
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include "Header_File.h"

#include <gnuradio/io_signature.h>
#include "Departitioning_impl.h"

namespace gr {
  namespace HsKA_DAB_plus {

	Departitioning::sptr
	Departitioning::make(gr::database_module::shared_database *database, int vector_length)
	{
	return gnuradio::get_initial_sptr(new Departitioning_impl(database, vector_length));
	}

	Departitioning_impl::Departitioning_impl(gr::database_module::shared_database *database, int vector_length)
	: gr::block("Departitioning", gr::io_signature::make2(2, 2, 2*vector_length*sizeof(float), sizeof(byte)), gr::io_signature::makev(4, 4, boost::assign::list_of(vector_length*2*3/4*sizeof(float))(sizeof(byte))(sizeof(float))(sizeof(byte)))),
	  m_database(database)
	{
		this->vector_length = vector_length;

		write_index = 0;
		remaining_fic_count = 0;
		symbol_counter = 0;

		sync_received = false;
		is_first_cif = false;

		cif_length = vector_length * 2 * 72 / 4; // 18 mal Symbollänge
		fic_length = vector_length * 2 * 3 / 4;

		block_buffer = new float[cif_length];
		fic_buffer = new float[3 * fic_length]; // Einer der 4 FIC-Blöcke kann direkt ausgegeben werden

		set_output_multiple(128 * CU_SIZE);
	}

	Departitioning_impl::~Departitioning_impl()
	{
		if(block_buffer)
		{
			delete [] block_buffer;
			block_buffer = 0;
		}

		if(fic_buffer)
		{
			delete [] fic_buffer;
			fic_buffer = 0;
		}
	}

	void
	Departitioning_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
	{
		for(int i = 0; i < ninput_items_required.size(); ++i)
		{
			ninput_items_required[i] = 1;
		}
	}

	int
	Departitioning_impl::general_work (int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
	{
	const float *in = (const float *) input_items[0];
	const byte *sync_in = (const byte *) input_items[1];

	float *fic_block_out = (float *) output_items[0];
	byte *fic_block_sync = (byte *) output_items[1];
	float *subch_out = (float *) output_items[2];
	byte *subch_sync = (byte *) output_items[3];
			
	// Müssen noch gespeicherte FIC-Blöcke ausgegeben werden?
	if(remaining_fic_count > 0)
	{
		int start_index = (3 - remaining_fic_count) * fic_length;

		// 1 FIC-Block kopieren (da hier nur die Blöcke 2-4 ausgegeben werden ist sync immer 0)
		memcpy(fic_block_out, &fic_buffer[start_index], fic_length * sizeof(float));
		fic_block_sync[0] = 0;

		remaining_fic_count--;

		printf("FIC Block (%d)\n", (4 -remaining_fic_count));

		// Signalisieren, dass 1 FIC-Block ausgegeben wurde.
		produce(0, 1);
		produce(1, 1);
		return WORK_CALLED_PRODUCE;
	}

	// Ansonsten neue Daten verarbeiten
	for(int i = 0; i < ninput_items[0]; ++i)
	{
		if(sync_in[i] != 0)
		{
			symbol_counter = 1;
			write_index = 0;
			remaining_fic_count = 0;
			sync_received = true;
			//printf("Sync received (%d)\n", (int)(sync_in[i]));
		}
		else if(!sync_received)
			continue;
		else
		{
			symbol_counter++;
		}

		// 1 Symbol in den Block-Buffer kopieren
		memcpy(&block_buffer[write_index*vector_length*2], &in[i*vector_length*2], vector_length*2*sizeof(float));
		write_index++;

		//printf("write_index: %d\n", write_index);
		//printf("symbol_counter: %d\n", symbol_counter);

		// Wenn die ersten 3 Symbole empfangen wurden -> FIC partitionieren
		if(symbol_counter == 3)
		{
			// Die Blöcke 2-4 zwischenspeichern
			memcpy(fic_buffer, &block_buffer[fic_length], 3 * fic_length * sizeof(float));
			remaining_fic_count = 3;

			// 1. FIC-Block kopieren (Sync ist 1)
			memcpy(fic_block_out, block_buffer, fic_length * sizeof(float));
			fic_block_sync[0] = 1;

			// Write-Index zurücksetzen, damit der nachfolgende CIF-Block mit Index 0 startet
			write_index = 0;

			// Der Nach den FIC-Blöcken folgende CIF-Block ist der erste.
			is_first_cif = true;

			// Signalisieren, dass 1 FIC-Block ausgegeben wurde.
			produce(0, 1);
			produce(1, 1);

			printf("FIC Block (1)\n");

			// Signalisieren, dass i + 1 Symbole eingelesen wurden
			consume(0, i + 1);
			consume(1, i + 1);
			return WORK_CALLED_PRODUCE;
		}

		// Wenn der Block-Buffer komplett gefüllt ist, kann ein Subchannel ausgegeben werden (symbol_counter ist >= 21)
		if(write_index >= 18)
		{
			uint16_t start_address = 0;
			uint16_t subchannel_size = 0;
			uint32_t current_subchannel_id = 0;

			SubChannelOrga subch_orga;
			if(m_database->check_and_get_variable<uint32_t>(DB_SELECTED_SUBCHANNEL_ID, &current_subchannel_id))
			{
				printf("Current Subchannel ID: %d\n", current_subchannel_id);
				if(m_database->check_and_get_variable<SubChannelOrga>(DB_SUBCHANNEL_ORGA_ID(current_subchannel_id), &subch_orga))
				{
					start_address = subch_orga.start_address;
					subchannel_size = subch_orga.sub_channel_size;

					// Den jeweiligen Subchannel des CIF-Blocks in des Ausgangsbuffer schreiben
					memcpy(subch_out, &block_buffer[start_address * CU_SIZE], subchannel_size * CU_SIZE * sizeof(float));

					// Wenn dies der erste CIF-Block ist -> sync = 1
					memset(subch_sync, 0, subchannel_size * CU_SIZE * sizeof(byte));
					if(is_first_cif)
						subch_sync[0] = 1;
					else
						subch_sync[0] = 0;
					is_first_cif = false;

					printf("Subchannel from CIF Block %d written.\n", (symbol_counter - 3) / 18);

					// Signalisieren, dass 1 CIF-Block ausgegeben wurde.
					produce(2, subchannel_size * CU_SIZE);
					produce(3, subchannel_size * CU_SIZE);
				}
			}

			// Write-Index zurücksetzen, damit der nachfolgende CIF-Block mit Index 0 startet
			write_index = 0;

			// Signalisieren, dass i + 1 Symbole eingelesen wurden
			consume(0, i + 1);
			consume(1, i + 1);
			return WORK_CALLED_PRODUCE;
		}
	}

	// Signalisieren, dass alle Symbole eingelesen wurden
	consume(0, ninput_items[0]);
	consume(1, ninput_items[0]);
	return 0;
	}

  } /* namespace HsKA_DAB_plus */
} /* namespace gr */

