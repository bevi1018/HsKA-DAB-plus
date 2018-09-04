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


#ifndef INCLUDED_DATABASE_MODULE_DATABASE_H
#define INCLUDED_DATABASE_MODULE_DATABASE_H

#include <database_module/api.h>
#include <database_module/shared_database.h>

namespace gr 
{
	namespace database_module 
	{		
		class DATABASE_MODULE_API database
		{
		public:
			database();
			~database();
		private:
		};
	} // namespace database_module
} // namespace gr

#endif /* INCLUDED_DATABASE_MODULE_DATABASE_H */

