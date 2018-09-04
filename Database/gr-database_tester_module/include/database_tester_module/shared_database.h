#ifndef SHARED_DATABASE_H
#define SHARED_DATABASE_H

#include <stdint.h>
#include <boost/unordered_map.hpp>
#include <boost/thread/mutex.hpp>
#include <database_module/api.h>

namespace gr 
{
	namespace database_module 
	{	
		typedef boost::unordered_map<uint32_t, void *> db_type;
		
		class shared_database;
		DATABASE_MODULE_API shared_database *get_database(const char *database_id);
		
		class DATABASE_MODULE_API shared_database
		{
		public:				
			bool variable_exists(uint32_t id)
			{
				// Zugriff auf die Datenbank sichern
				boost::mutex::scoped_lock lock(*m_mutex);

				// Prüfen, ob die Variable vorhanden ist
				return (m_database->find(id) != m_database->end());
			};

			template<typename T>
			bool create_variable(uint32_t id, T &init_value);

			template<typename T>
			bool create_variable(uint32_t id);

			template<typename T>
			T get_variable(uint32_t id);

			template<typename T>
			bool set_variable(uint32_t id, T &value);

			template<typename T>
			bool destroy_variable(uint32_t id);

		private:
			db_type *m_database;
			boost::mutex *m_mutex;
		};

		template<typename T>
		bool shared_database::create_variable(uint32_t id, T &init_value)
		{
			// Zugriff auf die Datenbank sichern
			boost::mutex::scoped_lock lock(*m_mutex);
			
			// Prüfen, ob die Variable schon vorhanden ist
			db_type::iterator iterator = m_database->find(id);
			if(iterator != m_database->end())
			{
				//Variable schon vorhanden
				return false;
			}	

			// Speicher reservieren und Variable kopieren
			T *var = new T(init_value);

			// Variable in die Datenbank schreiben
			m_database->insert(std::make_pair(id, var));

			return true;
		}

		template<typename T>
		bool shared_database::create_variable(uint32_t id)
		{
			// Zugriff auf die Datenbank sichern
			boost::mutex::scoped_lock lock(*m_mutex);
			
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
		T shared_database::get_variable(uint32_t id)
		{
			// Zugriff auf die Datenbank sichern
			boost::mutex::scoped_lock lock(*m_mutex);
			
			// Prüfen, ob die Variable vorhanden ist
			db_type::iterator var = m_database->find(id);
			if(var == m_database->end())
			{
				//Variable nicht vorhanden
				return T();
			}

			// Wert zurückgeben
			return *((T*)var->second);
		}

		template<typename T>
		bool shared_database::set_variable(uint32_t id, T &value)
		{
			// Zugriff auf die Datenbank sichern
			boost::mutex::scoped_lock lock(*m_mutex);
			
			// Prüfen, ob die Variable vorhanden ist
			db_type::iterator var = m_database->find(id);
			if(var == m_database->end())
			{
				//Variable nicht vorhanden -> Variable anlegen
				
				// Speicher reservieren und Variable kopieren
				T *var = new T(value);

				// Variable in die Datenbank schreiben
				m_database->insert(std::make_pair(id, var));
				
				return false;
			}

			*((T*)var->second) = value;
			return true;
		}

		template<typename T>
		bool shared_database::destroy_variable(uint32_t id)
		{
			// Zugriff auf die Datenbank sichern
			boost::mutex::scoped_lock lock(*m_mutex);
			
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

