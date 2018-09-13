%template(ofdm_equalizer_base_sptr) boost::shared_ptr<gr::digital::ofdm_equalizer_base>;
%template(ofdm_equalizer_1d_pilots_sptr) boost::shared_ptr<gr::digital::ofdm_equalizer_1d_pilots>;
%pythoncode %{
ofdm_equalizer_1d_pilots_sptr.__repr__ = lambda self: "<OFDM equalizer 1D base class>"
%}

%template(equalizer_SCMA_sptr) boost::shared_ptr<gr::SCMA::equalizer_SCMA>;
%pythoncode %{
equalizer_SCMA_sptr.__repr__ = lambda self: "< equalizer scma>"
equalizer_SCMA = equalizer_SCMA.make;
%}
