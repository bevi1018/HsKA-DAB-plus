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

/*
Hochschule Karlsruhe Technik u. Wirtschaft (HsKa)
Projektarbeit Master
- Projekt: DAB+ Empfänger mit Gnu Radio
- Studenten: David Kohler, Vivian Becher
- Professor: M. Litzenburger
- Datum: 11.09.18
--------------------------------------------------
Informationen
- Block: Unpuncture FIC
- Beschreibung: Dieser Block depunktiert den Fast Information Channel ([1], S.91).
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifndef INCLUDED_HSKA_DAB_PLUS_UNPUNCTURE_FIC_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_UNPUNCTURE_FIC_IMPL_H

#include <HsKA_DAB_plus/unpuncture_fic.h>

namespace gr
{
	namespace HsKA_DAB_plus 
	{
		class unpuncture_fic_impl : public unpuncture_fic
		{
		private:
			int32_t m_vector_length; ///< Anzahl der OFDM-Untertraeger

		public:
			/**
			 * Konstruktor fuer diesen Block
			 * \param vector_length Anzahl der OFDM-Untertraeger
			 */
			unpuncture_fic_impl(int32_t vector_length);
			
			/**
			 * Destruktor.
			 */
			~unpuncture_fic_impl();
			
			/**
			 * Führt die Depunktierung durch. Diese Funktion beschreibt nur den durch out_index referenzierten Wert.
			 * \param out_index Index, der im Array out beschrieben wird.
			 * \param in_index Index, von dem aus dem Array in gelesen wird. Je nach Wert im puncturing_vector wird der Index inkrementiert.
			 * \param puncturing_vector Punktierungsvektor.
			 * \param puncturing_vector_length Laenge des Punktierungsvektor.
			 * \param in Punktierte Eingangsdaten
			 * \param out Depunktierte Ausgangsdaten
			 */
			void unpuncture(int out_index, int *in_index, const uint8_t *puncturing_vector, int32_t puncturing_vector_length, const float *in, float *out);

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

#endif /* INCLUDED_HSKA_DAB_PLUS_UNPUNCTURE_FIC_IMPL_H */

