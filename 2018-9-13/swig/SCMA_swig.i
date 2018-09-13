/* -*- c++ -*- */

#define SCMA_API
#define  DIGITAL_API

%include "gnuradio.i"			// the common stuff

//load generated python docstrings
%include "SCMA_swig_doc.i"

%{
#include "SCMA/encode.h"
#include "SCMA/decode.h"
#include "SCMA/changeTag.h"
#include "SCMA/header_packet_SCMA.h"
#include "SCMA/constellation_SCMA.h"
#include "SCMA/equalizer_SCMA.h"
#include "SCMA/packet_test.h"
#include "SCMA/test1.h"
%}

%include "gnuradio/digital/packet_header_default.h"
%include "gnuradio/digital/constellation.h"
%include "gnuradio/digital/ofdm_equalizer_base.h"

%include "SCMA/encode.h"
GR_SWIG_BLOCK_MAGIC2(SCMA, encode);
%include "SCMA/decode.h"
GR_SWIG_BLOCK_MAGIC2(SCMA, decode);
%include "SCMA/changeTag.h"
GR_SWIG_BLOCK_MAGIC2(SCMA, changeTag);
%include "SCMA/header_packet_SCMA.h"
%include "SCMA/constellation_SCMA.h"
%include "SCMA/equalizer_SCMA.h"

%include "header_SCMA.i"
%include "constellation_SCMA.i"
%include "equalizer_SCMA.i"
%include "SCMA/packet_test.h"
%include "SCMA/test1.h"
%template(test1_sptr) boost::shared_ptr<gr::SCMA::test1>;
%pythoncode %{
test1_sptr.__repr__ = lambda self: "<test1>"
test1 = test1 .make;
%}
