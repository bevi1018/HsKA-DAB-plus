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

// Genauere Beschreibung in fib_decoder_impl.h

#ifndef INCLUDED_HSKA_DAB_PLUS_FIB_DECODER_H
#define INCLUDED_HSKA_DAB_PLUS_FIB_DECODER_H

#include <HsKA_DAB_plus/api.h>
#include <gnuradio/sync_block.h>

#include <stdint.h>

namespace gr 
{
	namespace HsKA_DAB_plus
	{
		class HSKA_DAB_PLUS_API fib_decoder : virtual public gr::sync_block
		{
		public:
			// Typdefinition fuer einen shared_ptr zu diesem Block
			typedef boost::shared_ptr<fib_decoder> sptr;

			/**
			* Konstruktor
			* \param databaseID ID der Datenbank, in die die dekodierten Daten geschrieben werden sollen.
			* \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			*/
			static sptr make(char *databaseID, int32_t debug_enable);
		};
	} // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_FIB_DECODER_H */

