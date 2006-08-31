// $Id$

#include "cuts/Benchmark_Agent_i.h"

#if !defined (__CUTS_INLINE__)
#include "cuts/Benchmark_Agent_i.inl"
#endif

#include "cuts/PortAgent.h"
#include "cuts/Port_Measurement.h"
#include "cuts/ActivationRecord.h"
#include "cuts/IDL_Streams.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_time.h"
#include <algorithm>

//
// Benchmark_Agent_i
//
Benchmark_Agent_i::Benchmark_Agent_i (void)
{

}

//
// Benchmark_Agent_i
//
Benchmark_Agent_i::~Benchmark_Agent_i (void)
{

}

//
// performance_data
//
::CUTS::Benchmark_Data *
Benchmark_Agent_i::collect_performance_data (
  ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Create a new data collection bundle.
  CUTS::Benchmark_Data_var benchmark_data;

  ACE_NEW_THROW_EX (benchmark_data,
                    ::CUTS::Benchmark_Data,
                    ::CORBA::NO_MEMORY ());

  // Initialize the <benchmark_data> object with informantion
  // from this compenent, e.g., ID.
  benchmark_data->owner = this->parent ();
  benchmark_data->timestamp = ACE_OS::gettimeofday ().msec ();

  // Collect the data from the agents and return the structure
  // to the client.
  benchmark_data->ports << this->port_agents ();

  return benchmark_data._retn ();
}
