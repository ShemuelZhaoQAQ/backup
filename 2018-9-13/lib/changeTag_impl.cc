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
#include "changeTag_impl.h"

namespace gr {
  namespace SCMA {

    changeTag::sptr
    changeTag::make(int multi,const std::string &len_tag_key,bool debug_flag)
    {
      return gnuradio::get_initial_sptr
        (new changeTag_impl(multi,len_tag_key,debug_flag));
    }

    /*
     * The private constructor
     */
    changeTag_impl::changeTag_impl(int multi,const std::string &len_tag_key,bool debug_flag)
      : gr::tagged_stream_block("changeTag",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(unsigned char)), len_tag_key),
              d_multi(multi),
              d_debug_flag(debug_flag)
    {
      set_tag_propagation_policy(TPP_DONT);
    }

    /*
     * Our virtual destructor.
     */
    changeTag_impl::~changeTag_impl()
    {
    }

    int
    changeTag_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = d_multi*ninput_items[0];
      return noutput_items ;
    }

    int
    changeTag_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      size_t packet_length = ninput_items[0];
      const unsigned char *in = (const unsigned char *) input_items[0];
      unsigned char *out = (unsigned char *) output_items[0];
      for (size_t i = 0; i < packet_length*d_multi; i++) {
        /* code */
        memcpy(out, in, sizeof(unsigned char)*packet_length);
        out+=packet_length;
      }
      std::vector<tag_t> tags;
      get_tags_in_range(tags, 0, nitems_read(0),nitems_read(0)+packet_length*d_multi );
      for (unsigned int j = 0; j < tags.size(); j++) {
             tags[j].offset = tags[j].offset*packet_length*d_multi;
          }
      if (d_debug_flag) {
        for (size_t i = 0; i < packet_length*d_multi; i++) {
          std::cout << "input :  " <<i<<":\t"<<(int)out[i]<< '\n';
          std::cout << "output:  " <<i<<":\t"<<(int)out[i]<< '\n';
        }
        d_debug_flag =false;
      }
      return packet_length*d_multi;
    }


  } /* namespace SCMA */
} /* namespace gr */
