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

#include <SCMA/header_packet_SCMA.h>
#include <gnuradio/digital/packet_header_ofdm.h>
#include <gnuradio/digital/lfsr.h>

namespace gr {
  namespace SCMA {

    int _get_header_len_from_occupied_carriers_SCMA(const std::vector<std::vector<int> > &occupied_carriers, int n_syms)
    {
      int header_len = 0;
      for (int i = 0; i < n_syms; i++) {
	       header_len += occupied_carriers[i].size();
      }

      return header_len;
    }

    header_packet_SCMA::sptr
    header_packet_SCMA::make(
		      const std::vector<std::vector<int> > &occupied_carriers,
		      int n_syms,
		      const std::string &len_tag_key,
		      const std::string &frame_len_tag_key,
		      const std::string &num_tag_key,
		      int bits_per_header_sym,
		      int bits_per_payload_sym,
		      bool scramble_header)
    {
      return header_packet_SCMA::sptr(
      	  new header_packet_SCMA(
      	    occupied_carriers, n_syms, len_tag_key, frame_len_tag_key, num_tag_key,
      	    bits_per_header_sym, bits_per_payload_sym, scramble_header
      	  )
      );
    }

    header_packet_SCMA::header_packet_SCMA(
		    const std::vector<std::vector<int> > &occupied_carriers,
		    int n_syms,
		    const std::string &len_tag_key,
		    const std::string &frame_len_tag_key,
		    const std::string &num_tag_key,
		    int bits_per_header_sym,
		    int bits_per_payload_sym,
		    bool scramble_header
    ) : gr::digital::packet_header_default(
	  _get_header_len_from_occupied_carriers_SCMA(occupied_carriers, n_syms),
	  len_tag_key,
	  num_tag_key,
	  bits_per_header_sym),
      d_frame_len_tag_key(pmt::string_to_symbol(frame_len_tag_key)),
      d_occupied_carriers(occupied_carriers),
      d_bits_per_payload_sym(bits_per_payload_sym),
      d_scramble_mask(d_header_len, 0)
    {
      // Init scrambler mask
      if (scramble_header) {
      	// These are just random values which already have OK PAPR:
      	gr::digital::lfsr shift_reg(0x8a, 0x6f, 7);
      	for (int i = 0; i < d_header_len; i++) {
      	  for (int k = 0; k < bits_per_header_sym; k++) {
      	    d_scramble_mask[i] ^= shift_reg.next_bit() << k;
      	  }
      	}
      }
    }

    header_packet_SCMA::~header_packet_SCMA()
    {
    }

    bool header_packet_SCMA::header_formatter(long packet_len, unsigned char *out, const std::vector<tag_t> &tags)
    {
		long packet_length;
		packet_length = packet_len *4;
      bool ret_val = gr::digital::packet_header_default::header_formatter(packet_length, out, tags);
      for (int i = 0; i < d_header_len; i++) {
      	out[i] ^= d_scramble_mask[i];
      }
      return ret_val;
    }

    bool header_packet_SCMA::header_parser(
        const unsigned char *in,
        std::vector<tag_t> &tags
    ) {
      std::vector<unsigned char> in_descrambled(d_header_len, 0);
      for (int i = 0; i < d_header_len; i++) {
        in_descrambled[i] = in[i] ^ d_scramble_mask[i];
      }
      if (!gr::digital::packet_header_default::header_parser(&in_descrambled[0], tags)) {
        return false;
      }
      int packet_len = 0; // # of complex symbols in this frame
      for (size_t i = 0; i < tags.size(); i++) {
        if (pmt::equal(tags[i].key, d_len_tag_key)) {
          // Convert bytes to complex symbols:
          packet_len = pmt::to_long(tags[i].value) * 8 / d_bits_per_payload_sym;
          if (pmt::to_long(tags[i].value) * 8 % d_bits_per_payload_sym) {
            packet_len++;
          }
          tags[i].value = pmt::from_long(packet_len);
          break;
        }
      }

      // To figure out how many payload OFDM symbols there are in this frame,
      // we need to go through the carrier allocation and count the number of
      // allocated carriers per OFDM symbol.
      // frame_len == # of payload OFDM symbols in this frame
      int frame_len = 0;
      size_t k = 0; // position in the carrier allocation map
      int symbols_accounted_for = 0;
      while (symbols_accounted_for < packet_len) {
        frame_len++;
        symbols_accounted_for += d_occupied_carriers[k].size();
        k = (k + 1) % d_occupied_carriers.size();
      }
      tag_t tag;
      tag.key = d_frame_len_tag_key;
      tag.value = pmt::from_long(frame_len);
      tags.push_back(tag);

      return true;
    }

  } /* namespace digital */
} /* namespace gr */
