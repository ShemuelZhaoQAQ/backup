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

#ifndef INCLUDED_SCMA_ENCODE_IMPL_H
#define INCLUDED_SCMA_ENCODE_IMPL_H

#include <SCMA/encode.h>

namespace gr {
  namespace SCMA {

    class encode_impl : public encode
    {
    private:
         unsigned d_packet_len;
         const std::string &d_len_tag_key;
         pmt::pmt_t d_packet_len_pmt;
         uint64_t d_next_tag_pos;
         unsigned char d_user_codebook;
         void codebookMapping();
         gr_complex sum_zero(int resource);
         std::vector<std::vector<gr_complex> > d_codebook; //(4,std::vector<int>(4,0));
    protected:
         int calculate_output_stream_length(const gr_vector_int &ninput_items);

    public:
         encode_impl(unsigned char user_codebook,unsigned packet_len,const std::string &len_tag_key);
         ~encode_impl();
         bool isValidCodebook();
         // Where all the action really happens
         int work(int noutput_items,
              gr_vector_int &ninput_items,
              gr_vector_const_void_star &input_items,
              gr_vector_void_star &output_items);
       };

  } // namespace SCMA
} // namespace gr

#endif /* INCLUDED_SCMA_ENCODE_IMPL_H */
