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


#ifndef INCLUDED_HSKA_DAB_PLUS_RADIO_GUI_H
#define INCLUDED_HSKA_DAB_PLUS_RADIO_GUI_H

#include <HsKA_DAB_plus/api.h>
#include <HsKA_DAB_plus/shared_database.h>
#include <HsKA_DAB_plus/py_callback.h>
#include <HsKA_DAB_plus/country_name.h>
#include <Python.h>
#include <QWidget>
#include <thread>

#include "dab_radio_widget.h"

namespace gr
{
	namespace HsKA_DAB_plus 
	{
		/**
		 * Block der die zum Radio gehoerende GUI anzeigt.
		 * \author Vivian Becher
		 * \author David Kohler
		**/
		class HSKA_DAB_PLUS_API radio_gui
		{
		public:
			/**
			 * Konstruktor
			 * \param databaseID ID der Datenbank
			 * \param debug_enable 0 deaktiviert die Debugausgabe, sonst ist die Debugausgabe aktiv
			 * \param parent QWidget des Parents, falls die Oberflaeche in eine andere Oberflaeche eingebettet werden soll.
			**/
			radio_gui(char *databaseID, int32_t debug_enable, QWidget *parent = NULL);
			
			/**
			 * Destruktor
			**/
			~radio_gui();
			
			/**
			 * Gibt das QWidget als Python-Objekt zurueck.
			 * \return Das zu dieser GUI gehoerende QWidget als Python-Objekt.
			**/
			PyObject* pyqwidget();
			
			/**
			 * Setzt den Callback, mit dem die Centerfrequenz des SDR gesetzt werden kann. Dafuer muss in Python von der py_callback-Klasse abgeleitet werden.
			 * \param center_freq_callback Callback, der die Centerfrequenz des SDR setzt.
			**/
			void set_center_freq_callback(py_callback &center_freq_callback);
		
		private:
			QWidget *			m_parent;		///< QWidget des Parents
			QApplication *		m_application;	///< Aktuelle Anwendung, enthaelt den Message-Loop
			dab_radio_widget *	m_widget;		///< QWidget, das die GUI enthaelt
			shared_database *	m_database;		///< Instanz der Datenbank
			int32_t				m_debug_enable;	///< Debugswitch
			
			std::thread			m_thread;		///< Thread-Objekt. Aktualisiert die anzuzeigenden Variablen
			bool				m_is_running;	///< Solange das Flag auf True steht, laeuft der Aktualisierungsthread
			
			/**
			 * Wird von der GUI aufgerufen, wenn die Centerfrequenz geaendert wurde.
			 * \param frequency Neue Centerfrequenz
			**/
			void center_frequency_changed(double frequency);
			
			/**
			 * Thread, der jede Sekunde alle anzuzeigenden Werte aktualisiert.
			**/
			void update_thread();
			
			/**
			 * Wandelt das Modifizierte julianische Datum in Text um.
			 * \param buffer Puffer, in den der Text geschrieben wird.
			 * \param mjd Das umzuwandelnde modifizierte julianische Datum
			**/
			void convert_date(char *buffer, uint32_t mjd);
			
			/**
			 * Liest alle Sendernamen anhand eines Arrays mit Service-IDs und speichert diese in ein String-Array.
			 * \param services Array mit Service-IDs dessen Sendernamen gelesen werden.
			 * \return Array mit Sendernamen.
			**/
			QStringList build_sender_list(std::vector<uint32_t> &services);	
		};
	} // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_RADIO_GUI_H */

