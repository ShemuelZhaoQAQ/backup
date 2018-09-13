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


#ifndef INCLUDED_SCMA_CONSTELLATION_OWN_H
#define INCLUDED_SCMA_CONSTELLATION_OWN_H

#include <SCMA/api.h>
#include <SCMA/metric_type.h>
#include <boost/enable_shared_from_this.hpp>
#include <boost/any.hpp>
#include <gnuradio/gr_complex.h>
#include <SCMA/pmt.h>
#include <vector>
#include "gnuradio/digital/constellation.h"

namespace gr {
  namespace SCMA {

    class SCMA_API constellation_SCMA : public gr::digital::constellation
    {
    public:
      typedef boost::shared_ptr<gr::SCMA::constellation_SCMA> sptr;

      // public constructor
      static sptr make();

      ~constellation_SCMA();
      gr_complex make_est(const gr_complex *sample,unsigned resource);
      void constellation_init(unsigned resource);
      unsigned int decision_maker(const gr_complex *sample);

    protected:
      constellation_SCMA();
    private:
      std::vector<std::vector<std::vector<gr_complex> > > d_codebook;
    };

  } // namespace SCMA
} // namespace gr

#endif /* INCLUDED_SCMA_CONSTELLATION_H */
