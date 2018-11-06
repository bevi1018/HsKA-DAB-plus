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

#ifndef INCLUDED_HSKA_DAB_PLUS_MP4_DECODER_IMPL_H
#define INCLUDED_HSKA_DAB_PLUS_MP4_DECODER_IMPL_H

#include <HsKA_DAB_plus/MP4_decoder.h>
#include <neaacdec.h>

namespace gr
{
	namespace HsKA_DAB_plus 
	{
		/**
		 * Klasse zur Dekodierung von Audio-Frames
		 * \author Vivian Becher
		 * \author David Kohler
		**/
		class MP4_decoder_impl : public MP4_decoder
		{
		 private:
		 	int32_t m_debug_enable;			///< Debugswitch
			int32_t m_block_count;			///< Zaehler fuer die Anzahl der Bloecke im Frame
			
			uint16_t m_crc_table[256];		///< CRC16 Tabelle zur schnelleren CRC Berechnung
			
			NeAACDecHandle m_AAC_handle;	///< Handle fuer den AAC Decoder

		 public:
		 	/**
			 * Konstruktor
			 * \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			**/
			MP4_decoder_impl(int32_t debug_enable);
			
			/**
			 * Destruktor
			**/
			~MP4_decoder_impl();
			
			/**
			 * Konfiguriert den Dekoder fuer das naechste Audio-Frame
			 * \param aac_sample_freq Samplefrequenz des Audio-Frames
			 * \param mpeg_surround_config Gibt die Surround-Konfiguration an.
			 * \param is_stereo True, wenn das Audio-Frame ein Stereosignal enthaelt
			 * \return True, wenn die Konfiguration des Dekoders erfoglreich war
			**/
			bool config_decoder(int32_t aac_sample_freq, int16_t mpeg_surround_config, bool is_stereo);
			
			/**
			 * Verarbeitet ein komplettes Audio-Frame
			 * \param in Daten des Audio-Frames
			 * \param superframe_size Groesse des Audio-Frames
			 * \param right Samples fuer den rechten Audio-Kanal
			 * \param left Samples fuer den linken Audio-Kanal
			 * \return Anzahl der ausgegebenen Samples
			**/
			int32_t process_frame(const uint8_t *in, int32_t superframe_size, float *right, float *left);
			
			/**
			 * Verarbeitet eine einzelne Audio-Unit
			 * \param au Daten des Audio-Units
			 * \param au_size Groesse des Audio-Units
			 * \param right Samples fuer den rechten Audio-Kanal
			 * \param left Samples fuer den linken Audio-Kanal
			 * \param aac_sample_freq Samplefrequenz der Audio-Unit
			 * \param mpeg_surround_config Gibt die Surround-Konfiguration an.
			 * \param is_stereo True, wenn die Audio-Unit ein Stereosignal enthaelt
			 * \return Anzahl der ausgegebenen Samples
			**/
			int32_t process_audio_unit(const uint8_t *au, int32_t au_size, float *right, float *left, int32_t aac_sample_freq, int16_t mpeg_surround_config, bool is_stereo);
			
			/**
			 * Berechnet die CRC16 von einem Datenblock
			 * \param data Daten, ueber die die CRC16 berechnet wird
			 * \param length Laenge des Datenblocks
			**/
			bool check_CRC16(const uint8_t *data, int16_t length);

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

#endif /* INCLUDED_HSKA_DAB_PLUS_MP4_DECODER_IMPL_H */

