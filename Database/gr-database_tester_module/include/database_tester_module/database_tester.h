/* -*- c++ -*- */
/* 
 * Copyright 2018 <+YOU OR YOUR COMPANY+>.
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


#ifndef INCLUDED_DATABASE_TESTER_MODULE_DATABASE_TESTER_H
#define INCLUDED_DATABASE_TESTER_MODULE_DATABASE_TESTER_H

#include <database_tester_module/api.h>

namespace gr
{
	namespace database_module
	{
		class shared_database;
	}
}

namespace gr
{
	namespace database_tester_module
	{
		class DATABASE_TESTER_MODULE_API database_tester
		{
		public:
			database_tester(gr::database_module::shared_database *database, int read_id);
			~database_tester();
		private:
		};
	} // namespace database_tester_module
} // namespace gr

#endif /* INCLUDED_DATABASE_TESTER_MODULE_DATABASE_TESTER_H */

