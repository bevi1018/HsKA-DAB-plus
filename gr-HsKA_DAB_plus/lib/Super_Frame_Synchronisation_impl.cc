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
- Block: Super_Frame_Synchronisation
- Beschreibung: Schätzt die Rahmenstruktur von DAB+ Signalen, siehe [1; S.23]. Da der Empfänger asychron eingeschaltet wird, muss zunächst das Null Symbol
  gefunden werden. Dieses enthält keine oder nur geringfügig Sendeleistung. Der gleitende Mittelwert der Momentanleistung über die Null Symbol Dauer hat 
  bei exakter Überlappung dort ein Minimum. Der Übergang von fallenden Flanke zu steigender Flanke wird mit einem Vergleich der Leistung mit einer kurzen 
  zeitlichen Differenz detektiert.
- Quellen: [1] ETSI Standard EN 300 401
*/

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include "Super_Frame_Synchronisation_impl.h"


namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		Super_Frame_Synchronisation::sptr Super_Frame_Synchronisation::make(int32_t T_NULL, int32_t LOOK_AHEAD, int32_t debug_enable)
		{
			// Echter Konstruktor aufrufen und die Instanz GnuRadio zur Verfügung stellen.
			return gnuradio::get_initial_sptr(new Super_Frame_Synchronisation_impl(T_NULL, LOOK_AHEAD, debug_enable));
		}

		Super_Frame_Synchronisation_impl::Super_Frame_Synchronisation_impl(int32_t T_NULL, int32_t LOOK_AHEAD, int32_t debug_enable)
			: gr::block("Super_Frame_Synchronisation", 										// Grundlegender Blockkonstruktor aufrufen
						gr::io_signature::make(1, 1, sizeof(gr_complex)),					// 1 Komplexer Dateneingang
						gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(uint8_t))),	// 1 Komplexer Datenausgang und 1 Byte zur Synchronisation 
			  power(T_NULL), 				// Mittelwert wird über die Länge des Nullsymbols gebildet
			  data_buffer(LOOK_AHEAD + 1),	// Wir möchten die letzten LOOK_AHEAD Samples sehen
			  power_buffer(LOOK_AHEAD + 1)	// Wir möchten die letzten LOOK_AHEAD Leistungen sehen
		{
			// Konstruktorparameter speichern
			this->T_NULL = T_NULL;
			this->LOOK_AHEAD = LOOK_AHEAD;
			
			// Zähler auf 0 setzen
			negative_counter = 0;
			super_frame_counter = 0;
				  
			// Debugswitch setzen
			this->debug_enable = debug_enable;
		}

		Super_Frame_Synchronisation_impl::~Super_Frame_Synchronisation_impl()
		{
		}

		void Super_Frame_Synchronisation_impl::forecast(int32_t noutput_items, gr_vector_int &ninput_items_required)
		{
			// Es werden genauso viele Eingangsdaten wie Ausgangsdaten benötigt
			for(int32_t i = 0; i < ninput_items_required.size(); ++i)
			{
				ninput_items_required[i] = noutput_items;	
			}
		}

		int32_t Super_Frame_Synchronisation_impl::general_work(int32_t noutput_items, gr_vector_int &ninput_items, gr_vector_const_void_star &input_items, gr_vector_void_star &output_items)
		{
			// Eingangs- und Ausgangsvektoren zur einfacheren Handhabung casten
			const gr_complex *in = (const gr_complex *) input_items[0];
			gr_complex *out		 = (gr_complex *) output_items[0];
			uint8_t *sync		 = (uint8_t *) output_items[1];

			// Alle geforderten Daten generieren
			for(int32_t i = 0; i < noutput_items; i++)
			{
				// Speichern des aktuellen Samples in den Ringpuffer
				data_buffer.pushValue(in[i]);

				// Ermittlung einer mittleren Momentanleistung
				float currentPower = power.filter(norm(in[i]));
				
				// Speichern der mittleren Momentanleistung im Ringpuffer
				power_buffer.pushValue(currentPower);

				// Vergleich der aktuellen Leistung mit der Leistung vor LOOK_AHEAD Samples
				float delta = currentPower - power_buffer.getValueAt(LOOK_AHEAD);

				// Sync-Signal zunächst auf 0 setzen
				sync[i] = 0;
				
				// Ist die gemittelte Leistung schon lang genug gefallen? (80% der Nullsymbollänge zur Vermeidung einer Fehldetektion)
				if(negative_counter > T_NULL * 8 / 10)
				{
					// Beginnt die Leistung schon wieder zu steigen? Dann wurde ein Symbolanfang gefunden!
					if(delta > 0.0f)
					{
						// Symbolanfang mit einer 1 im Sync-Signal markieren
						sync[i] = 1;
						
						// Debugausgabe aktiv?
						if(debug_enable != 0)
						{
							// Sample mit einem Tag für Debugzwecke markieren
							add_item_tag(0, nitems_written(0) + i, pmt::mp("Super Frame Start"), pmt::from_long(super_frame_counter++));
							printf("Super Frame Start\n");
						}
					}
				}

				// Wenn die Leistung abnimmt: negative_counter inkrementieren, sonst auf 0 setzen
				if(delta < 0.0f)
					negative_counter++;
				else
					negative_counter = 0;
				
				// Da die Steigung der Leistung über LOOK_AHEAD Samples berechnet wird, hat das Sync-Signal eine Verzögerung um LOOK_AHEAD/2 Samples
				// Das muss für die gesampleten Daten auch gelten
				out[i] = data_buffer.getValueAt(LOOK_AHEAD/2);
			}

			// Es wurden noutput_items verarbeitet.
			consume_each(noutput_items);
			
			// Es wurden noutput_items ausgegeben.
			return noutput_items;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

