#include "params.hh"

ROA_Parameters* ROA_Parameters::_instance = 0;

ROA_Parameters::ROA_Parameters()
  : using_threads_(0),
    thread_flags_(THR_NEW_LWP),
    context_p_(0),
    upcall_(0),
    forwarder_(0),
    oa_(0)
{
}

ROA_Parameters*
ROA_Parameters::instance()
{
  if (_instance == 0)
    {
      _instance = new ROA_Parameters;
    }
  return _instance;
}

#if !defined(__ACE_INLINE__)
#  include "params.i"
#endif
