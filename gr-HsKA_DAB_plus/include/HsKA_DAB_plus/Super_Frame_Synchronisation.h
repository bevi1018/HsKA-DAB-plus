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

// Genauere Beschreibung: Siehe Super_Frame_Synchronisation_impl.h

#ifndef INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_H
#define INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_H

#include <HsKA_DAB_plus/api.h>
#include <gnuradio/block.h>

#include <stdint.h>

namespace gr 
{
	namespace HsKA_DAB_plus
	{
		class HSKA_DAB_PLUS_API Super_Frame_Synchronisation : virtual public gr::block
		{
		public:
			// Typdefinition fuer einen shared_ptr zu diesem Block
			typedef boost::shared_ptr<Super_Frame_Synchronisation> sptr;

			/**
			 * Konstruktor fuer diesen Block
			 * \param T_NULL Gibt die Laenge des Nullsymbols in Samples an.
			 * \param LOOK_AHEAD Gibt an, ueber wie viele Samples die Steigung der gemittelten Leistung berechnet wird.
			 * \param debug_enable 0 Deaktivierte Debugausgabe. Sonst Debugausgabe aktiv
			 */
			static sptr make(int32_t T_NULL, int32_t LOOK_AHEAD, int32_t debug_enable);
		};
	} // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_H */

