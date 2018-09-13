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
#include <SCMA/constellation_SCMA.h>
#include <gnuradio/math.h>
#include <gnuradio/gr_complex.h>
#include <cstdlib>
#include <cfloat>
#include <stdexcept>
#include <boost/format.hpp>
#include <iostream>


namespace gr {
  namespace SCMA {

    const unsigned NUM_RESOURCE =  4;
    const unsigned NUM_USER = 6;
    const unsigned NUM_CODE = 4;
/***********************************************************************/


      constellation_SCMA::sptr
      constellation_SCMA::make()
      {
        return constellation_SCMA::sptr(new constellation_SCMA());
      }
      constellation_SCMA::constellation_SCMA()
      {
              d_constellation.resize(64);
              for (size_t i = 0; i < 64; i++) {
                d_constellation[i] = gr_complex(0,0);
              }
              d_dimensionality = 1;
        #ifdef  DEBUG_MODE
              std::cout<<"#############codebookInit#######################"<<std::endl;
        #endif
              d_codebook.resize(NUM_RESOURCE);
              for(int i = 0; i<NUM_RESOURCE; i++) {
                      d_codebook[i].resize(NUM_CODE);
                      for(int j = 0; j<NUM_CODE; j++) {
                              d_codebook[i][j].resize(NUM_USER);
                      }
              }
              d_rotational_symmetry = 2;

              d_codebook[0][0][0]=gr_complex(0,0);
                  d_codebook[0][1][0]=gr_complex(0,0);
                  d_codebook[0][2][0]=gr_complex(0,0);
                  d_codebook[0][3][0]=gr_complex(0,0);

                  d_codebook[1][0][0]=gr_complex(-0.1815,-0.1318);
                  d_codebook[1][1][0]=gr_complex(-0.6351,-0.4615);
                  d_codebook[1][2][0]=gr_complex(0.6351,0.4615);
                  d_codebook[1][3][0]=gr_complex(0.1815,0.1318);

                  d_codebook[2][0][0]=gr_complex(0,0);
                  d_codebook[2][1][0]=gr_complex(0,0);
                  d_codebook[2][2][0]=gr_complex(0,0);
                  d_codebook[2][3][0]=gr_complex(0,0);

                  d_codebook[3][0][0]=gr_complex(0.7851,0);
                  d_codebook[3][1][0]=gr_complex(-0.2243,0);
                  d_codebook[3][2][0]=gr_complex(0.2243,0);
                  d_codebook[3][3][0]=gr_complex(-0.7851,0);

              d_codebook[0][0][1]=gr_complex(0.7851,0);
                  d_codebook[0][1][1]=gr_complex(-0.2243,0);
                  d_codebook[0][2][1]=gr_complex(0.2243,0);
                  d_codebook[0][3][1]=gr_complex(-0.7851,0);

                  d_codebook[1][0][1]=gr_complex(0,0);
                  d_codebook[1][1][1]=gr_complex(0,0);
                  d_codebook[1][2][1]=gr_complex(0,0);
                  d_codebook[1][3][1]=gr_complex(0,0);

                  d_codebook[2][0][1]=gr_complex(-0.1815,-0.1318);
                  d_codebook[2][1][1]=gr_complex(-0.6351,-0.4615);
                  d_codebook[2][2][1]=gr_complex(0.6351,0.4615);
                  d_codebook[2][3][1]=gr_complex(0.1815,0.1318);

                  d_codebook[3][0][1]=gr_complex(0,0);
                  d_codebook[3][1][1]=gr_complex(0,0);
                  d_codebook[3][2][1]=gr_complex(0,0);
                  d_codebook[3][3][1]=gr_complex(0,0);

              d_codebook[0][0][2]=gr_complex(-0.6351,0.4615);
                  d_codebook[0][1][2]=gr_complex(0.1815,-0.1318);
                  d_codebook[0][2][2]=gr_complex(-0.1815,0.1318);
                  d_codebook[0][3][2]=gr_complex(0.6351,-0.4615);

                  d_codebook[1][0][2]=gr_complex(0.1392,-0.1759);
                  d_codebook[1][1][2]=gr_complex(0.4873,-0.6156);
                  d_codebook[1][2][2]=gr_complex(-0.4873,0.6156);
                  d_codebook[1][3][2]=gr_complex(-0.1392,0.1759);

                  d_codebook[2][0][2]=gr_complex(0,0);
                  d_codebook[2][1][2]=gr_complex(0,0);
                  d_codebook[2][2][2]=gr_complex(0,0);
                  d_codebook[2][3][2]=gr_complex(0,0);

                  d_codebook[3][0][2]=gr_complex(0,0);
                  d_codebook[3][1][2]=gr_complex(0,0);
                  d_codebook[3][2][2]=gr_complex(0,0);
                  d_codebook[3][3][2]=gr_complex(0,0);

              d_codebook[0][0][3]=gr_complex(0,0);
                  d_codebook[0][1][3]=gr_complex(0,0);
                  d_codebook[0][2][3]=gr_complex(0,0);
                  d_codebook[0][3][3]=gr_complex(0,0);

                  d_codebook[1][0][3]=gr_complex(0,0);
                  d_codebook[1][1][3]=gr_complex(0,0);
                  d_codebook[1][2][3]=gr_complex(0,0);
                  d_codebook[1][3][3]=gr_complex(0,0);

                  d_codebook[2][0][3]=gr_complex(0.7851,0);
                  d_codebook[2][1][3]=gr_complex(-0.2243,0);
                  d_codebook[2][2][3]=gr_complex(0.2243,0);
                  d_codebook[2][3][3]=gr_complex(-0.7851,0);

                  d_codebook[3][0][3]=gr_complex(-0.0055,-0.2242);
                  d_codebook[3][1][3]=gr_complex(-0.0193,-0.7848);
                  d_codebook[3][2][3]=gr_complex(0.0193,0.7848);
                  d_codebook[3][3][3]=gr_complex(0.0055,0.2242);

              d_codebook[0][0][4]=gr_complex(-0.0055,-0.2242);
                  d_codebook[0][1][4]=gr_complex(-0.0193,-0.7848);
                  d_codebook[0][2][4]=gr_complex(0.0193,0.7848);
                  d_codebook[0][3][4]=gr_complex(0.0055,0.2242);

                  d_codebook[1][0][4]=gr_complex(0,0);
                  d_codebook[1][1][4]=gr_complex(0,0);
                  d_codebook[1][2][4]=gr_complex(0,0);
                  d_codebook[1][3][4]=gr_complex(0,0);

                  d_codebook[2][0][4]=gr_complex(0,0);
                  d_codebook[2][1][4]=gr_complex(0,0);
                  d_codebook[2][2][4]=gr_complex(0,0);
                  d_codebook[2][3][4]=gr_complex(0,0);

                  d_codebook[3][0][4]=gr_complex(-0.6351,0.4615);
                  d_codebook[3][1][4]=gr_complex(0.1815,-0.1318);
                  d_codebook[3][2][4]=gr_complex(-0.1815,0.1318);
                  d_codebook[3][3][4]=gr_complex(0.6351,-0.4615);

              d_codebook[0][0][5]=gr_complex(0,0);
                  d_codebook[0][1][5]=gr_complex(0,0);
                  d_codebook[0][2][5]=gr_complex(0,0);
                  d_codebook[0][3][5]=gr_complex(0,0);

                  d_codebook[1][0][5]=gr_complex(0.7851,0);
                  d_codebook[1][1][5]=gr_complex(-0.2243,0);
                  d_codebook[1][2][5]=gr_complex(0.2243,0);
                  d_codebook[1][3][5]=gr_complex(-0.7851,0);

                  d_codebook[2][0][5]=gr_complex(0.1392,-0.1759);
                  d_codebook[2][1][5]=gr_complex(0.4873,-0.6156);
                  d_codebook[2][2][5]=gr_complex(-0.4873,0.6156);
                  d_codebook[2][3][5]=gr_complex(-0.1392,0.1759);

                  d_codebook[3][0][5]=gr_complex(0,0);
                  d_codebook[3][1][5]=gr_complex(0,0);
                  d_codebook[3][2][5]=gr_complex(0,0);
                  d_codebook[3][3][5]=gr_complex(0,0);
      }


      void
      constellation_SCMA::constellation_init(unsigned resource )
      {

        switch (resource) {
          case 0:{

            d_constellation[0] = d_codebook[0][0][1]+d_codebook[0][0][2]+d_codebook[0][0][4];
            d_constellation[1] = d_codebook[0][0][1]+d_codebook[0][0][2]+d_codebook[0][1][4];
            d_constellation[2] = d_codebook[0][0][1]+d_codebook[0][0][2]+d_codebook[0][2][4];
            d_constellation[3] = d_codebook[0][0][1]+d_codebook[0][0][2]+d_codebook[0][3][4];

            d_constellation[4] = d_codebook[0][0][1]+d_codebook[0][1][2]+d_codebook[0][0][4];
            d_constellation[5] = d_codebook[0][0][1]+d_codebook[0][1][2]+d_codebook[0][1][4];
            d_constellation[6] = d_codebook[0][0][1]+d_codebook[0][1][2]+d_codebook[0][2][4];
            d_constellation[7] = d_codebook[0][0][1]+d_codebook[0][1][2]+d_codebook[0][3][4];

            d_constellation[8] = d_codebook[0][0][1]+d_codebook[0][2][2]+d_codebook[0][0][4];
            d_constellation[9] = d_codebook[0][0][1]+d_codebook[0][2][2]+d_codebook[0][1][4];
            d_constellation[10] = d_codebook[0][0][1]+d_codebook[0][2][2]+d_codebook[0][2][4];
            d_constellation[11] = d_codebook[0][0][1]+d_codebook[0][2][2]+d_codebook[0][3][4];

            d_constellation[12] = d_codebook[0][0][1]+d_codebook[0][3][2]+d_codebook[0][0][4];
            d_constellation[13] = d_codebook[0][0][1]+d_codebook[0][3][2]+d_codebook[0][1][4];
            d_constellation[14] = d_codebook[0][0][1]+d_codebook[0][3][2]+d_codebook[0][2][4];
            d_constellation[15] = d_codebook[0][0][1]+d_codebook[0][3][2]+d_codebook[0][3][4];

            //codeword 1
            d_constellation[16] = d_codebook[0][1][1]+d_codebook[0][0][2]+d_codebook[0][0][4];
            d_constellation[17] = d_codebook[0][1][1]+d_codebook[0][0][2]+d_codebook[0][1][4];
            d_constellation[18] = d_codebook[0][1][1]+d_codebook[0][0][2]+d_codebook[0][2][4];
            d_constellation[19] = d_codebook[0][1][1]+d_codebook[0][0][2]+d_codebook[0][3][4];

            d_constellation[20] = d_codebook[0][1][1]+d_codebook[0][1][2]+d_codebook[0][0][4];
            d_constellation[21] = d_codebook[0][1][1]+d_codebook[0][1][2]+d_codebook[0][1][4];
            d_constellation[22] = d_codebook[0][1][1]+d_codebook[0][1][2]+d_codebook[0][2][4];
            d_constellation[23] = d_codebook[0][1][1]+d_codebook[0][1][2]+d_codebook[0][3][4];

            d_constellation[24] = d_codebook[0][1][1]+d_codebook[0][2][2]+d_codebook[0][0][4];
            d_constellation[25] = d_codebook[0][1][1]+d_codebook[0][2][2]+d_codebook[0][1][4];
            d_constellation[26] = d_codebook[0][1][1]+d_codebook[0][2][2]+d_codebook[0][2][4];
            d_constellation[27] = d_codebook[0][1][1]+d_codebook[0][2][2]+d_codebook[0][3][4];

            d_constellation[28] = d_codebook[0][1][1]+d_codebook[0][3][2]+d_codebook[0][0][4];
            d_constellation[29] = d_codebook[0][1][1]+d_codebook[0][3][2]+d_codebook[0][1][4];
            d_constellation[30] = d_codebook[0][1][1]+d_codebook[0][3][2]+d_codebook[0][2][4];
            d_constellation[31] = d_codebook[0][1][1]+d_codebook[0][3][2]+d_codebook[0][3][4];

            //codeword 2
            d_constellation[32] = d_codebook[0][2][1]+d_codebook[0][0][2]+d_codebook[0][0][4];
            d_constellation[33] = d_codebook[0][2][1]+d_codebook[0][0][2]+d_codebook[0][1][4];
            d_constellation[34] = d_codebook[0][2][1]+d_codebook[0][0][2]+d_codebook[0][2][4];
            d_constellation[35] = d_codebook[0][2][1]+d_codebook[0][0][2]+d_codebook[0][3][4];

            d_constellation[36] = d_codebook[0][2][1]+d_codebook[0][1][2]+d_codebook[0][0][4];
            d_constellation[37] = d_codebook[0][2][1]+d_codebook[0][1][2]+d_codebook[0][1][4];
            d_constellation[38] = d_codebook[0][2][1]+d_codebook[0][1][2]+d_codebook[0][2][4];
            d_constellation[39] = d_codebook[0][2][1]+d_codebook[0][1][2]+d_codebook[0][3][4];

            d_constellation[40] = d_codebook[0][2][1]+d_codebook[0][2][2]+d_codebook[0][0][4];
            d_constellation[41] = d_codebook[0][2][1]+d_codebook[0][2][2]+d_codebook[0][1][4];
            d_constellation[42] = d_codebook[0][2][1]+d_codebook[0][2][2]+d_codebook[0][2][4];
            d_constellation[43] = d_codebook[0][2][1]+d_codebook[0][2][2]+d_codebook[0][3][4];

            d_constellation[44] = d_codebook[0][2][1]+d_codebook[0][3][2]+d_codebook[0][0][4];
            d_constellation[45] = d_codebook[0][2][1]+d_codebook[0][3][2]+d_codebook[0][1][4];
            d_constellation[46] = d_codebook[0][2][1]+d_codebook[0][3][2]+d_codebook[0][2][4];
            d_constellation[47] = d_codebook[0][2][1]+d_codebook[0][3][2]+d_codebook[0][3][4];

            //codeword 3
            d_constellation[48] = d_codebook[0][3][1]+d_codebook[0][0][2]+d_codebook[0][0][4];
            d_constellation[49] = d_codebook[0][3][1]+d_codebook[0][0][2]+d_codebook[0][1][4];
            d_constellation[50] = d_codebook[0][3][1]+d_codebook[0][0][2]+d_codebook[0][2][4];
            d_constellation[51] = d_codebook[0][3][1]+d_codebook[0][0][2]+d_codebook[0][3][4];

            d_constellation[52] = d_codebook[0][3][1]+d_codebook[0][1][2]+d_codebook[0][0][4];
            d_constellation[53] = d_codebook[0][3][1]+d_codebook[0][1][2]+d_codebook[0][1][4];
            d_constellation[54] = d_codebook[0][3][1]+d_codebook[0][1][2]+d_codebook[0][2][4];
            d_constellation[55] = d_codebook[0][3][1]+d_codebook[0][1][2]+d_codebook[0][3][4];

            d_constellation[56] = d_codebook[0][3][1]+d_codebook[0][2][2]+d_codebook[0][0][4];
            d_constellation[57] = d_codebook[0][3][1]+d_codebook[0][2][2]+d_codebook[0][1][4];
            d_constellation[58] = d_codebook[0][3][1]+d_codebook[0][2][2]+d_codebook[0][2][4];
            d_constellation[59] = d_codebook[0][3][1]+d_codebook[0][2][2]+d_codebook[0][3][4];

            d_constellation[60] = d_codebook[0][3][1]+d_codebook[0][3][2]+d_codebook[0][0][4];
            d_constellation[61] = d_codebook[0][3][1]+d_codebook[0][3][2]+d_codebook[0][1][4];
            d_constellation[62] = d_codebook[0][3][1]+d_codebook[0][3][2]+d_codebook[0][2][4];
            d_constellation[63] = d_codebook[0][3][1]+d_codebook[0][3][2]+d_codebook[0][3][4];

            break;
          };
          case 1:{
            d_constellation[0] = d_codebook[1][0][0]+d_codebook[1][0][3]+d_codebook[1][0][5];
            d_constellation[1] = d_codebook[1][0][0]+d_codebook[1][0][3]+d_codebook[1][1][5];
            d_constellation[2] = d_codebook[1][0][0]+d_codebook[1][0][3]+d_codebook[1][2][5];
            d_constellation[3] = d_codebook[1][0][0]+d_codebook[1][0][3]+d_codebook[1][3][5];

            d_constellation[4] = d_codebook[1][0][0]+d_codebook[1][1][3]+d_codebook[1][0][5];
            d_constellation[5] = d_codebook[1][0][0]+d_codebook[1][1][3]+d_codebook[1][1][5];
            d_constellation[6] = d_codebook[1][0][0]+d_codebook[1][1][3]+d_codebook[1][2][5];
            d_constellation[7] = d_codebook[1][0][0]+d_codebook[1][1][3]+d_codebook[1][3][5];

            d_constellation[8] = d_codebook[1][0][0]+d_codebook[1][2][3]+d_codebook[1][0][5];
            d_constellation[9] = d_codebook[1][0][0]+d_codebook[1][2][3]+d_codebook[1][1][5];
            d_constellation[10] = d_codebook[1][0][0]+d_codebook[1][2][3]+d_codebook[1][2][5];
            d_constellation[11] = d_codebook[1][0][0]+d_codebook[1][2][3]+d_codebook[1][3][5];

            d_constellation[12] = d_codebook[1][0][0]+d_codebook[1][3][3]+d_codebook[1][0][5];
            d_constellation[13] = d_codebook[1][0][0]+d_codebook[1][3][3]+d_codebook[1][1][5];
            d_constellation[14] = d_codebook[1][0][0]+d_codebook[1][3][3]+d_codebook[1][2][5];
            d_constellation[15] = d_codebook[1][0][0]+d_codebook[1][3][3]+d_codebook[1][3][5];

            //codeword 1
            d_constellation[16] = d_codebook[1][1][0]+d_codebook[1][0][3]+d_codebook[1][0][5];
            d_constellation[17] = d_codebook[1][1][0]+d_codebook[1][0][3]+d_codebook[1][1][5];
            d_constellation[18] = d_codebook[1][1][0]+d_codebook[1][0][3]+d_codebook[1][2][5];
            d_constellation[19] = d_codebook[1][1][0]+d_codebook[1][0][3]+d_codebook[1][3][5];

            d_constellation[20] = d_codebook[1][1][0]+d_codebook[1][1][3]+d_codebook[1][0][5];
            d_constellation[21] = d_codebook[1][1][0]+d_codebook[1][1][3]+d_codebook[1][1][5];
            d_constellation[22] = d_codebook[1][1][0]+d_codebook[1][1][3]+d_codebook[1][2][5];
            d_constellation[23] = d_codebook[1][1][0]+d_codebook[1][1][3]+d_codebook[1][3][5];

            d_constellation[24] = d_codebook[1][1][0]+d_codebook[1][2][3]+d_codebook[1][0][5];
            d_constellation[25] = d_codebook[1][1][0]+d_codebook[1][2][3]+d_codebook[1][1][5];
            d_constellation[26] = d_codebook[1][1][0]+d_codebook[1][2][3]+d_codebook[1][2][5];
            d_constellation[27] = d_codebook[1][1][0]+d_codebook[1][2][3]+d_codebook[1][3][5];

            d_constellation[28] = d_codebook[1][1][0]+d_codebook[1][3][3]+d_codebook[1][0][5];
            d_constellation[29] = d_codebook[1][1][0]+d_codebook[1][3][3]+d_codebook[1][1][5];
            d_constellation[30] = d_codebook[1][1][0]+d_codebook[1][3][3]+d_codebook[1][2][5];
            d_constellation[31] = d_codebook[1][1][0]+d_codebook[1][3][3]+d_codebook[1][3][5];

            //codeword 2
            d_constellation[32] = d_codebook[1][2][0]+d_codebook[1][0][3]+d_codebook[1][0][5];
            d_constellation[33] = d_codebook[1][2][0]+d_codebook[1][0][3]+d_codebook[1][1][5];
            d_constellation[34] = d_codebook[1][2][0]+d_codebook[1][0][3]+d_codebook[1][2][5];
            d_constellation[35] = d_codebook[1][2][0]+d_codebook[1][0][3]+d_codebook[1][3][5];

            d_constellation[36] = d_codebook[1][2][0]+d_codebook[1][1][3]+d_codebook[1][0][5];
            d_constellation[37] = d_codebook[1][2][0]+d_codebook[1][1][3]+d_codebook[1][1][5];
            d_constellation[38] = d_codebook[1][2][0]+d_codebook[1][1][3]+d_codebook[1][2][5];
            d_constellation[39] = d_codebook[1][2][0]+d_codebook[1][1][3]+d_codebook[1][3][5];

            d_constellation[40] = d_codebook[1][2][0]+d_codebook[1][2][3]+d_codebook[1][0][5];
            d_constellation[41] = d_codebook[1][2][0]+d_codebook[1][2][3]+d_codebook[1][1][5];
            d_constellation[42] = d_codebook[1][2][0]+d_codebook[1][2][3]+d_codebook[1][2][5];
            d_constellation[43] = d_codebook[1][2][0]+d_codebook[1][2][3]+d_codebook[1][3][5];

            d_constellation[44] = d_codebook[1][2][0]+d_codebook[1][3][3]+d_codebook[1][0][5];
            d_constellation[45] = d_codebook[1][2][0]+d_codebook[1][3][3]+d_codebook[1][1][5];
            d_constellation[46] = d_codebook[1][2][0]+d_codebook[1][3][3]+d_codebook[1][2][5];
            d_constellation[47] = d_codebook[1][2][0]+d_codebook[1][3][3]+d_codebook[1][3][5];
            //codeword 3
            d_constellation[48] = d_codebook[1][3][0]+d_codebook[1][0][3]+d_codebook[1][0][5];
            d_constellation[49] = d_codebook[1][3][0]+d_codebook[1][0][3]+d_codebook[1][1][5];
            d_constellation[50] = d_codebook[1][3][0]+d_codebook[1][0][3]+d_codebook[1][2][5];
            d_constellation[51] = d_codebook[1][3][0]+d_codebook[1][0][3]+d_codebook[1][3][5];

            d_constellation[52] = d_codebook[1][3][0]+d_codebook[1][1][3]+d_codebook[1][0][5];
            d_constellation[53] = d_codebook[1][3][0]+d_codebook[1][1][3]+d_codebook[1][1][5];
            d_constellation[54] = d_codebook[1][3][0]+d_codebook[1][1][3]+d_codebook[1][2][5];
            d_constellation[55] = d_codebook[1][3][0]+d_codebook[1][1][3]+d_codebook[1][3][5];

            d_constellation[56] = d_codebook[1][3][0]+d_codebook[1][2][3]+d_codebook[1][0][5];
            d_constellation[57] = d_codebook[1][3][0]+d_codebook[1][2][3]+d_codebook[1][1][5];
            d_constellation[58] = d_codebook[1][3][0]+d_codebook[1][2][3]+d_codebook[1][2][5];
            d_constellation[59] = d_codebook[1][3][0]+d_codebook[1][2][3]+d_codebook[1][3][5];

            d_constellation[60] = d_codebook[1][3][0]+d_codebook[1][3][3]+d_codebook[1][0][5];
            d_constellation[61] = d_codebook[1][3][0]+d_codebook[1][3][3]+d_codebook[1][1][5];
            d_constellation[62] = d_codebook[1][3][0]+d_codebook[1][3][3]+d_codebook[1][2][5];
            d_constellation[63] = d_codebook[1][3][0]+d_codebook[1][3][3]+d_codebook[1][3][5];


            break;
          };
          case 2:{
            d_constellation[0] = d_codebook[2][0][1]+d_codebook[2][0][3]+d_codebook[2][0][5];
            d_constellation[1] = d_codebook[2][0][1]+d_codebook[2][0][3]+d_codebook[2][1][5];
            d_constellation[2] = d_codebook[2][0][1]+d_codebook[2][0][3]+d_codebook[2][2][5];
            d_constellation[3] = d_codebook[2][0][1]+d_codebook[2][0][3]+d_codebook[2][3][5];

            d_constellation[4] = d_codebook[2][0][1]+d_codebook[2][1][3]+d_codebook[2][0][5];
            d_constellation[5] = d_codebook[2][0][1]+d_codebook[2][1][3]+d_codebook[2][1][5];
            d_constellation[6] = d_codebook[2][0][1]+d_codebook[2][1][3]+d_codebook[2][2][5];
            d_constellation[7] = d_codebook[2][0][1]+d_codebook[2][1][3]+d_codebook[2][3][5];

            d_constellation[8] = d_codebook[2][0][1]+d_codebook[2][2][3]+d_codebook[2][0][5];
            d_constellation[9] = d_codebook[2][0][1]+d_codebook[2][2][3]+d_codebook[2][1][5];
            d_constellation[10] = d_codebook[2][0][1]+d_codebook[2][2][3]+d_codebook[2][2][5];
            d_constellation[11] = d_codebook[2][0][1]+d_codebook[2][2][3]+d_codebook[2][3][5];

            d_constellation[12] = d_codebook[2][0][1]+d_codebook[2][3][3]+d_codebook[2][0][5];
            d_constellation[13] = d_codebook[2][0][1]+d_codebook[2][3][3]+d_codebook[2][1][5];
            d_constellation[14] = d_codebook[2][0][1]+d_codebook[2][3][3]+d_codebook[2][2][5];
            d_constellation[15] = d_codebook[2][0][1]+d_codebook[2][3][3]+d_codebook[2][3][5];

            //codeword 1
            d_constellation[16] = d_codebook[2][1][1]+d_codebook[2][0][3]+d_codebook[2][0][5];
            d_constellation[17] = d_codebook[2][1][1]+d_codebook[2][0][3]+d_codebook[2][1][5];
            d_constellation[18] = d_codebook[2][1][1]+d_codebook[2][0][3]+d_codebook[2][2][5];
            d_constellation[19] = d_codebook[2][1][1]+d_codebook[2][0][3]+d_codebook[2][3][5];

            d_constellation[20] = d_codebook[2][1][1]+d_codebook[2][1][3]+d_codebook[2][0][5];
            d_constellation[21] = d_codebook[2][1][1]+d_codebook[2][1][3]+d_codebook[2][1][5];
            d_constellation[22] = d_codebook[2][1][1]+d_codebook[2][1][3]+d_codebook[2][2][5];
            d_constellation[23] = d_codebook[2][1][1]+d_codebook[2][1][3]+d_codebook[2][3][5];

            d_constellation[24] = d_codebook[2][1][1]+d_codebook[2][2][3]+d_codebook[2][0][5];
            d_constellation[25] = d_codebook[2][1][1]+d_codebook[2][2][3]+d_codebook[2][1][5];
            d_constellation[26] = d_codebook[2][1][1]+d_codebook[2][2][3]+d_codebook[2][2][5];
            d_constellation[27] = d_codebook[2][1][1]+d_codebook[2][2][3]+d_codebook[2][3][5];

            d_constellation[28] = d_codebook[2][1][1]+d_codebook[2][3][3]+d_codebook[2][0][5];
            d_constellation[29] = d_codebook[2][1][1]+d_codebook[2][3][3]+d_codebook[2][1][5];
            d_constellation[30] = d_codebook[2][1][1]+d_codebook[2][3][3]+d_codebook[2][2][5];
            d_constellation[31] = d_codebook[2][1][1]+d_codebook[2][3][3]+d_codebook[2][3][5];
            //codeword 2
            d_constellation[32] = d_codebook[2][2][1]+d_codebook[2][0][3]+d_codebook[2][0][5];
            d_constellation[33] = d_codebook[2][2][1]+d_codebook[2][0][3]+d_codebook[2][1][5];
            d_constellation[34] = d_codebook[2][2][1]+d_codebook[2][0][3]+d_codebook[2][2][5];
            d_constellation[35] = d_codebook[2][2][1]+d_codebook[2][0][3]+d_codebook[2][3][5];

            d_constellation[36] = d_codebook[2][2][1]+d_codebook[2][1][3]+d_codebook[2][0][5];
            d_constellation[37] = d_codebook[2][2][1]+d_codebook[2][1][3]+d_codebook[2][1][5];
            d_constellation[38] = d_codebook[2][2][1]+d_codebook[2][1][3]+d_codebook[2][2][5];
            d_constellation[39] = d_codebook[2][2][1]+d_codebook[2][1][3]+d_codebook[2][3][5];

            d_constellation[40] = d_codebook[2][2][1]+d_codebook[2][2][3]+d_codebook[2][0][5];
            d_constellation[41] = d_codebook[2][2][1]+d_codebook[2][2][3]+d_codebook[2][1][5];
            d_constellation[42] = d_codebook[2][2][1]+d_codebook[2][2][3]+d_codebook[2][2][5];
            d_constellation[43] = d_codebook[2][2][1]+d_codebook[2][2][3]+d_codebook[2][3][5];

            d_constellation[44] = d_codebook[2][2][1]+d_codebook[2][3][3]+d_codebook[2][0][5];
            d_constellation[45] = d_codebook[2][2][1]+d_codebook[2][3][3]+d_codebook[2][1][5];
            d_constellation[46] = d_codebook[2][2][1]+d_codebook[2][3][3]+d_codebook[2][2][5];
            d_constellation[47] = d_codebook[2][2][1]+d_codebook[2][3][3]+d_codebook[2][3][5];

            //codeword 3
            d_constellation[48] = d_codebook[2][3][1]+d_codebook[2][0][3]+d_codebook[2][0][5];
            d_constellation[49] = d_codebook[2][3][1]+d_codebook[2][0][3]+d_codebook[2][1][5];
            d_constellation[50] = d_codebook[2][3][1]+d_codebook[2][0][3]+d_codebook[2][2][5];
            d_constellation[51] = d_codebook[2][3][1]+d_codebook[2][0][3]+d_codebook[2][3][5];

            d_constellation[52] = d_codebook[2][3][1]+d_codebook[2][1][3]+d_codebook[2][0][5];
            d_constellation[53] = d_codebook[2][3][1]+d_codebook[2][1][3]+d_codebook[2][1][5];
            d_constellation[54] = d_codebook[2][3][1]+d_codebook[2][1][3]+d_codebook[2][2][5];
            d_constellation[55] = d_codebook[2][3][1]+d_codebook[2][1][3]+d_codebook[2][3][5];

            d_constellation[56] = d_codebook[2][3][1]+d_codebook[2][2][3]+d_codebook[2][0][5];
            d_constellation[57] = d_codebook[2][3][1]+d_codebook[2][2][3]+d_codebook[2][1][5];
            d_constellation[58] = d_codebook[2][3][1]+d_codebook[2][2][3]+d_codebook[2][2][5];
            d_constellation[59] = d_codebook[2][3][1]+d_codebook[2][2][3]+d_codebook[2][3][5];

            d_constellation[60] = d_codebook[2][3][1]+d_codebook[2][3][3]+d_codebook[2][0][5];
            d_constellation[61] = d_codebook[2][3][1]+d_codebook[2][3][3]+d_codebook[2][1][5];
            d_constellation[62] = d_codebook[2][3][1]+d_codebook[2][3][3]+d_codebook[2][2][5];
            d_constellation[63] = d_codebook[2][3][1]+d_codebook[2][3][3]+d_codebook[2][3][5];

            break;
          };
          case 3:{
            d_constellation[0] = d_codebook[3][0][0]+d_codebook[3][0][2]+d_codebook[3][0][4];
            d_constellation[1] = d_codebook[3][0][0]+d_codebook[3][0][2]+d_codebook[3][1][4];
            d_constellation[2] = d_codebook[3][0][0]+d_codebook[3][0][2]+d_codebook[3][2][4];
            d_constellation[3] = d_codebook[3][0][0]+d_codebook[3][0][2]+d_codebook[3][3][4];

            d_constellation[4] = d_codebook[3][0][0]+d_codebook[3][1][2]+d_codebook[3][0][4];
            d_constellation[5] = d_codebook[3][0][0]+d_codebook[3][1][2]+d_codebook[3][1][4];
            d_constellation[6] = d_codebook[3][0][0]+d_codebook[3][1][2]+d_codebook[3][2][4];
            d_constellation[7] = d_codebook[3][0][0]+d_codebook[3][1][2]+d_codebook[3][3][4];

            d_constellation[8] = d_codebook[3][0][0]+d_codebook[3][2][2]+d_codebook[3][0][4];
            d_constellation[9] = d_codebook[3][0][0]+d_codebook[3][2][2]+d_codebook[3][1][4];
            d_constellation[10] = d_codebook[3][0][0]+d_codebook[3][2][2]+d_codebook[3][2][4];
            d_constellation[11] = d_codebook[3][0][0]+d_codebook[3][2][2]+d_codebook[3][3][4];

            d_constellation[12] = d_codebook[3][0][0]+d_codebook[3][3][2]+d_codebook[3][0][4];
            d_constellation[13] = d_codebook[3][0][0]+d_codebook[3][3][2]+d_codebook[3][1][4];
            d_constellation[14] = d_codebook[3][0][0]+d_codebook[3][3][2]+d_codebook[3][2][4];
            d_constellation[15] = d_codebook[3][0][0]+d_codebook[3][3][2]+d_codebook[3][3][4];

            //codeword 1
            d_constellation[16] = d_codebook[0][1][0]+d_codebook[3][0][3]+d_codebook[3][0][4];
            d_constellation[17] = d_codebook[0][1][0]+d_codebook[3][0][3]+d_codebook[3][1][4];
            d_constellation[18] = d_codebook[0][1][0]+d_codebook[3][0][3]+d_codebook[3][2][4];
            d_constellation[19] = d_codebook[0][1][0]+d_codebook[3][0][3]+d_codebook[3][3][4];

            d_constellation[20] = d_codebook[0][1][0]+d_codebook[3][1][3]+d_codebook[3][0][4];
            d_constellation[21] = d_codebook[0][1][0]+d_codebook[3][1][3]+d_codebook[3][1][4];
            d_constellation[22] = d_codebook[0][1][0]+d_codebook[3][1][3]+d_codebook[3][2][4];
            d_constellation[23] = d_codebook[0][1][0]+d_codebook[3][1][3]+d_codebook[3][3][4];

            d_constellation[24] = d_codebook[0][1][0]+d_codebook[3][2][3]+d_codebook[3][0][4];
            d_constellation[25] = d_codebook[0][1][0]+d_codebook[3][2][3]+d_codebook[3][1][4];
            d_constellation[26] = d_codebook[0][1][0]+d_codebook[3][2][3]+d_codebook[3][2][4];
            d_constellation[27] = d_codebook[0][1][0]+d_codebook[3][2][3]+d_codebook[3][3][4];

            d_constellation[28] = d_codebook[0][1][0]+d_codebook[3][3][3]+d_codebook[3][0][4];
            d_constellation[29] = d_codebook[0][1][0]+d_codebook[3][3][3]+d_codebook[3][1][4];
            d_constellation[30] = d_codebook[0][1][0]+d_codebook[3][3][3]+d_codebook[3][2][4];
            d_constellation[31] = d_codebook[0][1][0]+d_codebook[3][3][3]+d_codebook[3][3][4];

            //codeword 2
            d_constellation[32] = d_codebook[0][2][0]+d_codebook[3][0][3]+d_codebook[3][0][4];
            d_constellation[33] = d_codebook[0][2][0]+d_codebook[3][0][3]+d_codebook[3][1][4];
            d_constellation[34] = d_codebook[0][2][0]+d_codebook[3][0][3]+d_codebook[3][2][4];
            d_constellation[35] = d_codebook[0][2][0]+d_codebook[3][0][3]+d_codebook[3][3][4];

            d_constellation[36] = d_codebook[3][2][0]+d_codebook[3][1][3]+d_codebook[3][0][4];
            d_constellation[37] = d_codebook[3][2][0]+d_codebook[3][1][3]+d_codebook[3][1][4];
            d_constellation[38] = d_codebook[3][2][0]+d_codebook[3][1][3]+d_codebook[3][2][4];
            d_constellation[39] = d_codebook[3][2][0]+d_codebook[3][1][3]+d_codebook[3][3][4];

            d_constellation[40] = d_codebook[3][2][0]+d_codebook[3][2][3]+d_codebook[3][0][4];
            d_constellation[41] = d_codebook[3][2][0]+d_codebook[3][2][3]+d_codebook[3][1][4];
            d_constellation[42] = d_codebook[3][2][0]+d_codebook[3][2][3]+d_codebook[3][2][4];
            d_constellation[43] = d_codebook[3][2][0]+d_codebook[3][2][3]+d_codebook[3][3][4];

            d_constellation[44] = d_codebook[3][2][0]+d_codebook[3][3][3]+d_codebook[3][0][4];
            d_constellation[45] = d_codebook[3][2][0]+d_codebook[3][3][3]+d_codebook[3][1][4];
            d_constellation[46] = d_codebook[3][2][0]+d_codebook[3][3][3]+d_codebook[3][2][4];
            d_constellation[47] = d_codebook[3][2][0]+d_codebook[3][3][3]+d_codebook[3][3][4];

            //codeword 3
            d_constellation[48] = d_codebook[3][3][0]+d_codebook[3][0][3]+d_codebook[3][0][4];
            d_constellation[49] = d_codebook[3][3][0]+d_codebook[3][0][3]+d_codebook[3][1][4];
            d_constellation[50] = d_codebook[3][3][0]+d_codebook[3][0][3]+d_codebook[3][2][4];
            d_constellation[51] = d_codebook[3][3][0]+d_codebook[3][0][3]+d_codebook[3][3][4];

            d_constellation[52] = d_codebook[3][3][0]+d_codebook[3][1][3]+d_codebook[3][0][4];
            d_constellation[53] = d_codebook[3][3][0]+d_codebook[3][1][3]+d_codebook[3][1][4];
            d_constellation[54] = d_codebook[3][3][0]+d_codebook[3][1][3]+d_codebook[3][2][4];
            d_constellation[55] = d_codebook[3][3][0]+d_codebook[3][1][3]+d_codebook[3][3][4];

            d_constellation[56] = d_codebook[3][3][0]+d_codebook[3][2][3]+d_codebook[3][0][4];
            d_constellation[57] = d_codebook[3][3][0]+d_codebook[3][2][3]+d_codebook[3][1][4];
            d_constellation[58] = d_codebook[3][3][0]+d_codebook[3][2][3]+d_codebook[3][2][4];
            d_constellation[59] = d_codebook[3][3][0]+d_codebook[3][2][3]+d_codebook[3][3][4];

            d_constellation[60] = d_codebook[3][3][0]+d_codebook[3][3][3]+d_codebook[3][0][4];
            d_constellation[61] = d_codebook[3][3][0]+d_codebook[3][3][3]+d_codebook[3][1][4];
            d_constellation[62] = d_codebook[3][3][0]+d_codebook[3][3][3]+d_codebook[3][2][4];
            d_constellation[63] = d_codebook[3][3][0]+d_codebook[3][3][3]+d_codebook[3][3][4];

            break;
          };
          default:{throw std::invalid_argument("invalid argument");break; };

        }
      }
      unsigned int
      constellation_SCMA::decision_maker(const gr_complex *sample)
      {

      }
      constellation_SCMA::~constellation_SCMA()
      {

      }
      gr_complex
      constellation_SCMA::make_est(const gr_complex *sample,unsigned resource)
      {
        int  min_flag = 0;
        gr_complex ret;
        std::vector<float> distance(65);
        constellation_SCMA::constellation_init(resource);
        distance[0] = 0;
        for (size_t i = 0; i < d_constellation.size(); i++) {
          distance[1+i] = std::norm(*sample - d_constellation[i]);
          if (distance[0]>distance[i+1]) {
            min_flag = i+1;
          }
        }
        ret = d_constellation[min_flag-1];

      }

  } /* namespace SCMA */
} /* namespace gr */
