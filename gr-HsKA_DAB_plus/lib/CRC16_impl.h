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

#ifndef INCLUDED_HSKA_DAB_PLUS_CRC16_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_CRC16_IMPL_H

#include <HsKA_DAB_plus/CRC16.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class CRC16_impl : public CRC16
		{
		private:	
			uint16_t m_crc_table[256];	///< Tabelle fuer die CRC16-Berechnung
			
			int32_t m_polynom;		///< Polynom
			int32_t m_length;		///< Laenge ueber die die CRC16 berechnet wird
			int32_t m_debug_enable;		///< 0 deaktiviert die Debugausgabe

		public:
			/**
			 * Konstruktor
			 * \param polynom Das Generatorpolynom der CRC16
			 * \param length Die Laenge ueber die die CRC16 berechnet wird (inkl. der 2 Byte fuer die CRC16)
			 * \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			 */
			CRC16_impl(int32_t polynom, int32_t length, int32_t debug_enable);
			
			/**
			 * Destruktor
			 */
			~CRC16_impl();

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

#endif /* INCLUDED_HSKA_DAB_PLUS_CRC16_IMPL_H */

