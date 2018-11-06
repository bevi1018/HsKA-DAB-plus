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

// Genauere Beschreibung in OFDM_coarse_frequency_correction_impl.h

#ifndef INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_H
#define INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_H

#include <HsKA_DAB_plus/api.h>
#include <gnuradio/block.h>

#include <stdint.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class HSKA_DAB_PLUS_API OFDM_coarse_frequency_correction : virtual public gr::block
		{
		public:
			// Typdefinition fuer einen shared_ptr zu diesem Block
			typedef boost::shared_ptr<OFDM_coarse_frequency_correction> sptr;

			/**
			 * Konstruktor fuer diesen Block
			 * \param fft_length Laenge der FFT
			 * \param num_carriers Anzahl der OFDM Untertraeger
			 * \param cp_length Laenge des Cyclic-Prefix
			 * \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			 */
			static sptr make(int32_t fft_length, int32_t num_carriers, int32_t cp_length, int32_t debug_enable);
		};
	} // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_OFDM_COARSE_FREQUENCY_CORRECTION_H */

