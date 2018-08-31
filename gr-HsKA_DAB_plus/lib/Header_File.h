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
- Datum: 28.08.18
--------------------------------------------------
Informationen
- Bibliothek:global
- Hierarchieebene: Hauptebene
- Beschreibung: 
- Quellen:
*/

#ifndef HEADER_FILE
#define HEADER_FILE

using namespace std;
typedef unsigned char byte;

// Super Frame Synchronisation
#include "MovingSum.h"
#include "RingBuffer.h"

// OFDM Symbol Synchronisation
#include <gnuradio/expj.h>
#include <limits>
#include <boost/math/constants/constants.hpp>

#define PI boost::math::constants::pi<float>()
#define FLOAT_NEG_MAX -std::numeric_limits<float>::max()

// OFDM_coarse_frequency_correction
#include <gnuradio/expj.h>
#include <boost/math/constants/constants.hpp>

#define PI boost::math::constants::pi<float>()

// DQPSK_Demodulation
#include <gnuradio/expj.h>
#include <boost/math/constants/constants.hpp>

#define PI boost::math::constants::pi<float>()

#endif
