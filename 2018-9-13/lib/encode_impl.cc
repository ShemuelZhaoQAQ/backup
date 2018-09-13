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
#include "encode_impl.h"
//#define DEBUG_MODE

namespace gr {
  namespace SCMA {

    encode::sptr
    encode::make(unsigned char user_codebook,unsigned packet_len ,const std::string &len_tag_key)
    {
      return gnuradio::get_initial_sptr
        (new encode_impl(user_codebook, packet_len, len_tag_key));
    }

    /*
     * The private constructor
     */
    encode_impl::encode_impl(unsigned char user_codebook,unsigned packet_len ,const std::string &len_tag_key)
      : gr::tagged_stream_block("encode",
              gr::io_signature::make(1, 1, sizeof(unsigned char)),
              gr::io_signature::make(1, 1, sizeof(gr_complex)), len_tag_key),
      	      d_user_codebook(user_codebook),
      	      d_len_tag_key(len_tag_key),
      	      d_packet_len(packet_len),
              d_packet_len_pmt(pmt::from_long(packet_len)),
              d_next_tag_pos(0)

    {
         set_tag_propagation_policy(TPP_DONT);
         d_codebook.resize(4);
         for (int i = 0; i < 4; ++i)
                d_codebook[i].resize(4);
         std::cout<<"##########encode construction########"<<std::endl;
         std::cout << "User is :"<<(int)d_user_codebook << '\n';

	}

        /******************************************************************************/
    bool encode_impl::isValidCodebook(){
                switch(d_user_codebook){
                        case 0x00: return true;
                        case 0x01: return true;
                        case 0x02: return true;
                        case 0x03: return true;
                        case 0x04: return true;
                        case 0x05: return true;
                        default :return false;
                        }
        }
	/******************************************************************************/
    void encode_impl::codebookMapping(){
    	switch(d_user_codebook){
    		case 0x00 :{
    		         d_codebook[0][0]=gr_complex(0,0);
          		   d_codebook[0][1]=gr_complex(0,0);
    		         d_codebook[0][2]=gr_complex(0,0);
          		   d_codebook[0][3]=gr_complex(0,0);

          		   d_codebook[1][0]=gr_complex(-0.1815,-0.1318);
          		   d_codebook[1][1]=gr_complex(-0.6351,-0.4615);
          		   d_codebook[1][2]=gr_complex(0.6351,0.4615);
          		   d_codebook[1][3]=gr_complex(0.1815,0.1318);

          		   d_codebook[2][0]=gr_complex(0,0);
          		   d_codebook[2][1]=gr_complex(0,0);
          		   d_codebook[2][2]=gr_complex(0,0);
          		   d_codebook[2][3]=gr_complex(0,0);

          		   d_codebook[3][0]=gr_complex(0.7851,0);
          		   d_codebook[3][1]=gr_complex(-0.2243,0);
          		   d_codebook[3][2]=gr_complex(0.2243,0);
          		   d_codebook[3][3]=gr_complex(-0.7851,0);


          		break; }
          		case 0x01 :{
          		   d_codebook[0][0]=gr_complex(0.7851,0);
          		   d_codebook[0][1]=gr_complex(-0.2243,0);
          		   d_codebook[0][2]=gr_complex(0.2243,0);
          		   d_codebook[0][3]=gr_complex(-0.7851,0);

          		   d_codebook[1][0]=gr_complex(0,0);
          		   d_codebook[1][1]=gr_complex(0,0);
          		   d_codebook[1][2]=gr_complex(0,0);
          		   d_codebook[1][3]=gr_complex(0,0);

          		   d_codebook[2][0]=gr_complex(-0.1815,-0.1318);
          		   d_codebook[2][1]=gr_complex(-0.6351,-0.4615);
          		   d_codebook[2][2]=gr_complex(0.6351,0.4615);
          		   d_codebook[2][3]=gr_complex(0.1815,0.1318);

          		   d_codebook[3][0]=gr_complex(0,0);
          		   d_codebook[3][1]=gr_complex(0,0);
          		   d_codebook[3][2]=gr_complex(0,0);
          		   d_codebook[3][3]=gr_complex(0,0);

          		break; }
          	        case 0x02 :{
          		   d_codebook[0][0]=gr_complex(-0.6351,0.4615);
          		   d_codebook[0][1]=gr_complex(0.1815,-0.1318);
          		   d_codebook[0][2]=gr_complex(-0.1815,0.1318);
          		   d_codebook[0][3]=gr_complex(0.6351,-0.4615);

          		   d_codebook[1][0]=gr_complex(0.1392,-0.1759);
          		   d_codebook[1][1]=gr_complex(0.4873,-0.6156);
          		   d_codebook[1][2]=gr_complex(-0.4873,0.6156);
          		   d_codebook[1][3]=gr_complex(-0.1392,0.1759);

          		   d_codebook[2][0]=gr_complex(0,0);
          		   d_codebook[2][1]=gr_complex(0,0);
          		   d_codebook[2][2]=gr_complex(0,0);
          		   d_codebook[2][3]=gr_complex(0,0);

          		   d_codebook[3][0]=gr_complex(0,0);
          		   d_codebook[3][1]=gr_complex(0,0);
          		   d_codebook[3][2]=gr_complex(0,0);
          		   d_codebook[3][3]=gr_complex(0,0);

          		break; }
          		case 0x03 :{
          		   d_codebook[0][0]=gr_complex(0,0);
          		   d_codebook[0][1]=gr_complex(0,0);
          		   d_codebook[0][2]=gr_complex(0,0);
          		   d_codebook[0][3]=gr_complex(0,0);

          		   d_codebook[1][0]=gr_complex(0,0);
          		   d_codebook[1][1]=gr_complex(0,0);
          		   d_codebook[1][2]=gr_complex(0,0);
          		   d_codebook[1][3]=gr_complex(0,0);

          		   d_codebook[2][0]=gr_complex(0.7851,0);
          		   d_codebook[2][1]=gr_complex(-0.2243,0);
          		   d_codebook[2][2]=gr_complex(0.2243,0);
          		   d_codebook[2][3]=gr_complex(-0.7851,0);

          		   d_codebook[3][0]=gr_complex(-0.0055,-0.2242);
          		   d_codebook[3][1]=gr_complex(-0.0193,-0.7848);
          		   d_codebook[3][2]=gr_complex(0.0193,0.7848);
          		   d_codebook[3][3]=gr_complex(0.0055,0.2242);

          		break; }
          		case 0x04 :{
          		   d_codebook[0][0]=gr_complex(-0.0055,-0.2242);
          		   d_codebook[0][1]=gr_complex(-0.0193,-0.7848);
          		   d_codebook[0][2]=gr_complex(0.0193,0.7848);
          		   d_codebook[0][3]=gr_complex(0.0055,0.2242);

          		   d_codebook[1][0]=gr_complex(0,0);
          		   d_codebook[1][1]=gr_complex(0,0);
          		   d_codebook[1][2]=gr_complex(0,0);
          		   d_codebook[1][3]=gr_complex(0,0);

          		   d_codebook[2][0]=gr_complex(0,0);
          		   d_codebook[2][1]=gr_complex(0,0);
          		   d_codebook[2][2]=gr_complex(0,0);
          		   d_codebook[2][3]=gr_complex(0,0);

          		   d_codebook[3][0]=gr_complex(-0.6351,0.4615);
          		   d_codebook[3][1]=gr_complex(0.1815,-0.1318);
          		   d_codebook[3][2]=gr_complex(-0.1815,0.1318);
          		   d_codebook[3][3]=gr_complex(0.6351,-0.4615);

          		break ; }
          		case 0x05 :{
          		   d_codebook[0][0]=gr_complex(0,0);
          		   d_codebook[0][1]=gr_complex(0,0);
          		   d_codebook[0][2]=gr_complex(0,0);
          		   d_codebook[0][3]=gr_complex(0,0);

          		   d_codebook[1][0]=gr_complex(0.7851,0);
          		   d_codebook[1][1]=gr_complex(-0.2243,0);
          		   d_codebook[1][2]=gr_complex(0.2243,0);
          		   d_codebook[1][3]=gr_complex(-0.7851,0);

          		   d_codebook[2][0]=gr_complex(0.1392,-0.1759);
          		   d_codebook[2][1]=gr_complex(0.4873,-0.6156);
          		   d_codebook[2][2]=gr_complex(-0.4873,0.6156);
          		   d_codebook[2][3]=gr_complex(-0.1392,0.1759);

          		   d_codebook[3][0]=gr_complex(0,0);
          		   d_codebook[3][1]=gr_complex(0,0);
          		   d_codebook[3][2]=gr_complex(0,0);
          		   d_codebook[3][3]=gr_complex(0,0);
          		break; }

		 }
    }
	/******************************************************************************/
    /*
     * Our virtual destructor.
     */
    encode_impl::~encode_impl()
    {
    }
        /******************************************************************************/

    int
    encode_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
    {
      int noutput_items = ninput_items[0]*4;
      return noutput_items ;
    }
        /******************************************************************************/
    int
    encode_impl::work (int noutput_items,
                       gr_vector_int &ninput_items,
                       gr_vector_const_void_star &input_items,
                       gr_vector_void_star &output_items)
    {
      size_t packet_length = ninput_items[0];
      size_t packet_read = nitems_read(0);
      codebookMapping();
      std::vector<tag_t> tags;
      #ifdef  DEBUG_MODE
      std::cout<<"################Display CodeBook##################"<<std::endl;
      for(int i = 0;i<4;i++){
    		for(int j = 0;j<4;j++){
    	    std::cout<<"codebook is"<<d_codebook[i][j]<<std::endl;
    		}
      }
      std::cout<<"##################################################"<<std::endl;
      std::cout << "packet_length is :\t" << packet_length<<std::endl;
      std::cout << "packet_read is :\t" <<packet_read <<std::endl;
      #endif
      //chunk_to_symbols
      unsigned int n_out = 0;
      int nstreams = input_items.size();
      for(int m = 0; m < nstreams; m++) {
          const unsigned char *in = (const unsigned char*)input_items[m];
          gr_complex *out = (gr_complex*)output_items[m];
          for(int i = 0; i < packet_length; i++) {
              assert(((unsigned int)in[i]) < 4);
    		  out[n_out+0] = d_codebook[0][in[i]];
    		  out[n_out+1] = d_codebook[1][in[i]];
    		  out[n_out+2] = d_codebook[2][in[i]];
    		  out[n_out+3] = d_codebook[3][in[i]];
        #ifdef DEBUG_MODE
    		  std::cout<<"out "<<n_out+0<< "is  :"<<out[n_out+0]<<std::endl;
    		  std::cout<<"out "<<n_out+1<< "is  :"<<out[n_out+1]<<std::endl;
    		  std::cout<<"out "<<n_out+2<< "is  :"<<out[n_out+2]<<std::endl;
    		  std::cout<<"out "<<n_out+3<< "is  :"<<out[n_out+3]<<std::endl;

         #endif
          n_out+=4;

    	}
	     get_tags_in_range(tags, 0, nitems_read(0),nitems_read(0)+packet_length );
       for (unsigned int j = 0; j < tags.size(); j++) {
              tags[j].offset = tags[j].offset*4;
           }

      }
     #ifdef DEBUG_MODE

      exit(1);
     #endif
      return n_out;
    }


  } /* namespace SCMA */
} /* namespace gr */
