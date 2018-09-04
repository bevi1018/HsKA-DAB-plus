/* -*- c++ -*- */

#define DATABASE_TESTER_MODULE_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "database_tester_module_swig_doc.i"

%{
#include "database_tester_module/database_tester.h"
%}


%include "database_tester_module/database_tester.h"
