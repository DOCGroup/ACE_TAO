/**
 * @file Client_Options.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Client_Options.h"

#include "ace/Arg_Shifter.h"
#include "ace/Log_Msg.h"

ACE_RCSID(TAO_PERF_RTEC, Client_Options, "$Id$")

Client_Options::Client_Options (int &argc, char *argv[])
  : ior ("file://test.ior")
  , iterations (1000)
  , nthreads (0)
  , high_priority_period (0)
  , high_priority_workload (0)
  , low_priority_period (0)
  , low_priority_workload (0)
  , low_priority_consumers (0)
  , dump_history (0)
  , use_rt_corba (0)
  , global_low_priority_rate (0)
  , unique_low_priority_event (0)
  , funky_supplier_publication (0)
  , high_priority_is_last (0)
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcmp (arg, "-k") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              this->ior = arg_shifter.get_current ();
              arg_shifter.consume_arg ();
            }
        }

      else if (option (arg_shifter, "-i", iterations)) {}
      else if (option (arg_shifter, "-n", nthreads)) {}
      else if (option (arg_shifter, "-h", high_priority_period)) {}
      else if (option (arg_shifter, "-l", low_priority_period)) {}
      else if (option (arg_shifter, "-w", high_priority_workload)) {}
      else if (option (arg_shifter, "-v", low_priority_workload)) {}
      else if (option (arg_shifter, "-c", low_priority_consumers)) {}

      else if (boolean_option (arg_shifter, "-d", dump_history)) {}
      else if (boolean_option (arg_shifter, "-r", use_rt_corba)) {}
      else if (boolean_option (arg_shifter, "-g", global_low_priority_rate)) {}
      else if (boolean_option (arg_shifter, "-u", unique_low_priority_event)) {}
      else if (boolean_option (arg_shifter, "-f", funky_supplier_publication)) {}
      else if (boolean_option (arg_shifter, "-x", high_priority_is_last)) {}

      else
        {
          arg_shifter.ignore_arg ();
        }
    }
}

int
Client_Options::option (ACE_Arg_Shifter &arg_shifter,
                 const char *option_name,
                 int &option_value)
{
  if (ACE_OS::strcmp (arg_shifter.get_current (), option_name) != 0)
    return 0;
  arg_shifter.consume_arg ();
  if (arg_shifter.is_parameter_next ())
    {
      option_value = ACE_OS::atoi (arg_shifter.get_current ());
      arg_shifter.consume_arg ();
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Missing value for option '%s'\n", option_name));
    }
  return 1;
}

int
Client_Options::boolean_option (ACE_Arg_Shifter &arg_shifter,
                         const char *option_name,
                         int &option_value)
{
  if (ACE_OS::strcmp (arg_shifter.get_current (), option_name) != 0)
    return 0;
  arg_shifter.consume_arg ();
  option_value = 1;
  return 1;
}
