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

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <gnuradio/io_signature.h>
#include <database_tester_module/database_tester.h>
#include <database_tester_module/shared_database.h>
#include <stdio.h>

namespace gr 
{
	namespace database_tester_module 
	{
		database_tester::database_tester(gr::database_module::shared_database *database, int read_id)
		{
			printf("\nExterner Datenbankzugriff\n");
			
			if(database->variable_exists(read_id))
			{
				int x = database->get_variable<int>(read_id);
				int y = x + 1;
				database->set_variable<int>(read_id, y);
				printf("Variable %d existiert. Gelesener Wert: %d. Geschriebener Wert: %d\n", read_id, x, y);
			}
			else
			{
				int val = 15698123;
				database->create_variable<int>(read_id, val);
				printf("Variable %d existiert nicht. Geschriebener Wert: %d\n", read_id, val);
			}
		}

		database_tester::~database_tester()
		{
		}
	} /* namespace database_tester_module */
} /* namespace gr */

