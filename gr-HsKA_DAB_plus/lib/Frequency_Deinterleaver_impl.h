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
- Datum: 30.08.18
--------------------------------------------------
Informationen
- Block: Frequency_Deinterleaver
- Beschreibung: Vertauschen der Symbole über die Frequenzrichtung. Position des Unterträgers wird nach dem Muster in [1, S.110] neu geordnet.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifndef INCLUDED_HSKA_DAB_PLUS_FREQUENCY_DEINTERLEAVER_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_FREQUENCY_DEINTERLEAVER_IMPL_H

#include <HsKA_DAB_plus/Frequency_Deinterleaver.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class Frequency_Deinterleaver_impl : public Frequency_Deinterleaver
		{
		private:
			int32_t m_fft_length;		///< Anzahl der OFDM-Untertraeger
			int32_t m_vector_length;	///< Laenge der FFT

		public:
			/**
			 * Konstruktor
			 * \param vector_length Anzahl der OFDM-Untertraeger
			 * \param fft_length Laenge der FFT
			 */
			Frequency_Deinterleaver_impl(int32_t vector_length, int32_t fft_length);
			
			/**
			 * Destruktor
			 */
			~Frequency_Deinterleaver_impl();

			/**
			 * Verarbeitet nur einen einzigen Datenvektor
			 * \param in Eingangsvektor
			 * \param out Ausgangsvektor
			 */
			void process_single_vector(gr_complex const *in, gr_complex *out);

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

#endif /* INCLUDED_HSKA_DAB_PLUS_FREQUENCY_DEINTERLEAVER_IMPL_H */

