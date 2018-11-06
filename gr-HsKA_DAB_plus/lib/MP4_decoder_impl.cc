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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "MP4_decoder_impl.h"

// Definitionen nach ISO 14496-3
#define AAC_LOW_COMPLEXITY 0x02

#define AAC_SAMPLE_FREQ_48kHz 3
#define AAC_SAMPLE_FREQ_32kHz 5
#define AAC_SAMPLE_FREQ_24kHz 6
#define AAC_SAMPLE_FREQ_16kHz 8

#define AAC_GENERAL_AUDIO_960_TRANSFORM 4

#define CHANNEL_CONFIG_1CH_MONO 1
#define CHANNEL_CONFIG_2CH_STEREO 2
#define CHANNEL_CONFIG_3CH_3STEREO 3
#define CHANNEL_CONFIG_4CH_QUAD 4
#define CHANNEL_CONFIG_5CH_PRO_LOGIC_I 5
#define CHANNEL_CONFIG_6CH_5_1 6
#define CHANNEL_CONFIG_8CH_7_1 7

// Syntactic Element IDs
#define AAC_SYN_ELEM_ID_SINGLE_CHANNEL 0
#define AAC_SYN_ELEM_ID_CHANNEL_PAIR 1
#define AAC_SYN_ELEM_ID_COUPLING_CHANNEL 2
#define AAC_SYN_ELEM_ID_LFE_CHANNEL 3
#define AAC_SYN_ELEM_ID_DATA_STREAM 4
#define AAC_SYN_ELEM_ID_PROGRAM_CONFIG 5
#define AAC_SYN_ELEM_ID_FILL 7

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		MP4_decoder::sptr MP4_decoder::make(int32_t debug_enable)
		{
			// Neue Instanz erzeugen und an GnuRadio weitergeben
			return gnuradio::get_initial_sptr(new MP4_decoder_impl(debug_enable));
		}

		/*
		* The private constructor
		*/
		MP4_decoder_impl::MP4_decoder_impl(int32_t debug_enable)
			: gr::block("MP4_decoder",
						gr::io_signature::make(2, 2, sizeof(uint8_t)),
						gr::io_signature::make(2, 2, sizeof(float))),
			  m_debug_enable(debug_enable),
			  m_block_count(1)
		{
			// CRC16/FALSE-CCITT Tabelle berechnen, damit die CRC byteweise berechnet werden kann
			for(int32_t i = 0; i < 256; ++i)
			{
				uint16_t crc = i << 8;
				
				for(int32_t bit = 0; bit < 8; ++bit)
				{
					if(crc & 0x8000)
						crc = (crc << 1) ^ 0x1021;
					else
						crc = (crc << 1);
				}
				
				m_crc_table[i] = crc;
			}	  
				  
			m_AAC_handle = NeAACDecOpen();
		}

		/*
		* Our virtual destructor.
		*/
		MP4_decoder_impl::~MP4_decoder_impl()
		{
			if(m_AAC_handle)
			{
				NeAACDecClose(m_AAC_handle);
				m_AAC_handle = nullptr;
			}
		}

		void MP4_decoder_impl::forecast (int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Daten ausgegeben, wie eingelesen werden
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;
			}
		}

		int32_t MP4_decoder_impl::general_work (int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			const uint8_t *in		= (const uint8_t *) input_items[0];
			const uint8_t *sync_in	= (const uint8_t *) input_items[1];
			float *right			= (float *) output_items[0];
			float *left				= (float *) output_items[1];
			
			int32_t superframe_size = 0;
			
			// Re-sync, damit in 120 Byte Blöcken gearbeitet werden kann
			if(sync_in[0] == 0)
			{
				uint32_t discarded_count = 1;
				
				while(discarded_count < ninput_items[0])
				{
					if(sync_in[discarded_count] != 0)
						break;
					
					++discarded_count;
				}
				
				if(m_debug_enable != 0)
					printf("MP4 Decoder: Resyncing! Discarded: %d\n", discarded_count);
				
				// Es wird mindestens ein Block verarbeitet
				m_block_count = 1;
				
				consume_each(discarded_count);
				return 0;
			}
			
			// Den nächsten Sync suchen, damit die Audio-Superframe Länge bestimmt werden kann
			bool sync_found = false;
			while((m_block_count * 110) < ninput_items[0])
			{				
				for(int32_t i = 0; i < 110; ++i)
				{
					if(sync_in[m_block_count * 110 + i] != 0)
					{
						// Superframe Größe speichern
						superframe_size = m_block_count * 110 + i;
						
						if(m_debug_enable != 0)
							printf("MP4 Decoder: Sync found: %d\n", m_block_count * 110 + i);
						
						sync_found = true;
						break;
					}
				}
				
				if(sync_found)
					break;
				
				++m_block_count;
			}	
			
			if(m_debug_enable != 0)
				printf("MP4 Decoder: Block Count: %d\n", m_block_count);
			
			// Wenn kein Sync gefunden wurde, die Größe der Ein- und Ausgangspuffer ändern
			if(!sync_found)
			{
				set_output_multiple((m_block_count + 1) * 110 * sizeof(uint8_t));
				return 0;
			}
			
			// Komplettes Frame verarbeiten
			int32_t output_count = process_frame(in, superframe_size, right, left);
			
			// Die Größe der Ein- und Ausgangspuffer auf aktuelle Frame-Länge setzen
			set_output_multiple(m_block_count * 110 * sizeof(uint8_t));

			int32_t temp_block_count = m_block_count;
			
			// Es wird immer mindestens ein Block verarbeitet
			m_block_count = 1;
			
			// GnuRadio mitteilen, wieviele Bytes eingelesen und ausgegeben werden.
			consume_each (temp_block_count * 110);
			return output_count;
		}
		
		int32_t MP4_decoder_impl::process_frame(const uint8_t *in, int32_t superframe_size, float *right, float *left)
		{
			uint16_t firecode		= (in[0] << 8) | in[1];				// Firecode
			int32_t dac_rate		= (in[2] & 0x40) ? 48000 : 32000;	// Samplerate des DAC
			bool aac_half_rate		= (in[2] & 0x20) ? true : false;	// True, wenn AAC die halbe DAC Sampling-Rate hat
			bool is_stereo			= (in[2] & 0x10) ? true : false;	// True, wenn Stereo-Signal im AU
			bool param_stereo		= (in[2] & 0x08) ? true : false;	// True, wenn Parametrisches Stereo verwendet wurde
			int32_t surround_config	= (in[2] & 0x07);					// MPEG Surround-Config
			
			int32_t au_count = 0;
			uint32_t au_start[7];
			
			int32_t aac_sample_freq = 0;
			
			// AU Startadressen extrahieren (Array aus 12-Bit Integer)
			if(aac_half_rate)
			{
				if(dac_rate == 32000)
				{
					// AAC Core Samplerate ist 16 kHz
					aac_sample_freq = AAC_SAMPLE_FREQ_16kHz;
					
					au_count = 2;
					au_start[0] = 5;
					au_start[1] = ((in[3] & 0xFF) << 4) | ((in[4] & 0xF0) >> 4);
					au_start[2] = superframe_size;
				}
				else
				{
					// AAC Core Samplerate ist 24 kHz
					aac_sample_freq = AAC_SAMPLE_FREQ_24kHz;
					
					au_count = 3;
					au_start[0] = 6;
					au_start[1] = ((in[3] & 0xFF) << 4) | ((in[4] & 0xF0) >> 4);
					au_start[2] = ((in[4] & 0x0F) << 8) | ((in[5] & 0xFF) >> 0);
					au_start[3] = superframe_size;
				}
			}
			else
			{
				if(dac_rate == 32000)
				{
					// AAC Core Samplerate ist 32 kHz
					aac_sample_freq = AAC_SAMPLE_FREQ_32kHz;
					
					au_count = 4;
					au_start[0] = 8;
					au_start[1] = ((in[3] & 0xFF) << 4) | ((in[4] & 0xF0) >> 4);
					au_start[2] = ((in[4] & 0x0F) << 8) | ((in[5] & 0xFF) >> 0);
					au_start[3] = ((in[6] & 0xFF) << 4) | ((in[7] & 0xF0) >> 4);
					au_start[4] = superframe_size;
				}
				else
				{
					// AAC Core Samplerate ist 48 kHz
					aac_sample_freq = AAC_SAMPLE_FREQ_48kHz;
					
					au_count = 6;
					au_start[0] = 11;
					au_start[1] = ((in[3] & 0xFF) << 4) | ((in[4] & 0xF0) >> 4);
					au_start[2] = ((in[4] & 0x0F) << 8) | ((in[5] & 0xFF) >> 0);
					au_start[3] = ((in[6] & 0xFF) << 4) | ((in[7] & 0xF0) >> 4);
					au_start[4] = ((in[7] & 0x0F) << 8) | ((in[8] & 0xFF) >> 0);
					au_start[5] = ((in[9] & 0xFF) << 4) | ((in[10] & 0xF0) >> 4);
					au_start[6] = superframe_size;
				}
			}	
			
			int32_t output_size = 0;
			
			// Alle AUs verarbeiten
			for(int32_t i = 0; i < au_count; ++i)
			{
				int32_t au_size = au_start[i + 1] - au_start[i];
				int32_t size = process_audio_unit(&in[au_start[i]], au_size, right, left, aac_sample_freq, surround_config, is_stereo);
				
				output_size += size;
				right += size;
				left += size;
		  	}
			
			return output_size;
		}
		
		bool MP4_decoder_impl::config_decoder(int32_t aac_sample_freq, int16_t mpeg_surround_config, bool is_stereo)
		{
			// Channel Konfiguration erstellen
			uint32_t channel_config = 0;
			switch (mpeg_surround_config) 
			{
			default:
			case 0:     // no surround
				channel_config = is_stereo ? CHANNEL_CONFIG_2CH_STEREO : CHANNEL_CONFIG_1CH_MONO;
				break;
			case 1:     // 5.1
				channel_config = CHANNEL_CONFIG_6CH_5_1;
				break;
			case 2:     // 7.1
				channel_config = CHANNEL_CONFIG_8CH_7_1;
				break;
			}
			
			// AudioSpecificConfig-Struktur erzeugen
			uint16_t audio_specific_config = (AAC_LOW_COMPLEXITY << 11) | (aac_sample_freq << 7) | (channel_config << 3) | AAC_GENERAL_AUDIO_960_TRANSFORM;
			audio_specific_config = (audio_specific_config << 8) | (audio_specific_config >> 8);
				
			// Dekoder konfigurieren
			uint64_t sample_rate = 0;
			uint8_t channel_count = 0;
			if(NeAACDecInit2(m_AAC_handle, (uint8_t *)&audio_specific_config, sizeof(uint16_t), &sample_rate, &channel_count) < 0)
			{
				if(m_debug_enable != 0)
					printf("MP4 Decoder: Decoder-Konfiguration schlug fehl!\n");
				return false;
			}
			
			return true;
		}
		
		int32_t MP4_decoder_impl::process_audio_unit(const uint8_t *au, int32_t au_size, float *right, float *left, int32_t aac_sample_freq, int16_t mpeg_surround_config, bool is_stereo)
		{
			// CRC16 überprüfen
			if(check_CRC16(au, au_size))
			{
				// PAD Daten extrahieren
				// Diese befinden sich in einem Data Stream Element des AAC-Codecs 
				if((au[0] & 0xE0) == (AAC_SYN_ELEM_ID_DATA_STREAM << 5))
				{
					
				}
				
				// AAC-Dekoder konfigurieren
				if(config_decoder(aac_sample_freq, mpeg_surround_config, is_stereo))
				{
					// Dekodieren!
					NeAACDecFrameInfo frame_info;
					int16_t *pcm_samples = (int16_t *)NeAACDecDecode(m_AAC_handle, &frame_info, const_cast<uint8_t *>(au), au_size);
					
					if (frame_info.error != 0) 
					{
						printf("MP4 Decoder: Warning: %s\n", faacDecGetErrorMessage(frame_info.error));
						return 0;
					}
					
					// PCM in die Ausgangspuffer schreiben
      				if(frame_info.channels == 2)
					{
						for(int32_t i = 0; i < (frame_info.samples / 2); ++i)
						{
							// Stereochannels sind interleaved.
							left[i] = pcm_samples[i * 2] / 32767.0f;
							right[i] = pcm_samples[i * 2 + 1] / 32767.0f;
						}
						
						return frame_info.samples / 2;
					}
					else if(frame_info.channels == 1)
					{
						for(int32_t i = 0; i < frame_info.samples; ++i)
						{
							left[i] = right[i] = pcm_samples[i] / 32767.0f;
						}
						
						return frame_info.samples;
					}
					else if(m_debug_enable != 0)
						printf("MP4 Decoder: Aktuelle Channelkonfiguration wird nicht unterstützt (channels: %d)\n", frame_info.channels);
				}
			}
			
			return 0;
		}
		
		bool MP4_decoder_impl::check_CRC16(const uint8_t *data, int16_t length)
		{
			// CRC initialisieren
			uint16_t crc = 0xFFFF;
			
			// CRC16 byteweise berechnen
			for(int32_t i = 0; i < length - 2; ++i)				
				crc = (crc << 8) ^ m_crc_table[((crc >> 8) ^ data[i]) & 0x00FF];
			
			crc = ~crc;
			uint16_t crc_received = (data[length - 2] << 8) | data[length - 1];
			
			// Wenn die Debugausgabe aktiv ist...
			if(m_debug_enable >= 2)
			{
				if(crc == crc_received)
					printf("CRC16 Check successful!\n");
				
				printf("CRC16 (calculated): 0x%04hX\n", crc);
				printf("CRC16 (received): 0x%04hX\n", crc_received);
			}
			
			return crc == crc_received;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

