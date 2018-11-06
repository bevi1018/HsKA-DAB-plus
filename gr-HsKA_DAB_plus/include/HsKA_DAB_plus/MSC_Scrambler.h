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

// Genauere Beschreibung in MSC_Scrambler_impl.h


#ifndef INCLUDED_HSKA_DAB_PLUS_MSC_SCRAMBLER_H
#define INCLUDED_HSKA_DAB_PLUS_MSC_SCRAMBLER_H

#include <HsKA_DAB_plus/api.h>
#include <gnuradio/block.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		class HSKA_DAB_PLUS_API MSC_Scrambler : virtual public gr::block
		{
		public:
			// Typdefinition fuer eine Zeiger auf diesen Block
			typedef boost::shared_ptr<MSC_Scrambler> sptr;

			/**
			 * Konstruktor. Die Laenge des Vektors wird per Eingang mit uebergeben.
			 */
			static sptr make();
		};
	} // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_MSC_SCRAMBLER_H */

