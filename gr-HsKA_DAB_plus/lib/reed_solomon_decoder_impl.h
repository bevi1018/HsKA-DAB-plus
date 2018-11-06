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

#ifndef INCLUDED_HSKA_DAB_PLUS_REED_SOLOMON_DECODER_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_REED_SOLOMON_DECODER_IMPL_H

#include <HsKA_DAB_plus/reed_solomon_decoder.h>

namespace gr
{
	namespace HsKA_DAB_plus 
	{
		class reed_solomon_decoder_impl : public reed_solomon_decoder
		{
		private:
			int32_t	m_debug_enable;				///< Debugswitch
			void *	m_rs_handle;				///< handle auf die Implementierung des RS-Codes von Phil Karn (KA9Q)
			
			int32_t	m_correction_positions[10];	///< Fehlerstellen-Array
			uint8_t m_rs_packet[120];			///< Speicher für das Reed Solomon Packet
			
			int32_t m_block_count;				/// < Anzahl der 120 Byte Bloecke

		public:
			/**
			 * Konstruktor fuer diesen Block
			 * \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			 */
			reed_solomon_decoder_impl(int32_t debug_enable);
			
			/**
			 * Destruktor
			 */
			~reed_solomon_decoder_impl();

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

#endif /* INCLUDED_HSKA_DAB_PLUS_REED_SOLOMON_DECODER_IMPL_H */

