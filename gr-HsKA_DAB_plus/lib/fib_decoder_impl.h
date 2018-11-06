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

#ifndef INCLUDED_HSKA_DAB_PLUS_FIB_DECODER_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_FIB_DECODER_IMPL_H

#include <HsKA_DAB_plus/fib_decoder.h>
#include <HsKA_DAB_plus/shared_database.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		/**
		 * Klasse fuer die Verarbeitung von Fast Information Blocks (FIB).
		 * \author Vivian Becher
		 * \author David Kohler
		**/
		class fib_decoder_impl : public fib_decoder
		{
		private:
			shared_database *m_database;	///< Datenbank in die die dekodierten Daten geschrieben werden
			int32_t m_debug_enable;			///< Debugswitch

		public:
			/**
			* Konstruktor
			* \param databaseID ID der Datenbank, in die die dekodierten Daten geschrieben werden sollen.
			* \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			*/
			fib_decoder_impl(char *databaseID, int32_t debug_enable);
			
			/**
			* Destruktor
			*/
			~fib_decoder_impl();
			
			/**
			 * Verarbeitet einen kompletten FIB (Fast Information Block) bestehend aus 32 Bytes.
			 * \param in Zeiger auf einen 32 Byte Buffer, der den FIB enthaelt.
			**/
			void process_fib(const uint8_t *in);
			
			/**
			 * Verarbeitet eine FIG (Fast Information Group) von Typ 0.
			 * \param fig_buffer Zeiger auf den FIG-Buffer
			 * \param length Laenge des FIG-Buffers
			**/
			void process_mci(const uint8_t *fig_buffer, int length);
			
			/**
			 * Verarbeitet ein Label von FIG-Typ 1.
			 * \param fig_buffer Zeiger auf den FIG-Buffer
			 * \param length Laenge des FIG-Buffers
			**/
			void process_label1(const uint8_t *fig_buffer, int length);
			
			/**
			 * Verarbeitet ein Label von FIG-Typ 2.
			 * \param fig_buffer Zeiger auf den FIG-Buffer
			 * \param length Laenge des FIG-Buffers
			**/
			void process_label2(const uint8_t *fig_buffer, int length);
			
			/**
			 * Verarbeitet einen FIG mit Conditional Access
			 * \param fig_buffer Zeiger auf den FIG-Buffer
			 * \param length Laenge des FIG-Buffers
			**/
			void process_ca(const uint8_t *fig_buffer, int length);

			/**
			  * \param noutput_items Anzahl der Werte die ausgegben werden sollen (da Sync-Block ist das gleichbedeutend mit der Anzahl der Werte am Eingang)
			  * \param input_items Werte am Eingang
			  * \param output_items Ausgangswerte (da dieser Block ein Sink ist, werden keine Daten ausgegeben)
			  */
			int32_t work(int32_t noutput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items);
		};
	} // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_FIB_DECODER_IMPL_H */

