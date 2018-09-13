%template(constellation_sptr) boost::shared_ptr<gr::digital::constellation>;

%template(constellation_SCMA_sptr) boost::shared_ptr<gr::SCMA::constellation_SCMA>;
%pythoncode %{
constellation_SCMA_sptr.__repr__ = lambda self: "<constellation SCMA>"
constellation_SCMA = constellation_SCMA.make;
%}
