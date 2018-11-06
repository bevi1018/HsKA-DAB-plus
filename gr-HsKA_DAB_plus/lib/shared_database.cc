#include <HsKA_DAB_plus/shared_database.h>
#include <boost/unordered_map.hpp>
#include <stdio.h>

namespace gr 
{
	namespace HsKA_DAB_plus 
	{
		db_list_type	shared_database::g_database_list;
		boost::mutex	shared_database::g_list_mutex;
		
		shared_database::shared_database(db_type *database, boost::mutex *mutex)
		{
			m_database = database;
			m_mutex = mutex;
		}
		
		shared_database::shared_database(const shared_database &other)
		{
			m_database = other.m_database;
			m_mutex = other.m_mutex;
		}

		shared_database::~shared_database()
		{
		}
		
		bool shared_database::variable_exists(uint64_t id)
		{
			// Zugriff auf die Datenbank sichern
			volatile boost::mutex::scoped_lock lock(*m_mutex);

			// Prüfen, ob die Variable vorhanden ist
			return (m_database->find(id) != m_database->end());
		};
		
		shared_database *shared_database::open_database(const char *database_id)
		{
			db_type *database = 0;
			boost::mutex *mutex = 0;
			
			// Zugriff auf die Datenbank sichern
			volatile boost::mutex::scoped_lock lock(g_list_mutex);
			
			// Prüfen, ob die Datenbank vorhanden ist
			db_list_type::iterator iterator = g_database_list.find(database_id);
			if(iterator != g_database_list.end())
			{
				// Datenbank ist vorhanden
				database = iterator->second.first;
				mutex = iterator->second.second;
			}
			else
			{
				// Datenbank erzeugen
				database = new db_type();
				mutex = new boost::mutex();
				g_database_list.insert(std::make_pair(database_id, std::make_pair(database, mutex)));
			}
			
			return new shared_database(database, mutex);
		}
	}
}