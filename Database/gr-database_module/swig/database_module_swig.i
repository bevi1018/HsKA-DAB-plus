/* -*- c++ -*- */

#define DATABASE_MODULE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "database_module_swig_doc.i"

%{
#include "database_module/database.h"
#include "database_module/database_tester.h"
#include "database_module/shared_database.h"
using namespace gr::database_module;
%}


%include "database_module/database.h"
%include "database_module/database_tester.h"
%include "database_module/shared_database.h"


	