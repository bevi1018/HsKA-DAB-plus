#!/bin/sh
export VOLK_GENERIC=1
export GR_DONT_LOAD_PREFS=1
export srcdir=/home/gnuradiouser/gnuradio/HsKA-DAB+/Database/gr-database_tester_module/lib
export PATH=/home/gnuradiouser/gnuradio/HsKA-DAB+/Database/gr-database_tester_module/build/lib:$PATH
export LD_LIBRARY_PATH=/home/gnuradiouser/gnuradio/HsKA-DAB+/Database/gr-database_tester_module/build/lib:$LD_LIBRARY_PATH
export PYTHONPATH=$PYTHONPATH
test-database_tester_module 
