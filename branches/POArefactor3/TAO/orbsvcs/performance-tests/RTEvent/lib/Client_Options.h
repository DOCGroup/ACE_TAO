/**
 * @file Client_Options.h
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 *
 */

#ifndef TAO_RTEC_PERF_CLIENT_OPTIONS_H
#define TAO_RTEC_PERF_CLIENT_OPTIONS_H
#include /**/ "ace/pre.h"

#include "rtec_perf_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Arg_Shifter;

/// Parse the command-line client_options for the tests.
/**
 * Note that some of the tests ignore some of the client_options, but it is
 * easier to have them all in a single class.
 */
class TAO_RTEC_Perf_Export Client_Options
{
public:
  /// Constructor
  Client_Options (int &argc, char *argv[]);

  const char *ior;
  int iterations;
  int nthreads;
  int high_priority_period;
  int high_priority_workload;
  int low_priority_period;
  int low_priority_workload;
  int low_priority_consumers;

  int dump_history;
  int use_rt_corba;
  int global_low_priority_rate;
  int unique_low_priority_event;
  int funky_supplier_publication;
  int high_priority_is_last;

private:
  int option (ACE_Arg_Shifter &arg_shifter,
              const char *option_name,
              int &option_value);

  int boolean_option (ACE_Arg_Shifter &arg_shifter,
                      const char *option_name,
                      int &option_value);
};

#include /**/ "ace/post.h"
#endif /* TAO_RTEC_PERF_CLIENT_OPTIONS_H */
