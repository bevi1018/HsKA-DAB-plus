#!/bin/bash

gnuradio_dir=/home/gnuradiouser/gnuradio/installation/default

cmake .. -DCMAKE_PREFIX_PATH=$gnuradio_dir -DCMAKE_INSTALL_PREFIX=$gnuradio_dir && make && make install

