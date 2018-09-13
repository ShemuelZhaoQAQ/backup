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

#ifndef INCLUDED_SCMA_DECODE_IMPL_H
#define INCLUDED_SCMA_DECODE_IMPL_H

#include <SCMA/decode.h>
#include <fstream>
#define uint_8		unsigned char

#define NUM_USER	     6
#define NUM_CODE	     4
#define NUM_RESOURCE	     4
#define NUM_ITERATION	     10
#define AP		     0.25
#define SNR		     21
#define numSIgv		     16
#define numSIvg		     4


namespace gr {
  namespace SCMA {

    class decode_impl : public decode
    {
     private:
           std::ofstream outfile;
           float noisePower  = 0.0;
           unsigned char d_user_codebook;
           unsigned char factor_graph[NUM_RESOURCE][NUM_USER];
           float Q_matrix[NUM_USER][NUM_CODE];
           float LLR_Matrix[2*NUM_USER];
           float sIgv[numSIgv] = {0.0};
           std::vector<unsigned char> d_index;
           std::vector<std::vector<std::vector<float> > > inforG2V;
           std::vector<std::vector<std::vector<float> > > inforV2G;
           std::vector<std::vector<std::vector<gr_complex> > > d_codebook;
           std::vector<std::vector<std::vector<std::vector<float > > > > d_distance_matrix;

     protected:
          int calculate_output_stream_length(const gr_vector_int &ninput_items);

     public:
          decode_impl(unsigned char user_codebook,const std::string &len_tag_key);
          ~decode_impl();
          // Where all the action really happens
          void messageMatrixInit();
          void factorGraphInit();
          void distanceSpaceAlloc();
          void inforG2VUpdate();
          void inforV2GUpdate();
          void LLRCal();
          float log_sum_exp(float srcMatrix[],unsigned char sumNum);
          float  Q_matrixCalulate(float Q1,float Q2, float Q3,float Q4);
          void distanceInit(const gr_complex *single_sample);
          void messageSpaceAlloc();
          void codebookInit();
          void find(unsigned char factor_graph[NUM_RESOURCE][NUM_USER],unsigned char num,bool flag  );

          // Where all the action really happens
          int work(int noutput_items,
               gr_vector_int &ninput_items,
               gr_vector_const_void_star &input_items,
               gr_vector_void_star &output_items);
    };

  } // namespace SCMA
} // namespace gr

#endif /* INCLUDED_SCMA_DECODE_IMPL_H */
