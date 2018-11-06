/* -*- c++ -*- */
/* 
 * Copyright 2018 KIT
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
Dieser Block wurde von der KIT Implementierung mit kleineren Anpassungen übernommen.
Hochschule Karlsruhe Technik u. Wirtschaft (HsKa)
Projektarbeit Master
- Projekt: DAB+ Empfänger mit Gnu Radio
- Studenten: David Kohler, Vivian Becher
- Professor: M. Litzenburger
- Datum: 30.08.18
--------------------------------------------------
Informationen
- Block: OFDM_coarse_frequency_correction
- Beschreibung: Schätzt grob den Frequenzfehler durch Vergleich der Leistungen der positiven und negativen Frequenzachse. Der Frequenzfehler wird dann grob korrigiert.
*/

#ifndef INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_IMPL_H

#include <HsKA_DAB_plus/OFDM_coarse_frequency_correction.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class OFDM_coarse_frequency_correction_impl : public OFDM_coarse_frequency_correction
		{
		private:
			int32_t m_fft_length;	//< Laenge der FFT
			int32_t m_num_carriers;	///< Anzahl der OFDM Untertraeger
			int32_t m_cp_length;	///< Laenge des Cyclic Prefix
			int32_t m_symbol_num;	///< aktuelle Symbolnummer
			int32_t m_freq_offset;	///< Geschaetzter Frequenzoffset als Anzahl der OFDM Untertraeger (absolut mit Nulltraeger)
			int32_t m_delta_f;		///< Geschaetzter Frequenzoffset als Anzahl der OFDM Untertraeger relativ zum idealen Offset
			int32_t m_debug_enable;	///< Debugswitch

			/**
			 * Schaetzt den Frequenzoffset durch Vergleich der Leistungen der positiven und negativen Frequenzachse.
			 * Aktualisiert m_freq_offset.
			 * \param symbol OFDM-Symbol zu dem der Frequenzoffset geschaetzt werden soll.
			 */
			void correlate_energy(const gr_complex *symbol);

		public:
			/**
			 * Konstruktor fuer diesen Block
			 * \param fft_length Laenge der FFT
			 * \param num_carriers Anzahl der OFDM Untertraeger
			 * \param cp_length Laenge des Cyclic-Prefix
			 * \param debug_enable 0 deaktiviert Debugausgabe, sonst ist die Debugausgabe aktiv
			 */
			OFDM_coarse_frequency_correction_impl(int32_t fft_length, int32_t num_carriers, int32_t cp_length, int32_t debug_enable);
			
			/**
			 * Destruktor
			 */
			~OFDM_coarse_frequency_correction_impl();

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

#endif /* INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_IMPL_H */

