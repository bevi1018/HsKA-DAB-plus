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
- Datum: 02.09.18
--------------------------------------------------
Informationen
- Block: Soft_Demapper
- Beschreibung: Softbits und Mapping nach [1, S. 110]. Zuerst alle 1536 Realteile gefolgt von den Imaginärteilen.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifndef INCLUDED_HSKA_DAB_PLUS_SOFT_DEMAPPER_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_SOFT_DEMAPPER_IMPL_H

#include <HsKA_DAB_plus/Soft_Demapper.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class Soft_Demapper_impl : public Soft_Demapper
		{
		private:
			int32_t m_vector_length; ///< Anzahl der OFDM-Untertraeger

		public:
			/**
			 * Konstruktor
			 * \param vector_length Anzahl der OFDM-Untertraeger
			 */
			Soft_Demapper_impl(int32_t vector_length);
			
			/**
			 * Destruktor
			 */
			~Soft_Demapper_impl();

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

#endif /* INCLUDED_HSKA_DAB_PLUS_SOFT_DEMAPPER_IMPL_H */

