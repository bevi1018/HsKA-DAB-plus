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
- Datum: 28.08.18
--------------------------------------------------
Informationen
- Block: Super_Frame_Synchronisation
- Beschreibung: Schätzt die Rahmenstruktur von DAB+ Signalen, siehe [1; S.23]. Da der Empfänger asychron eingeschaltet wird, muss zunächst das Null Symbol
  gefunden werden. Dieses enthält keine oder nur geringfügig Sendeleistung. Der gleitende Mittelwert der Momentanleistung über die Null Symbol Dauer hat 
  bei exakter Überlappung dort ein Minimum. Der Übergang von fallenden Flanke zu steigender Flanke wird mit einem Vergleich der Leistung mit einer kurzen 
  zeitlichen Differenz detektiert.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifndef INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_IMPL_H

#include <HsKA_DAB_plus/Super_Frame_Synchronisation.h>

#include "HsKA_DAB_plus/MovingSum.h"
#include "HsKA_DAB_plus/RingBuffer.h"

namespace gr
{
	namespace HsKA_DAB_plus
	{
		class Super_Frame_Synchronisation_impl : public Super_Frame_Synchronisation
		{
		private:
			int32_t T_NULL;								///< Gibt die Laenge des Nullsymbols in Samples an [1; S. 104]
			int32_t LOOK_AHEAD;							///< Gibt an, ueber wie viele Samples die Steigung der gemittelten Leistung berechnet wird.
			
			int32_t negative_counter;					///< Zaehlt, seit wievielen Samples die gemittelte Leistung faellt
			int32_t super_frame_counter;				///< Zaehlt die Anzahl der erkannten Superframes
			
			int32_t debug_enable;						///< 0 Deaktivierte Debugausgabe. Sonst Debugausgabe aktiv

			MovingSum<float>		power;			///< Damit kann die Summe ueber die letzten N Werte gebildet werden.
			RingBuffer<gr_complex>	data_buffer;	///< Ringpuffer fuer die gesampleten Daten
			RingBuffer<float>		power_buffer;	///< Ringpuffer fuer die Leistungen der Samples
		
		public:
			/**
			 * Konstruktor fuer diesen Block
			 * \param T_NULL Gibt die Laenge des Nullsymbols in Samples an.
			 * \param LOOK_AHEAD Gibt an, ueber wie viele Samples die Steigung der gemittelten Leistung berechnet wird.
			 * \param debug_enable 0 Deaktivierte Debugausgabe. Sonst Debugausgabe aktiv
			 */
			Super_Frame_Synchronisation_impl(int32_t T_NULL, int32_t LOOK_AHEAD, int32_t debug_enable);
			
			/**
			 * Destruktor fuer diesen Block
			 */
			~Super_Frame_Synchronisation_impl();

			/**
			 * Funktion zur Vorhersage, wie viele Eingangsdaten am jeweiligen Port benoetigt werden, um noutput_items generieren zu koennen.
			 * Wichtig fuer den Scheduler in GnuRadio.
			 * \param noutput_items Gibt an, wieviele Ausgangsdaten erzeugt werden sollen.
			 * \param ninput_items_required Gibt an, wieviele Eingangsdaten am jeweilgen Port benoetigt werden. Wird von der Funktion befuellt.
			 */
			void forecast(int32_t noutput_items, gr_vector_int &ninput_items_required);
			
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

#endif /* INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_IMPL_H */

