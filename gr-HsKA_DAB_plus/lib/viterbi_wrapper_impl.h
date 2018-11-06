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

#ifndef INCLUDED_HSKA_DAB_PLUS_VITERBI_WRAPPER_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_VITERBI_WRAPPER_IMPL_H

#include <HsKA_DAB_plus/viterbi_wrapper.h>
#include <gnuradio/trellis/fsm.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		/**
		 * \brief Wrapper fuer den in GnuRadio integrierten Viterbi-Block.
		 *
		 * \ingroup HsKA_DAB_plus
		 * \author Vivian Becher
		 * \author David Kohler
		 */
		class viterbi_wrapper_impl : public viterbi_wrapper
		{
		private:
			const gr::trellis::fsm			m_state_machine;		///< Finite State Machine
			int32_t							m_init_state;			///< Status der Finite State Machine beim Start des Viterbi-Algorithmus
			int32_t							m_final_state;			///< Status der Finite State Machne beim Ende des Viterbi-Algorithmus
			int32_t							m_dimension;			///< Dimension des Codes
			std::vector<float>				m_table;				///< Tabelle mit den moeglichen Zustaenden
			
			int32_t							m_max_block_size;		///< Maximale Blockgroesse (wird angepasst, sobald sich die Groesse am Eingang geaendert hat)
			int32_t							m_current_block_size;	///< Aktuelle Blockgroesse

		public:
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
			viterbi_wrapper_impl(int32_t fsm_input_size, int32_t fsm_output_size, const std::vector<int32_t> &fsm_table, int32_t init_state, int32_t final_state, int32_t dimension, const std::vector<float> &table);
			
			/**
			 * Destruktor
			 */
			~viterbi_wrapper_impl();

			/**
			* Funktion zur Vorhersage, wie viele Eingangsdaten am jeweiligen Port benoetigt werden, um noutput_items generieren zu koennen.
			* Wichtig fuer den Scheduler in GnuRadio.
			* \param noutput_items Gibt an, wieviele Ausgangsdaten erzeugt werden sollen.
			* \param ninput_items_required Gibt an, wieviele Eingangsdaten am jeweilgen Port benoetigt werden. Wird von der Funktion befuellt.
			*/
			void forecast (int32_t noutput_items, gr_vector_int &ninput_items_required);

			/**
			* Beinhaltet die eingentliche Funktionalitaet. Hier werden die Eingangsdaten verarbeitet und an den entsprechenden Ausgangsport ausgegeben.
			* Wird von Scheduler in GnuRadio aufgerufen.
			* \param noutput_items Gibt an, wieviele Ausgangsdaten erzeugt werden sollen.
			* \param ninput_items Gibt an, wieviele Eingangsdaten am jeweiligen Eingangsport zur Verfuegung stehen.
			* \param input_items Datenvektor fuer die Eingangsdaten
			* \param output_items Datenvektor fuer die Ausgangsdaten
			*/
			int32_t general_work(int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
		};
	} // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_VITERBI_WRAPPER_IMPL_H */

