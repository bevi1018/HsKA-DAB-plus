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

#ifndef INCLUDED_HSKA_DAB_PLUS_DEPARTITIONING_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_DEPARTITIONING_IMPL_H

#include <HsKA_DAB_plus/Departitioning.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class Departitioning_impl : public Departitioning
		{
		private:
			int32_t m_vector_length;		///< Anzahl der OFDM-Untertraeger
			int32_t m_cif_length;			///< Laenge eine CIFs in Anzahl Werte
			int32_t m_fic_length;			///< Laenge eine FICs in Anzahl Werte

			int32_t m_symbol_counter;		///< Symbolzaehler
			int32_t m_write_index;			///< Schreibzaehler
			
			float *m_cif_buffer;			///< Speicher fuer den aktuellen CIF-Block
			float *m_fic_buffer;			///< Speicher fuer alle FIC-Bloecke
			
			int32_t m_remaining_fic_count;	///< Anzahl der FIC-Bloecke im FIC-Speicher
			
			int32_t m_debug_enable;			///< Debugswitch

			shared_database *m_database; ///< Datenbank-Objekt. Daraus wird die aktuell eingestellte Subchannel ID gelesen (DB_SELECTED_SUBCHANNEL_ID)

		public:
			/**
			 * Konstruktor fuer diesen Block
			 * \param databaseID ID der Datenbank, von der die eingestellte Subchannel ID gelesen werden kann (DB_SELECTED_SUBCHANNEL_ID)
			 * \param vector_length Anzahl der OFDM Untertraeger
			 * \param debug_enable 0 deaktiviert die Debugausgabe, sonst werden Debugdaten ausgegeben
 			 */
			Departitioning_impl(char *databaseID, int vector_length, int32_t debug_enable);
			
			/**
			 * Destruktor
			 */
			~Departitioning_impl();

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

#endif /* INCLUDED_HSKA_DAB_PLUS_DEPARTITIONING_IMPL_H */

