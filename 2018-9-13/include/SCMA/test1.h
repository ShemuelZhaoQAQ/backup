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


#ifndef INCLUDED_SCMA_TEST1_H
#define INCLUDED_SCMA_TEST1_H

#include <SCMA/api.h>
#include <iostream>
#include <boost/any.hpp>
namespace gr {
  namespace SCMA {

    /*!
     * \brief <+description+>
     *
     */
    class SCMA_API test1
    {
    public:
      typedef boost::shared_ptr<gr::SCMA::test1> sptr;
      test1(int test);
      ~test1();
    private:
    };

  } // namespace SCMA
} // namespace gr

#endif /* INCLUDED_SCMA_TEST1_H */
