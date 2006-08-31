// $Id$

namespace CUTS
{
  //
  // testing_service
  //
  CUTS_INLINE
  Testing_Service_exec_i *
  Benchmark_Data_Collector_Impl::testing_service (void)
  {
    return this->testing_service_.get ();
  }
}
