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

#include "Header_File.h"

#include <gnuradio/io_signature.h>
#include "Super_Frame_Synchronisation_impl.h"


namespace gr {
  namespace HsKA_DAB_plus {

	Super_Frame_Synchronisation::sptr
	Super_Frame_Synchronisation::make(int T_NULL, int LOOK_AHEAD)
	{
	return gnuradio::get_initial_sptr (new Super_Frame_Synchronisation_impl(T_NULL, LOOK_AHEAD));
	}

	Super_Frame_Synchronisation_impl::Super_Frame_Synchronisation_impl(int T_NULL, int LOOK_AHEAD)
	: gr::block("Super_Frame_Synchronisation",gr::io_signature::make(1, 1, sizeof(gr_complex)),gr::io_signature::make2(2, 2, sizeof(gr_complex), sizeof(byte))),
	  power(T_NULL), data_buffer(LOOK_AHEAD+1), power_buffer(LOOK_AHEAD+1)
	{
		this->T_NULL = T_NULL; // Null Symbol Dauer [1; S. 104]
		this->LOOK_AHEAD = LOOK_AHEAD;
	}

	Super_Frame_Synchronisation_impl::~Super_Frame_Synchronisation_impl()
	{
	}

	void
	Super_Frame_Synchronisation_impl::forecast (int noutput_items, gr_vector_int &ninput_items_required)
	{
		for(int i=0; i < ninput_items_required.size(); ++i)
		{
			ninput_items_required[i] = noutput_items;	
		}
	}

	int
	Super_Frame_Synchronisation_impl::general_work (int noutput_items,gr_vector_int &ninput_items,gr_vector_const_void_star &input_items,gr_vector_void_star &output_items)
	{
	const gr_complex *in = (const gr_complex *) input_items[0];
	gr_complex *out = (gr_complex *) output_items[0];
	byte *sync = (byte *) output_items[1];

	static int negative_counter = 0;
	static int super_frame_counter = 0;


	for(int i=0; i<noutput_items; i++)
	{
		data_buffer.pushValue(in[i]); // Laden des aktuellen Abtastwertes in den Ringbuffer

		float currentPower = power.filter(norm(in[i])); // Ermittlung einer mittleren Momentanleistung
		power_buffer.pushValue(currentPower); // Speichern der mittleren Momentanleistung

		float delta = currentPower - power_buffer.getValueAt(LOOK_AHEAD); // Vergleich der Leistungswerte von LOOK_AHEAD samples Zeitunterschied

		sync[i] = 0; // Initialisierung des Synchronisationsignals
		if(negative_counter > T_NULL * 8 / 10) // Totzeit zur Vermeidung von Fehldetektion
		{
			if(delta > 0.0f) // Symbolanfang augrund von Leistungszunahme verifiziert, liegt zeitlich LOOK_AHEAD/2 samples zurück
			{
				sync[i] = 1; // Update Synchronisationssignal für dieses sample
				add_item_tag(0, nitems_written(0) + i, pmt::mp("Super Frame Start"), pmt::from_long(super_frame_counter++)); // Hinzufügen einer Markierung
			}
		}

		if(delta < 0.0f) // Inkrementeller Zähler bei Leistungsabnahme
		{
			negative_counter++;
		}
		else
		{
			negative_counter = 0;
		}
		out[i] = data_buffer.getValueAt(LOOK_AHEAD/2); // Ausgabe aus dem Ringbuffer mit LOOK_AHEAD/2 samples zeitlicher Verzögerung
	}

	consume_each (noutput_items);
	return noutput_items;
	}

  } /* namespace HsKA_DAB_plus */
} /* namespace gr */

