/* -*- c++ -*- */
/* Copyright 2012 Free Software Foundation, Inc.
 *
 * This file is part of GNU Radio
 *
 * GNU Radio is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 3, or (at your option)
 * any later version.
 *
 * GNU Radio is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with GNU Radio; see the file COPYING.  If not, write to
 * the Free Software Foundation, Inc., 51 Franklin Street,
 * Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <SCMA/equalizer_SCMA.h>
#include <gnuradio/digital/ofdm_equalizer_base.h>
#include <gnuradio/digital/constellation.h>
namespace gr {
  namespace  SCMA{

    equalizer_SCMA::sptr
    equalizer_SCMA::make(
	  int fft_len,
    const gr::SCMA::constellation_SCMA::sptr &constellation,
	  const std::vector<std::vector<int> > &occupied_carriers,
	  const std::vector<std::vector<int> > &pilot_carriers,
	  const std::vector<std::vector<gr_complex> > &pilot_symbols,
	  int symbols_skipped,
	  float alpha,
	  bool input_is_shifted)
    {
      return equalizer_SCMA::sptr(
    	  new equalizer_SCMA(
    	      fft_len,
    	      constellation,
    	      occupied_carriers,
    	      pilot_carriers,
    	      pilot_symbols,
    	      symbols_skipped,
    	      alpha,
    	      input_is_shifted
    	     )
      );
    }

    equalizer_SCMA::equalizer_SCMA(
      	int fft_len,
      	const gr::SCMA::constellation_SCMA::sptr &constellation,
      	const std::vector<std::vector<int> > &occupied_carriers,
      	const std::vector<std::vector<int> > &pilot_carriers,
      	const std::vector<std::vector<gr_complex> > &pilot_symbols,
      	int symbols_skipped,
      	float alpha,
      	bool input_is_shifted)
      :  gr::digital::ofdm_equalizer_1d_pilots(fft_len, occupied_carriers, pilot_carriers, pilot_symbols, symbols_skipped, input_is_shifted),
	  d_constellation(constellation),
	  d_alpha(alpha)
    {
    }


     equalizer_SCMA::~ equalizer_SCMA()
    {
    }


    void
     equalizer_SCMA::equalize(gr_complex *frame,
	      int n_sym,
	      const std::vector<gr_complex> &initial_taps,
	      const std::vector<tag_t> &tags)
    {
      if (!initial_taps.empty()) {
      	d_channel_state = initial_taps;
            }
      gr_complex sym_eq,sym_est;
      unsigned num_sym_valid = 0;
      for (int i = 0; i < n_sym; i++) {
      	for (int k = 0; k < d_fft_len; k++) {
      	  if (!d_occupied_carriers[k]) {
      	    continue;
	         }
      	  if (!d_pilot_carriers.empty() && d_pilot_carriers[d_pilot_carr_set][k]) {
      	       d_channel_state[k] = d_alpha * d_channel_state[k]+ (1-d_alpha) * frame[i*d_fft_len + k] / d_pilot_symbols[d_pilot_carr_set][k];
      	       frame[i*d_fft_len+k] = d_pilot_symbols[d_pilot_carr_set][k];
      	  } else {
            num_sym_valid++;
      	    sym_eq = frame[i*d_fft_len+k] / d_channel_state[k];
                  // The `map_to_points` function will treat `sym_est` as an array
                  // pointer.  This call is "safe" because `map_to_points` is limited
                  // by the dimensionality of the constellation. This class calls the
                  // `constellation` class default constructor, which initializes the
                  // dimensionality value to `1`. Thus, Only the single `gr_complex`
                  // value will be dereferenced.
      	   // d_constellation->map_to_points(d_constellation->decision_maker(&sym_eq), &sym_est);
      	    unsigned resource = (num_sym_valid-1)%4;
            sym_est = d_constellation->make_est(&sym_eq,resource);
      	    d_channel_state[k] = d_alpha * d_channel_state[k]+ (1-d_alpha) * frame[i*d_fft_len + k] / sym_est;
      	    frame[i*d_fft_len+k] = sym_est;

      	  }
	       }
        	if (!d_pilot_carriers.empty()) {
        	  d_pilot_carr_set = (d_pilot_carr_set + 1) % d_pilot_carriers.size();
        	}
      }
    }

  } /* namespace SCMA */
} /* namespace gr */
