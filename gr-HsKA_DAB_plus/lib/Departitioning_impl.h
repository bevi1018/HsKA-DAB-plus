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

#ifndef INCLUDED_HSKA_DAB_PLUS_DEPARTITIONING_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_DEPARTITIONING_IMPL_H

#include <HsKA_DAB_plus/Departitioning.h>

namespace gr {
  namespace HsKA_DAB_plus {

	class Departitioning_impl : public Departitioning
	{
	private:
	int vector_length;
	int cif_length;
	int fic_length;

	int symbol_counter;
	int write_index;

	bool is_first_cif;
	float *block_buffer;

	float *fic_buffer;
	int remaining_fic_count;

	bool sync_received;

	gr::database_module::shared_database *m_database;

	public:
	Departitioning_impl(gr::database_module::shared_database *database, int vector_length);
	~Departitioning_impl();


	void forecast (int noutput_items, gr_vector_int &ninput_items_required);
	int general_work(int noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
	};

  } // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_DEPARTITIONING_IMPL_H */

