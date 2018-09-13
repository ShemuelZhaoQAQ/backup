
%template(packet_header_default_sptr) boost::shared_ptr<gr::digital::packet_header_default>;
%pythoncode %{
packet_header_default_sptr.__repr__ = lambda self: "<packet_header_default>"
packet_header_default = packet_header_default .make;
%}
%template(header_packet_SCMA_sptr) boost::shared_ptr<gr::SCMA::header_packet_SCMA>;
%pythoncode %{
header_packet_SCMA_sptr.__repr__ = lambda self: "<header_packet_SCMA>"
header_packet_SCMA = header_packet_SCMA .make;
%}
