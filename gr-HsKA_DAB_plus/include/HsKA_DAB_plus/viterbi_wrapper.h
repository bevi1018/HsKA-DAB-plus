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

// Genauere Beschreibung in viterbi_wrapper_impl.h

#ifndef INCLUDED_HSKA_DAB_PLUS_VITERBI_WRAPPER_H
#define INCLUDED_HSKA_DAB_PLUS_VITERBI_WRAPPER_H

#include <HsKA_DAB_plus/api.h>
#include <gnuradio/block.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class HSKA_DAB_PLUS_API viterbi_wrapper : virtual public gr::block
		{
		public:
			// Typdefinition für Zeiger zu diesem Block
			typedef boost::shared_ptr<viterbi_wrapper> sptr;

			/**
			* Konstruktor
			* \param fsm_input_size Anzahl der Eingaenge in die Finite State Machine
			* \param fsm_output_size Anzahl der Ausgaenge in die Finite State Machine
			* \param fsm_table Vektor der Generatorsequenzen fuer die Finite State Machine
			* \param init_state Status der Finite State Machine beim Start des Viterbi-Algorithmus
			* \param final_state Status der Finite State Machne beim Ende des Viterbi-Algorithmus
			* \param dimension Dimension des Codes
			* \param table Tabelle mit den moeglichen Zustaenden
			*/
			static sptr make(int32_t fsm_input_size, int32_t fsm_output_size, const std::vector<int32_t> &fsm_table, int32_t init_state, int32_t final_state, int32_t dimension, const std::vector<float> &table);
		};
	} // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_VITERBI_WRAPPER_H */

