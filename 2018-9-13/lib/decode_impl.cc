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
//#define DEBUG_MODE
#include <gnuradio/io_signature.h>
#include "decode_impl.h"

namespace gr {
namespace SCMA {

decode::sptr
decode::make(unsigned char user_codebook,const std::string &len_tag_key)
{
        return gnuradio::get_initial_sptr
                       (new decode_impl(user_codebook, len_tag_key));
}
/*********************************************************************/

/*
 * The private constructor
 */
decode_impl::decode_impl(unsigned char user_codebook,const std::string &len_tag_key)
        : gr::tagged_stream_block("decode",
                                  gr::io_signature::make(1, 1, sizeof(gr_complex)),
                                  gr::io_signature::make(1, 1, sizeof(unsigned char)), len_tag_key),
        d_user_codebook(user_codebook)
{
        set_tag_propagation_policy(TPP_DONT);
        std::cout<<"#######decode constructor########"<<std::endl;
        printf("user_codebook is :   %x\n",d_user_codebook);
        //########################data structure init#########################
        codebookInit();
        std::cout<<"#########Decode Display#############################"<<std::endl;
        factorGraphInit();
        distanceSpaceAlloc();
        messageSpaceAlloc();
        noisePower = 1/(std::pow(10.0,2.1));
        //####################################################################
}
/*********************************************************************/
/*
 * Our virtual destructor.
 */
decode_impl::~decode_impl()
{
}
/*********************************************************************/
int
decode_impl::calculate_output_stream_length(const gr_vector_int &ninput_items)
{
        int noutput_items = ninput_items[0]/4;
        return noutput_items;
}

/*********************************************************************/


void
decode_impl::messageMatrixInit()
{
#ifdef DEBUG_MODE
        std::cout<<"#############mesageMatrixInit#######################"<<std::endl;
#endif
//inforG2V_matrix init
        for(int i = 0; i < NUM_RESOURCE; i++) {
                for(int j = 0; j< NUM_USER; j++ ) {
                        for(int k =0; k<NUM_CODE; k++) {
                                inforG2V[i][j][k] = 0.0;
                        }
                }
        }
//inforV2G_matrix init
        for(int i = 0; i<NUM_RESOURCE; i++) {
                for(int j = 0; j<NUM_USER; j++) {
                        for(int k = 0; k<NUM_CODE; k++) {
                                inforV2G[i][j][k] = std::log(AP);
                        }
                }
        }

}

/*********************************************************************/
void
decode_impl::factorGraphInit()
{
    #ifdef DEBUG_MODE
        //  initiate  factor_graph
        //  row represent resource
        //  col represent user
        std::cout<<"#############factorGraphInit#######################"<<std::endl;
    #endif
        for(int i = 0; i<NUM_RESOURCE; i++) {
                for(int j = 0; j<NUM_USER; j++) {
                        factor_graph[i][j] = 0;
                }
        }
        factor_graph[0][1] = 1;
        factor_graph[0][2] = 1;
        factor_graph[0][4] = 1;

        factor_graph[1][0] = 1;
        factor_graph[1][2] = 1;
        factor_graph[1][5] = 1;

        factor_graph[2][1] = 1;
        factor_graph[2][3] = 1;
        factor_graph[2][5] = 1;

        factor_graph[3][0] = 1;
        factor_graph[3][3] = 1;
        factor_graph[3][4] = 1;
        for(int i = 0; i<4; i++) {
                for(int j = 0; j<6; j++) {
                        printf("%x\t",factor_graph[i][j]);
                }
                printf("\n");
        }
}
/*********************************************************************/
void decode_impl::inforG2VUpdate()
{
//G2V update
#ifdef DEBUG_MODE
//        std::cout<<"#############inforG2VUpdate#######################"<<std::endl;
#endif
        for(int kk = 0; kk<NUM_RESOURCE; kk++) {
                //printf("test kkkkkkkkkkk         %x\n",kk);
                find(factor_graph,kk,false);
                //m1 Igv
                for(int m1 = 0; m1<NUM_CODE; m1++) {
                        for(int m2 = 0; m2<NUM_CODE; m2++) {
                                for(int m3 = 0; m3<NUM_CODE; m3++) {
                                        sIgv[m2*NUM_CODE+m3] = d_distance_matrix[m1][m2][m3][kk]+inforV2G[kk][d_index[1]][m2]+inforV2G[kk][d_index[2]][m3];

#ifdef DEBUG_MODE


    // std::cout<<"m1 igv  SIgv"<<m2*NUM_CODE+m3<<" :"
    // <<sIgv[m2*NUM_CODE+m3]<<"\t distance "<<m1<<m2<<m3<<kk
    // <<"\t :"<<d_distance_matrix[m1][m2][m3][kk]
    // <<"inforV2G"<<kk<<(unsigned int)d_index[1]<<m2<<" :"<< inforV2G[kk][d_index[1]][m2]
    //
    // <<"inforV2G"<<kk<<(unsigned int)d_index[2]<<m3<<" :"<< inforV2G[kk][d_index[2]][m3]
    // <<std::endl;

#endif
                                }
                        }
                        inforG2V[kk][d_index[0]][m1] = log_sum_exp(sIgv,numSIgv);

#ifdef DEBUG_MODE

		// std::cout<<"inforG2V "<<"["<<kk<<"]"<<"["<<(unsigned int)d_index[0]<<"]"<<"["<<m1<<"]"<<"   is"<<inforG2V[kk][(unsigned int)d_index[0]][m1] <<std::endl;

#endif
                }

                //m2 Igv
                for(int m2 = 0; m2<NUM_CODE; m2++) {
                        for(int m1 = 0; m1<NUM_CODE; m1++) {
                                for(int m3 = 0; m3<NUM_CODE; m3++) {
                                        sIgv[m1*NUM_CODE+m3] = d_distance_matrix[m1][m2][m3][kk]+inforV2G[kk][d_index[0]][m1]+inforV2G[kk][d_index[2]][m3];
#ifdef DEBUG_MODE
        //
  			// std::cout<<"m2 igv  SIgv"<<m1*NUM_CODE+m3<<" :"
  			// <<sIgv[m1*NUM_CODE+m3]<<"\t distance "<<m1<<m2<<m3<<kk
  			// <<"\t :"<<d_distance_matrix[m1][m2][m3][kk]
  			// <<"inforV2G"<<kk<<(unsigned int)d_index[0]<<m1<<" :"<< inforV2G[kk][d_index[0]][m1]
        //
  			// <<"inforV2G"<<kk<<(unsigned int)d_index[2]<<m3<<" :"<< inforV2G[kk][d_index[2]][m3]
  			// <<std::endl;

#endif
                                }
                        }
                        inforG2V[kk][d_index[1]][m2] = log_sum_exp(sIgv,numSIgv);

#ifdef DEBUG_MODE
//		std::cout<<"inforG2V "<<"["<<kk<<"]"<<"["<<(unsigned int)d_index[1]<<"]"<<"["<<m2<<"]"<<"   is"<<inforG2V[kk][(unsigned int)d_index[1]][m2] <<std::endl;
#endif
                }
                //m3 Igv
                for(int m3 = 0; m3<NUM_CODE; m3++) {
                        for(int m1 = 0; m1<NUM_CODE; m1++) {
                                for(int m2 = 0; m2<NUM_CODE; m2++) {
                                        sIgv[m1*NUM_CODE+m2] = d_distance_matrix[m1][m2][m3][kk]+inforV2G[kk][d_index[0]][m1]+inforV2G[kk][d_index[1]][m2];
#ifdef DEBUG_MODE

  			// std::cout<<"m3 igv  SIgv"<<m1*NUM_CODE+m2<<" :"
  			// <<sIgv[m1*NUM_CODE+m2]<<"\t distance "<<m1<<m2<<m3<<kk
  			// <<"\t :"<<d_distance_matrix[m1][m2][m3][kk]
  			// <<"inforV2G"<<kk<<(unsigned int)d_index[0]<<m1<<" :"<< inforV2G[kk][d_index[0]][m1]
        //
  			// <<"inforV2G"<<kk<<(unsigned int)d_index[2]<<m2<<" :"<< inforV2G[kk][d_index[1]][m2]
  			// <<std::endl;

#endif
                                }
                        }
                        inforG2V[kk][d_index[2]][m3] = log_sum_exp(sIgv,numSIgv);
#ifdef DEBUG_MODE
//		std::cout<<"inforG2V "<<"["<<kk<<"]"<<"["<<(unsigned int)d_index[2]<<"]"<<"["<<m3<<"]"<<"   is"<<inforG2V[kk][(unsigned int)d_index[2]][m3] <<std::endl;
#endif
                }

        }


#ifdef DEBUG_MODE
// display inforG2V
	// for(int q = 0;q<4;q++){
	//     for(int p = 0;p<6;p++){
  //   		for(int n = 0;n<4;n++){
  //     		std::cout<<"Information G2V"<<"["<<n<<" "<< p<<" "<<q <<" "<<"]is   :"<<inforG2V[n][p][q]<<std::endl;
  //     		}
  //     		printf("\n");
	//     }
	// }
  // exit(1);
#endif
}
/*********************************************************************/
void
decode_impl::inforV2GUpdate()
{
#ifdef DEBUG_MODE
//        std::cout<<"#############inforV2GUpdate#######################"<<std::endl;
#endif
//works
        for(int k = 0; k <NUM_USER; k++) {

                find(factor_graph,k,true);
                for(int m = 0; m<NUM_CODE; m++) {
                        float sum_exp_temp_1 = 0.0;
                        for(int i =0; i< NUM_CODE; i++) {
                                sum_exp_temp_1 = sum_exp_temp_1+std::exp(inforG2V[d_index[1]][k][i]);
#ifdef DEBUG_MODE
//			std::cout<<"inforG2V "<<d_index[1]<<k<<i<<"is "<<inforG2V[d_index[1]][k][i]<<std::endl;
//			std::cout<<"sum_exp_temp1 "<<i<<"time is "<<sum_exp_temp_1<<std::endl;
#endif
                        }
                        inforV2G[d_index[0]][k][m] = std::log(AP)+inforG2V[d_index[1]][k][m]-std::log(sum_exp_temp_1);
#ifdef DEBUG_MODE
//		std::cout<<"inforG2V "<<d_index[1]<<k<<m<<"is "<<inforG2V[d_index[1]][k][m]
//		    <<"log(sum_exp_temp_1) is"<<std::log(sum_exp_temp_1)
//		    <<"inforV2G"<<d_index[0]<<k<<m<<" is: "<<inforV2G[d_index[0]][k][m]<<std::endl;
#endif
                        float sum_exp_temp_2 = 0.0;
                        for(int i =0; i< NUM_CODE; i++) {
                                sum_exp_temp_2 =sum_exp_temp_2+std::exp(inforG2V[d_index[0]][k][i]);
#ifdef DEBUG_MODE
//			std::cout<<"inforG2V "<<d_index[0]<<k<<i<<"is "<<inforG2V[d_index[0]][k][i]<<std::endl;
//			std::cout<<"sum_exp_temp2 "<<i<<"time is "<<sum_exp_temp_2<<std::endl;
#endif
                        }
                        inforV2G[d_index[1]][k][m] = std::log(AP)+inforG2V[d_index[0]][k][m]-std::log(sum_exp_temp_2);
#ifdef DEBUG_MODE
//		std::cout<<"inforG2V "<<d_index[0]<<k<<m<<"is "<<inforG2V[d_index[0]][k][m]
//		    <<"log(sum_exp_temp_1) is"<<std::log(sum_exp_temp_1)
//		    <<"inforV2G"<<d_index[1]<<k<<m<<" is: "<<inforV2G[d_index[1]][k][m]<<std::endl;
#endif
                }
        }
#ifdef DEBUG_MODE
	// for(int i = 0; i<4;i++){
	//     for(int j =0;j<6;j++){
  //     		for(int k =0;k<4;k++){
  //     		    std::cout <<"inforV2G"<<k<<j<<i<<" is    "<<inforV2G[k][j][i]<<std::endl;
  //     		}
  //     		printf("\n");
	//     }
	// }
  // exit(1);
#endif
}
/*********************************************************************/
void
decode_impl::LLRCal()
{
//Q_matrix init
//Q_matrix
#ifdef DEBUG_MODE
        std::cout<<"#############LLRCal#######################"<<std::endl;
#endif
        for(int i = 0; i<NUM_USER; i++) {
                for(int j = 0; j<NUM_CODE; j++) {
                        Q_matrix[i][j] = 0.0;
                }
        }
//works
        for(int i = 0; i<NUM_USER; ++i) {
                find(factor_graph,i,true);
#ifdef DEBUG_MODE
//	    std::cout<<"d_index 0 is \t"<<(unsigned int)d_index[0]
//		<<"\td_index 1 is   "<<(unsigned int )d_index[1]<<std::endl;
#endif
                for(int j = 0; j<NUM_CODE; ++j) {
                        Q_matrix[i][j] = std::log(AP)+inforG2V[d_index[0]][i][j]+inforG2V[d_index[1]][i][j];
#ifdef DEBUG_MODE
		// std::cout<<"Q_matrix["<<i<<j<<"]"<<Q_matrix[i][j]<<std::endl;
#endif
                }
//	    printf("\n");
        }

        for( unsigned char i = 0; i<NUM_USER; i++) {
                LLR_Matrix[2*i] = Q_matrixCalulate(Q_matrix[i][0],Q_matrix[i][1],Q_matrix[i][2],Q_matrix[i][3]);
                LLR_Matrix[2*i+1] =Q_matrixCalulate(Q_matrix[i][0],Q_matrix[i][2],Q_matrix[i][1],Q_matrix[i][3]);
#ifdef DEBUG_MODE
  //  std::cout<<"LLR_Matrix "<<(unsigned int)2*i<<"\t"<<LLR_Matrix[2*i]<<"\t"
	// <<"LLR_Matrix "<<(unsigned int)2*i+1<<"\t"<<LLR_Matrix[2*i+1]<<std::endl;
#endif
        }
}

/*********************************************************************/
float decode_impl::Q_matrixCalulate(float Q1,float Q2, float Q3,float Q4)
{
//	std::cout<<"Q1:\t"<<Q1<<"\tQ2:\t"<<Q2<<"\tQ3:\t"<<Q3<<"\tQ4:\t"<<Q4<<std::endl;
        float result =0.0;
        result = (std::exp(Q1)+std::exp(Q2))/(std::exp(Q3)+std::exp(Q4));
//	std::cout<<"result is : "<<result<<"return : "<<std::log(result)<<std::endl;
        return std::log(result);
}
/*********************************************************************/
float
decode_impl::log_sum_exp(float srcMatrix[],unsigned char sumNum)
{
#ifdef DEBUG_MODE
//	std::cout<<"###########log_sum_exp function##############"<<std::endl;
#endif
        float max_num = srcMatrix[0];
        for(int i = 0; i<sumNum; i++) {
                max_num = std::max(max_num,srcMatrix[i]);
        }
        for(int i =0; i<sumNum; i++) {
                srcMatrix[i] =  srcMatrix[i]-max_num;
        }
        float sum = 0.0;
        for(int i =0; i<sumNum; i++) {
                sum = sum +std::exp(srcMatrix[i]);
        }

        return max_num+std::log(sum);

}
/*********************************************************************/
void
decode_impl::distanceInit(const gr_complex *single_sample)
{
//可能会因为没除噪声功率产生问题

#ifdef DEBUG_MODE
        std::cout<<"#############distanceInit#######################"<<std::endl;
#endif
        for(unsigned char k = 0; k<NUM_RESOURCE; k++) { //resource iterition
                find(factor_graph,k,false);

#ifdef DEBUG_MODE
                std::cout<<"sample"<<(unsigned int )k<<" is :"<< *(single_sample+k)<<std::endl;
                // std::cout << "d_index "<< (int)k <<" is " << (int)d_index[k]<<std::endl;
//	    printf("test#2  d_index is :  %x\n",d_index[2]);
#endif
                for(unsigned char m1 = 0; m1<NUM_CODE; m1++) {
                        for(unsigned char m2 = 0; m2<NUM_CODE; m2++) {
                                for(unsigned char m3 = 0; m3 <NUM_CODE; m3++) {
                                        d_distance_matrix[m1][m2][m3][k] =-(std::pow(std::abs(*(single_sample+k) -(d_codebook[k][m1][d_index[0]]+d_codebook[k][m2][d_index[1]]+d_codebook[k][m3][d_index[2]]) ),2) )/noisePower;
#ifdef DEBUG_MODE
//                 std::cout<<"sample "<<(int)k<<" is :"<< *(single_sample+k)<<std::endl;
// if (k ==1) {
//                 std::cout<<"sample"<<(unsigned int )k<<" is :"<< *(single_sample+k)<<std::endl;
//                 std::cout<<"distance matrix "<<(unsigned int)m3<<(unsigned int)m2<<(unsigned int)m1<<(unsigned int)k
//                 <<" is :   "<<d_distance_matrix[m3][m2][m1][k]<<std::endl;
//                 std::cout << "d_codebook["<<(int)k<<"]["<<(int)m1<<"]["<<(int)d_index[0]<<"] is :" << d_codebook[k][m1][d_index[0]]<<std::endl;
//                 std::cout << "d_codebook["<<(int)k<<"]["<<(int)m2<<"]["<<(int)d_index[1]<<"] is :" << d_codebook[k][m2][d_index[1]]<<std::endl;
//                 std::cout << "d_codebook["<<(int)k<<"]["<<(int)m3<<"]["<<(int)d_index[2]<<"] is :" << d_codebook[k][m3][d_index[2]]<<std::endl;
// }
#endif
                                }
                        }
                }
        }
#ifdef DEBUG_MODE
	// for(unsigned char  k = 0 ; k<NUM_RESOURCE ; k++){ //resource iterition
	//     for(unsigned char m1 = 0 ; m1<NUM_CODE; m1++){
  //     		for(unsigned char m2 = 0 ; m2<NUM_CODE;m2++){
  //     		    for(unsigned char m3 = 0;m3 <NUM_CODE;m3++){
  //           			std::cout<<"distance matrix "<<(unsigned int)m3<<(unsigned int)m2<<(unsigned int)m1<<(unsigned int)k
  //           			<<" is :   "<<d_distance_matrix[m3][m2][m1][k]<<std::endl;
  //           			}
  //           			printf("\n");
  //     		    }
  //     		}
	//     }
  //     exit(1);
#endif
}
/*********************************************************************/
void
decode_impl::messageSpaceAlloc()
{
//message G to V matrix allocation
        std::cout<<"#############messageSpaceAlloc#######################"<<std::endl;
        inforG2V.resize(NUM_RESOURCE);
        for(int i =0; i<NUM_RESOURCE; i++) {
                inforG2V[i].resize(NUM_USER);
                for(int j = 0; j<NUM_USER; j++) {
                        inforG2V[i][j].resize(NUM_CODE);
                }
        }
//sIgv init
        for(int iii = 0; iii<16; iii++) {
                sIgv[iii] = 0.0;
        }

// message V to G matrix allocation
        inforV2G.resize(NUM_RESOURCE);
        for(int i = 0; i<NUM_RESOURCE; i++) {
                inforV2G[i].resize(NUM_USER);
                for(int j = 0; j<NUM_USER; j++) {
                        inforV2G[i][j].resize(NUM_CODE);
                }
        }

}
/*********************************************************************/
void
decode_impl::distanceSpaceAlloc()
{
        std::cout<<"#############distanceSpaceAlloc#######################"<<std::endl;
        d_distance_matrix.resize(NUM_CODE);
        for(int i = 0; i<NUM_CODE; i++) {
                d_distance_matrix[i].resize(NUM_CODE);
                for(int j = 0; j<NUM_CODE; j++ ) {
                        d_distance_matrix[i][j].resize(NUM_CODE);
                        for(int k = 0; k<NUM_CODE; k++) {
                                d_distance_matrix[i][j][k].resize(NUM_RESOURCE);
                        }
                }
        }
}
/*********************************************************************/
void
decode_impl::codebookInit()
{
        std::cout<<"#############codebookInit#######################"<<std::endl;
        d_codebook.resize(NUM_RESOURCE);
        for(int i = 0; i<NUM_RESOURCE; i++) {
                d_codebook[i].resize(NUM_CODE);
                for(int j = 0; j<NUM_CODE; j++) {
                        d_codebook[i][j].resize(NUM_USER);
                }
        }
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
/*********************************************************************/

//find function return index
//para 2 flag represent return resource index or user index
//true return resource index
//false return user index
void decode_impl::find(unsigned char factor_graph[NUM_RESOURCE][NUM_USER],unsigned char num,bool flag  )
{
//init d_index
        d_index.clear();
        if(flag) {
                for(int i= 0; i<NUM_RESOURCE; i++) {
// user num
                        if(factor_graph[i][num]) {
                                d_index.push_back(i);
                        }
                }

        }
        else {
                //resource num
                for(int i = 0; i<NUM_USER; i++) {
                        if(factor_graph[num][i]) {
                                d_index.push_back(i);
                        }
                }

        }

}
/*********************************************************************/
int
decode_impl::work (int noutput_items,
                   gr_vector_int &ninput_items,
                   gr_vector_const_void_star &input_items,
                   gr_vector_void_star &output_items)
{
        unsigned packet_length = ninput_items[0];
        const gr_complex *in = (const gr_complex *) input_items[0];
        unsigned char  *out = (unsigned char  *) output_items[0];
        for(unsigned i = 0; i<packet_length; i++) {
                distanceInit(&in[i*NUM_CODE]);//the distance of code A on resource
                messageMatrixInit();
                for(int Niter = 0; Niter <NUM_ITERATION; Niter++) {//iteration for scma
//			printf("$$$$$$Niter is   :%d\n",Niter);
                        inforG2VUpdate();
                        inforV2GUpdate();
                }
#ifdef DEBUG_MODE

// 	for(int q = 0;q<4;q++){
// 	    for(int p = 0;p<6;p++){
//     		for(int n = 0;n<4;n++){
//       		std::cout<<"Information G2V"<<"["<<n<<" "<< p<<" "<<q <<" "<<"]is   :"<<inforG2V[n][p][q]<<std::endl;
//       		}
//       		printf("\n");
// 	    }
// 	}
// #endif
//
// #ifdef DEBUG_MODE
// 	for(int q = 0; q<4;q++){
// 	    for(int p =0;p<6;p++){
//       		for(int n =0;n<4;n++){
//       		    std::cout <<"InformationV2G"<<"["<<n<<" "<< p<<" "<<q <<" "<<"is    "<<inforV2G[n][p][q]<<std::endl;
//       		}
//       		printf("\n");
// 	    }
// 	}

#endif
//		printf("iteration is completed \n");
                LLRCal();
                int temp = 0;
                int datadec = 0;

                if(LLR_Matrix[d_user_codebook*2]<=0) {
                        temp = 1;
                }
                if(LLR_Matrix[d_user_codebook*2+1]<=0) {
                        datadec = 1;
                }
                datadec = (temp <<1 )+datadec;

                out[i] = datadec;
  #ifdef DEBUG_MODE
                std::cout << "packet_length is "<<packet_length << '\n';
                std::cout<<"output["<<i<<"]"<<"is :"<<std::hex<<(unsigned int)(unsigned char)out[i]<<std::endl;
  #endif


        }
        std::vector<tag_t> tags;
        get_tags_in_range(tags,0,ninput_items[0],ninput_items[0]+packet_length);
        for (size_t k = 0; k < tags.size(); k++) {

                tags[k].offset = tags[k].offset/NUM_CODE;
        }
#ifdef DEBUG_MODE
        //####################################################################
        exit(1);
#endif
        return packet_length/4;
}

}   /* namespace SCMA */
} /* namespace gr */
