/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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
- Datum: 30.08.18
--------------------------------------------------
Informationen
- Block: DQPSK_Demodulation
- Beschreibung: DQPSK Demodulation findet durch die Multiplikation des komplex konjugierten Wertes mit einem OFDM Symbol Verzögerung statt. D.h. DQPSK über die zeitliche
Richtung, also Symbole nacheinander auf dem Unterträger und nicht in Frequenzrichtung. Ändert sich der Symbolanfang theta, so verschiebt sich die Phasebeziehung. Für die
korrekte Relation muss ein Symbol korrigiert werden, da ansonsten die Phase bei den äußeren Unterträgern den Eindeutigkeitsbereich überschreiten kann.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifndef INCLUDED_HSKA_DAB_PLUS_DQPSK_DEMODULATION_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_DQPSK_DEMODULATION_IMPL_H

#include <HsKA_DAB_plus/DQPSK_Demodulation.h>

#include <stdint.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class DQPSK_Demodulation_impl : public DQPSK_Demodulation
		{
		private:
			gr_complex *m_last_sample;	///< Array der Laenge vector_length zu der die Differenz gebildet wird
			
			int32_t m_vector_length;	///< Anzahl der Untertraeger
			int32_t m_fft_length;		///< Laenge der FFT
			
			int16_t m_last_theta;		///< Speichert den letzten Wert fuer Theta (Zeitliche Verschiebung, siehe Symbol Synchronisation)

		public:
			/**
			 * Konstruktor fuer diesen Block
			 * \param vector_length Anzahl der OFDM Untertraeger
			 * \param fft_length Laenge der FFT
			 */
			DQPSK_Demodulation_impl(int32_t vector_length, int32_t fft_length);
			
			/**
			 * Destruktor
			 */
			~DQPSK_Demodulation_impl();

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

#endif /* INCLUDED_HSKA_DAB_PLUS_DQPSK_DEMODULATION_IMPL_H */

