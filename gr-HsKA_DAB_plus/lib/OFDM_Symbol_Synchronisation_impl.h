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
- Datum: 30.08.18
--------------------------------------------------
Informationen
- Block: OFDM_Symbol_Synchronisation
- Beschreibung: Schätzung des Symbolanfangs über das Guard Interval mit der Auto-Korrelationsfunktion bei einem Zeitabstand von N. Suchen nach dem Maximum dieser Funktion.
Feine Phasenkorrektur auf den einzelnen Unterträgern und Phasenkorrektur aufgrund der Verwerfung von samples. Vorgehen ist in Quelle [2] beschrieben - die Parameter
für dieses System sind im Standard zu finden.
- Quellen: [1] ETSI Standard EN 300 401; [2] 'On Synchronisation in OFDM Systems using the Cyclic Prefix' by J. van de Beek, M. Sandell and P. Börjesson
*/

#ifndef INCLUDED_HSKA_DAB_PLUS_OFDM_SYMBOL_SYNCHRONISATION_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_OFDM_SYMBOL_SYNCHRONISATION_IMPL_H

#include <HsKA_DAB_plus/OFDM_Symbol_Synchronisation.h>

#include "HsKA_DAB_plus/MovingSum.h"

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class OFDM_Symbol_Synchronisation_impl : public OFDM_Symbol_Synchronisation
		{
		private:
			int32_t N;				///< Symboldauer in Samples (Ohne Guardintervall)
			int32_t L;				///< Dauer des Guardintervalls in Samples

			float rho;				///< Gemaeß Quelle [2] berechnetes Rho

			float epsilon_maximum_likelihood; ///< Geschaetztes Epsilon gemaeß Quelle [2]

			float phi;				///< Aktuelle Phasenkorrektur

			int32_t symbol_counter;	///< Aktuelle Symbolnummer
			
			int32_t debug_enable;	///< Debugswitch
			
			MovingSum<float> epsilon_moving_sum;				///< Summe der letzten L Werten von Epsilon
			MovingSum<gr_complex> gamma_moving_sum;				///< Summe der letzten L Werten von Gamma
			MovingSum<float> estimated_epsilon_moving_average;	///< Summe der Letzten average_count Werten des geschaetzten Epsilons

		public:
			/**
			 * Konstruktor fuer diesen Block
			 * \param N Symboldauer in Samples (Ohne Guardintervall)
			 * \param L Dauer des Guardintervalls in Samples
			 * \param SNR Ungefaehres SNR des Signals
			 * \param average_count Anzahl der Werte ueber die Epsilon und Theta gemittelt wird
			 * \param debug_enable 0 deaktiviert die Debugausgabe; Sonst ist die Debugausgabe aktiv
			 */
			OFDM_Symbol_Synchronisation_impl(int32_t N, int32_t L, float SNR, int32_t average_count, int32_t debug_enable);
			
			/**
			 * Destruktor des Blocks
			 */
			~OFDM_Symbol_Synchronisation_impl();

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

#endif /* INCLUDED_HSKA_DAB_PLUS_OFDM_SYMBOL_SYNCHRONISATION_IMPL_H */

