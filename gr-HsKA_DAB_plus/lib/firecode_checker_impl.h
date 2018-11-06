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

#ifndef INCLUDED_HSKA_DAB_PLUS_FIRECODE_CHECKER_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_FIRECODE_CHECKER_IMPL_H

#include <HsKA_DAB_plus/firecode_checker.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class firecode_checker_impl : public firecode_checker
		{
		private:
			int32_t m_debug_enable;				///< 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			int32_t m_byte_counter;				///< Nummer des aktuellen Bytes, das gerade eingelesen wurde (0 ist das erste Byte)
			uint16_t m_received_firecode;		///< Der empfangene Firecode im Audio-Superframe
			uint16_t m_firecode;				///< Der berechnete Firecode des Audio-Superframes
			bool m_check_passed;				///< Gibt an, ob der Firecode gueltig war
			uint32_t m_remaining_audio_frames;	///< Wenn der Firecode gueltig war, gibt dieser Wert an, wieviel Audioframes noch ausgegeben werden muessen
			
			uint16_t m_crc_table[256];			///< Tabelle der vorberechneten Firecode-Bytes
			uint8_t m_firecode_header[12];		///< Buffer fuer den Header des Audio-Superframes

		public:
			/**
			 * Konstruktor fuer diesen Block
			 * \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			 */
			firecode_checker_impl(int32_t debug_enable);
			
			/**
			 * Destruktor
			 */
			~firecode_checker_impl();

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

#endif /* INCLUDED_HSKA_DAB_PLUS_FIRECODE_CHECKER_IMPL_H */

