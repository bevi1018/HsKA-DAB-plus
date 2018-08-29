/* -*- c++ -*- */

#define HSKA_DAB_PLUS_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "HsKA_DAB_plus_swig_doc.i"

%{
#include "HsKA_DAB_plus/Super_Frame_Synchronisation.h"
%}


%include "HsKA_DAB_plus/Super_Frame_Synchronisation.h"
GR_SWIG_BLOCK_MAGIC2(HsKA_DAB_plus, Super_Frame_Synchronisation);
