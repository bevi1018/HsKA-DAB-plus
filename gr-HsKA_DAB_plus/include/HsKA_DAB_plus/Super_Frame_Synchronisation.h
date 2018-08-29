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


#ifndef INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_H
#define INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_H

#include <HsKA_DAB_plus/api.h>
#include <gnuradio/block.h>

namespace gr {
  namespace HsKA_DAB_plus {

    /*!
     * \brief <+description of block+>
     * \ingroup HsKA_DAB_plus
     *
     */
    class HSKA_DAB_PLUS_API Super_Frame_Synchronisation : virtual public gr::block
    {
     public:
      typedef boost::shared_ptr<Super_Frame_Synchronisation> sptr;

      /*!
       * \brief Return a shared_ptr to a new instance of HsKA_DAB_plus::Super_Frame_Synchronisation.
       *
       * To avoid accidental use of raw pointers, HsKA_DAB_plus::Super_Frame_Synchronisation's
       * constructor is in a private implementation
       * class. HsKA_DAB_plus::Super_Frame_Synchronisation::make is the public interface for
       * creating new instances.
       */
      static sptr make(int T_NULL, int LOOK_AHEAD);
    };

  } // namespace HsKA_DAB_plus
} // namespace gr

#endif /* INCLUDED_HSKA_DAB_PLUS_SUPER_FRAME_SYNCHRONISATION_H */

