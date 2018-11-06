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
#include <HsKA_DAB_plus/radio_gui.h>
#include <HsKA_DAB_plus/FIB_Info.h>
#include <QApplication>
#include <chrono>

namespace gr
{
	namespace HsKA_DAB_plus
	{
		radio_gui::radio_gui(char *databaseID, int32_t debug_enable, QWidget *parent)
			: m_parent(parent), 
			  m_is_running(true), 
		 	  m_thread(&radio_gui::update_thread, this),
			  m_database(shared_database::open_database(databaseID)),
			  m_debug_enable(debug_enable)
		{
			if(qApp != NULL)
			{
				// Die QT-App von GnuRadio kopieren
				m_application = qApp;
			}
			else
			{
				int argc = 0;
				char **argv = NULL;
				m_application = new QApplication(argc, argv);
			}	

			// Neues Widget erzeugen und zur App hinzufügen
			m_widget = new dab_radio_widget(m_parent);
		}

		radio_gui::~radio_gui()
		{
			// Update-Thread stoppen und auf Abschluss warten
			m_is_running = false;
			m_thread.join();
		}
		
		void radio_gui::center_frequency_changed(double frequency)
		{
			// Warten bis sich die Frequenzänderung komplett durch den GnuRadio-Flowgraph durch ist
			std::this_thread::sleep_for(std::chrono::seconds(1));
			
			// Alle Services löschen
			ServiceEnumeration *services;
			if(m_database->check_and_get_variable<ServiceEnumeration *>(DB_SERVICE_ENUMERATION_PTR_ID, &services))
				services->services.clear();

			// Senderliste löschen
			QStringList empty_list;
			m_widget->set_sender_list(empty_list);
		}
		
		void radio_gui::set_center_freq_callback(py_callback &center_freq_callback)
		{		
			// Python-Callback speichern
			m_widget->set_center_freq_callback(center_freq_callback, std::bind(&radio_gui::center_frequency_changed, this, std::placeholders::_1));
		}
															  
		void radio_gui::update_thread()
		{
			char buffer[128];
			
			DateandTime date_time;
			EnsembleLabel ensemble_label;
			ConfigInfo config_info;
			ProgrammeType program_type;
			
			ServiceOrga current_service_orga;
			uint32_t current_service_id = 0;
			
			ServiceEnumeration *service_enum = nullptr;
			
			// Warten, bis das ServieEnumeration-Objekt verfügbar ist (Wird im FIB-Decoder erzeugt
			while(!m_database->check_and_get_variable<ServiceEnumeration *>(DB_SERVICE_ENUMERATION_PTR_ID, &service_enum))
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			
			while(m_is_running)
			{
				// Die GUI nur alle 1s aktualisieren, damit die Prozessorlast niedrig bleibt
				std::this_thread::sleep_for(std::chrono::seconds(1));
				
				// Ensemble-Label lesen und anzeigen
				if(m_database->check_and_get_variable<EnsembleLabel>(DB_ENSEMBLE_LABEL_ID, &ensemble_label))
				{
					m_widget->set_ensemble_name((char *)ensemble_label.character_field);
				}
				else
				{
					sprintf(buffer, "--");				
					m_widget->set_ensemble_name(buffer);
				}
				
				// Service Count schreiben
				uint32_t sender_count = service_enum->services.size();
				sprintf(buffer, "%d", sender_count);				
				m_widget->set_service_count(buffer);
				
				// Service Namen der Combobox hinzufügen
				QStringList sender_list = build_sender_list(service_enum->services);
				if(m_widget->get_sender_list_length() < sender_list.count())
					m_widget->set_sender_list(sender_list);
				
				// Bei Bedarf die Anzahl der Sender ausgeben
				if(m_debug_enable != 0)
					printf("\n\nSender list length: %d\n\n", sender_list.count());
				
				// Wenn ein Sender ausgewählt wurde...
				if(m_widget->get_selected_sender_index() >= 0 && (service_enum->services.size() > m_widget->get_selected_sender_index()))
				{
					// Aktuelle Service-ID speichern
					current_service_id = service_enum->services[m_widget->get_selected_sender_index()];
					
					// Service-Organisation laden
					if(m_database->check_and_get_variable<ServiceOrga>(DB_SERVICE_ORGA_ID(current_service_id), &current_service_orga))
					{
						// Bei Bedarf die Anzahl der Service Komponenten ausgeben
						if(m_debug_enable != 0)						
							printf("Service Component Count: %d\n", current_service_orga.ncomponents);
						
						// Alle Service-Komponenten iterieren
						for(int i = 0; i < current_service_orga.ncomponents; ++i)
						{
							// Bei Bedarf Infos zu der Service-Komponente ausgeben
							if(m_debug_enable != 0)
								printf("Service Component: TMID: %d, SCTY: %d, SubChID: %d\n", current_service_orga.tmid[i], current_service_orga.scty[i], current_service_orga.subchid[i]);
							
							// Handelt es sich um Stream Mode Audio?
							if(current_service_orga.tmid[i] == 0)
							{
								// Handelt es sich um DAB+ Audio?
								if(current_service_orga.scty[i] == 63)
								{
									// Wenn ja, Subchannel-ID lesen und in die Datenbank schreiben
									uint32_t subch_id = current_service_orga.subchid[i];									
									m_database->set_variable<uint32_t>(DB_SELECTED_SUBCHANNEL_ID, subch_id);
									break;
								}
							}
						}
						
						// Erweiterter Ländercode auf 0xE0 (Europa) setzen, falls nicht gesetzt
						if(current_service_orga.ecc == 0)
							current_service_orga.ecc = 0xE0;
						
						// Ländername laden und anzeigen
						CountryName::get_country_name(current_service_orga.ecc, current_service_orga.country_id, buffer);
						m_widget->set_country(buffer);
					}
					else
					{
						sprintf(buffer, "--");
						m_widget->set_country(buffer);
					}
					
					// Programm-Typ laden und anzeigen
					if(m_database->check_and_get_variable<ProgrammeType>(DB_PROGRAM_TYPE_ID(current_service_id), &program_type))
					{
						sprintf(buffer, "%s", programmetypecode[program_type.international_code]);
						m_widget->set_program_type(buffer);
					}
					else
					{
						sprintf(buffer, "--");
						m_widget->set_program_type(buffer);
					}
				}
				else
				{
					sprintf(buffer, "--");				
					m_widget->set_country(buffer);
					m_widget->set_program_type(buffer);
				}
				
				// Datum und Uhrzeit laden und anzeigen
				if(m_database->check_and_get_variable<DateandTime>(DB_DATE_AND_TIME_ID, &date_time))
				{
					// Uhrzeit anzeigen
					sprintf(buffer, "%02d:%02d UTC", date_time.hours, date_time.minutes);
					m_widget->set_time(buffer);
					
					// Datum konvertieren und anzeigen
					convert_date(buffer, date_time.mjd);
					m_widget->set_date(buffer);
				}
				else
				{
					sprintf(buffer, "--");				
					m_widget->set_time(buffer);
					m_widget->set_date(buffer);
				}
			}
		}
		
		void radio_gui::convert_date(char *buffer, uint32_t mjd)
		{				
			// MJD in Tage seit dem 01.01.0000 umwandeln
			mjd += 678941;
			
			uint32_t year = 0;
			uint32_t month = 0;
			uint32_t day = 0;
			
			// Anzahl der Tage pro 400, 100 und 4 Jahre berechnen
			const uint32_t days_per_400_year = 400 * 365 + 100 - 4 + 1; // Letztes Jahr enthält schon den Schalttag
			const uint32_t days_per_100_year = 100 * 365 + 25;
			const uint32_t days_per_4_year = 4 * 365 + 1; // Letztes Jahr enthält schon den Schalttag
			const uint8_t days_per_month[] = { 31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31 };
			const char *month_name[] = { "January", "February", "March", "April", "May", "June", "July", "August", "September", "October", "November", "December" };
			
			bool is_leap_year = false;
				
			// Jahreszahl in 400 Jahre Schritte inkrementieren
			while(mjd >= days_per_400_year)
			{
				year += 400;
				mjd -= days_per_400_year;
				
				// Durch 400 teilbare Jahre sind Schaltjahre
				is_leap_year = true;
			}
			
			// Jahreszahl in 100 Jahre Schritte inkrementieren
			while(mjd >= days_per_100_year)
			{
				if(is_leap_year)
					mjd--;
					
				year += 100;
				mjd -= days_per_100_year;
				
				// Alle durch 100 teilbare Jahre sind keine Schaltjahre
				is_leap_year = false;
			}
			
			// Jahreszahl in 4 Jahre Schritte inkrementieren
			while(mjd >= days_per_4_year)
			{
				year += 4;
				mjd -= days_per_4_year;
				
				// Durch 4 teilbare Jahre sind Schaltjahre
				is_leap_year = true;
			}
			
			// Ansonsten in 1 Jahr Schritte inkrementieren
			while(mjd >= 365)
			{
				if(is_leap_year)
					mjd--;
				
				year++;
				mjd -= 365;
				
				// Kein Schaltjahr
				is_leap_year = false;
			}
			
			for(uint32_t i = 0; i < 12; ++i)
			{
				uint8_t month_days = days_per_month[i];
				
				// Aktueller Monat ist Februar und das Jahr ist ein Schaltjahr?
				if((i == 1) && is_leap_year)
					month_days++;
				
				if(mjd < month_days)
					break;
				
				month++;
				mjd -= month_days;
			}
			
			day = mjd;
			
			// Datum formatieren
			sprintf(buffer, "%02d. %s %d", day + 1, month_name[month], year);
		}
		
		QStringList radio_gui::build_sender_list(std::vector<uint32_t> &services)
		{
			QStringList sender_list;
			ProgrammeServiceLabel program_service_label;
			char buffer[17];
			
			// Alle Sender iterieren
			uint32_t sender_count = services.size();
			for(int i = 0; i < sender_count; ++i)
			{
				// Program-Service-Label laden
				if(m_database->check_and_get_variable<ProgrammeServiceLabel>(DB_PROGRAM_SERVICE_LABEL_ID(services[i]), &program_service_label))
				{
					// Label in den Puffer schreiben
					strncpy(buffer, (char *)program_service_label.character_field, 16);
					// Terminierung schreiben
					buffer[16] = 0;
					// Sendername in die Liste speichern
					sender_list.push_back(buffer);
				}
			}
			
			// Senderliste zurückgeben
			return sender_list;
		}
		
		PyObject* radio_gui::pyqwidget()
		{
			PyObject *w = PyLong_FromVoidPtr((void*)m_widget);
			PyObject *retarg = Py_BuildValue("N", w);
			return retarg;
		}
	} /* namespace HsKA_DAB_plus */
} /* namespace gr */

