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

#ifndef SHARED_DATABASE_H
#define SHARED_DATABASE_H

#include <stdint.h>
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include <iostream>
#include <HsKA_DAB_plus/api.h>

typedef boost::unordered_map<uint64_t, void *> db_type;
typedef boost::unordered_map<const char *, std::pair<db_type *, boost::mutex *> > db_list_type;

namespace gr 
{
	namespace HsKA_DAB_plus 
	{			
		/**
		 * \brief Threadsichere Datenbank, die es ermoeglicht Daten zwischen den Bloecken auszutauschen
		 * \author Vivian Becher
		 * \author David Kohler
		**/
		class shared_database
		{
		public:		
			/**
			 * Konstruktor (Fuer interne Zwecke)
			 * \param database Zeiger auf die Datenbank
			 * \param mutex Mutex, der die Datenbank sperren kann
			**/
			shared_database(db_type *database, boost::mutex *mutex);
			
			/**
			 * Kopierkonstruktor
			 * \param other Referenz auf die Datenbank, die kopiert werden soll
			 **/
			shared_database(const shared_database &other);
			
			/**
			 * Destruktor
			**/
			~shared_database();
			
			/**
			 * Oeffnet eine Datenbank.
			 * \param database_id Identifiziert die Datenbank
			 * \return Die geoeffnete Datenbank
			 */
			static shared_database *open_database(const char *database_id);
			
			/**
			 * Ueberprueft, ob die Variable mit der angegebenen ID existiert.
			 * \param id ID der Variable
			 * \return True, wenn die Variable existiert. Sonst False.
			**/
			bool variable_exists(uint64_t id);

			/**
			 * Erzeugt eine Variable.
			 * \param id ID der neuen Variable
			 * \param init_value Wert der neuen Variable
			 * \return True, wenn die Variable erzeugt wurde. False, wenn sie schon vorhanden ist.
			**/
			template<typename T>
			bool create_variable(uint64_t id, T &init_value);

			/**
			 * Erzeugt eine Variable.
			 * \param id ID der neuen Variable
			 * \return True, wenn die Variable erzeugt wurde. False, wenn sie schon vorhanden ist.
			**/
			template<typename T>
			bool create_variable(uint64_t id);

			/**
			 * Ueberprueft, ob eine Variable vorhanden ist. Ist dies der Fall, wird die Variable gelesen.
			 * \param id ID der Variable
			 * \param value Zeiger auf einen Speicher, in den der Inhalt der Variable geladen wird sofern sie vorhanden ist.
			 * \return True, wenn die Variable vorhanden ist. Sonst False.
			**/
			template<typename T>
			bool check_and_get_variable(uint64_t id, T *value);

			/**
			 * Schreibt den Wert einer Variable. Die Variable wird bei Bedarf angelegt, sollte diese nicht vorhanden sein.
			 * \param id ID der Variable
			 * \param value Wert der geschrieben werden soll.
			 * \return True, wenn die Variable vorhanden war. False, wenn die Variable nicht vorhanden war (Wert wird trotzdem geschrieben).
			**/
			template<typename T>
			bool set_variable(uint64_t id, T &value);

			/**
			 * Loescht eine Variable
			 * \param id ID der Variable
			 * \return True, wenn die Variable geloescht wurde. False, wenn die Variable nicht gefunden wurde.
			**/
			template<typename T>
			bool destroy_variable(uint64_t id);

		private:
			db_type *m_database;					///< Zeiger auf die Datenbank
			boost::mutex *m_mutex;					///< Mutex zum Sperren der Datenbank (Threadsicherheit)
			
			static db_list_type	g_database_list;	///< Globale Liste aller Datenbanken
			static boost::mutex	g_list_mutex;		///< Mutex zum Sperren der Datenbankliste
		};

		template<typename T>
		bool shared_database::create_variable(uint64_t id, T &init_value)
		{
			// Zugriff auf die Datenbank sichern
			volatile boost::mutex::scoped_lock lock(*m_mutex);
			
			// Prüfen, ob die Variable schon vorhanden ist
			db_type::iterator iterator = m_database->find(id);
			if(iterator != m_database->end())
			{
				//Variable schon vorhanden
				return false;
			}	

			// Speicher reservieren und Variable kopieren
			T *var = new T();
			memcpy(var, &init_value, sizeof(T));

			// Variable in die Datenbank schreiben
			m_database->insert(std::make_pair(id, var));

			return true;
		}

		template<typename T>
		bool shared_database::create_variable(uint64_t id)
		{
			// Zugriff auf die Datenbank sichern
			volatile boost::mutex::scoped_lock lock(*m_mutex);
			
			// Prüfen, ob die Variable schon vorhanden ist
			db_type::iterator var = m_database->find(id);
			if(var != m_database->end())
			{
				//Variable schon vorhanden
				return false;
			}

			// Speicher in der Datenbank reservieren
			m_database->insert(std::make_pair(id, new T()));

			return true;
		}
		
		template<typename T>
		bool shared_database::check_and_get_variable(uint64_t id, T *value)
		{
			// Zugriff auf die Datenbank sichern
			volatile boost::mutex::scoped_lock lock(*m_mutex);
			
			// Prüfen, ob die Variable vorhanden ist
			db_type::iterator var = m_database->find(id);
			if(var == m_database->end())
			{
				//Variable nicht vorhanden
				return false;
			}

			// Wert zurückgeben
			memcpy(value, var->second, sizeof(T));
			return true;
		}

		template<typename T>
		bool shared_database::set_variable(uint64_t id, T &value)
		{
			// Zugriff auf die Datenbank sichern
			volatile boost::mutex::scoped_lock lock(*m_mutex);
			
			// Prüfen, ob die Variable vorhanden ist
			db_type::iterator var = m_database->find(id);
			if(var == m_database->end())
			{
				//Variable nicht vorhanden -> Variable anlegen
				
				// Speicher reservieren und Variable kopieren
				T *var = new T();
				memcpy(var, &value, sizeof(T));

				// Variable in die Datenbank schreiben
				m_database->insert(std::make_pair(id, var));
				
				return false;
			}

			memcpy(var->second, &value, sizeof(T));
			return true;
		}

		template<typename T>
		bool shared_database::destroy_variable(uint64_t id)
		{
			// Zugriff auf die Datenbank sichern
			volatile boost::mutex::scoped_lock lock(*m_mutex);
			
			// Prüfen, ob die Variable vorhanden ist
			db_type::iterator var = m_database->find(id);
			if(var == m_database->end())
			{
				//Variable nicht vorhanden
				return false;
			}

			m_database->erase(var);

			T *obj = var->second;
			delete obj;

			return true;
		}
	}
}

#endif

