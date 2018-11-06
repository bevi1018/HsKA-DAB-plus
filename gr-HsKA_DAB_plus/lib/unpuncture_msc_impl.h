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

#ifndef INCLUDED_HSKA_DAB_PLUS_UNPUNCTURE_MSC_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_UNPUNCTURE_MSC_IMPL_H

#include <HsKA_DAB_plus/unpuncture_msc.h>
#include <HsKA_DAB_plus/shared_database.h>

#include <stdint.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		/**
		 * \brief Fuehrt die Depunktierung des Main Service Channels durch.
		 *
		 * Fuehrt die Depunktierung des Main Service Channels durch. Es ist sowohl EEP als auch UEP implementiert worden. Die Auswahl
		 * des Verfahrens wird jedoch noch nicht von der Datenbank aus gesteuert.
		 *
		 * \ingroup HsKA_DAB_plus
		 * \author Vivian Becher
		 * \author David Kohler
		 */
		class unpuncture_msc_impl : public unpuncture_msc
		{
		private:
			const uint8_t *puncture[25];	///< Array mit den Punktierungsvektoren
			shared_database* m_database;	///< Datenbankinstanz, aus der die Punktierungsdaten gelesen werden können
			uint32_t m_debug_enable;		///< Debugswitch

		public:
			/**
			 * Konstruktor
			 * \param databaseID ID zur Database, aus der die Punktierungsdaten gelesen werden.
			 * \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			 */
			unpuncture_msc_impl(char *databaseID, uint32_t debug_enable);
			
			/**
			 * Destruktor
			 */
			~unpuncture_msc_impl();
			
			/**
			 * Führt die Depunktierung durch. Diese Funktion beschreibt nur den durch out_index referenzierten Wert.
			 * \param out_index Index, der im Array out beschrieben wird.
			 * \param in_index Index, von dem aus dem Array in gelesen wird. Je nach Wert im puncturing_vector wird der Index inkrementiert.
			 * \param puncturing_vector Punktierungsvektor.
			 * \param puncturing_vector_length Laenge des Punktierungsvektor.
			 * \param in Punktierte Eingangsdaten
			 * \param out Depunktierte Ausgangsdaten
			 */
			void unpuncture(int32_t out_index, int32_t *in_index, const uint8_t *puncturing_vector, int32_t puncturing_vector_length, const float *in, float *out);

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

#endif /* INCLUDED_HSKA_DAB_PLUS_UNPUNCTURE_MSC_IMPL_H */

