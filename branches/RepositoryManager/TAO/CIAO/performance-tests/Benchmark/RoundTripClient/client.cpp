// $Id$

//==============================================================
/**
 * @file client.cpp
 *
 * This is a simple client test program that interact with the RoundTrip
 * component implementation. The single threaded client issues
 * two-way operations, the total latency (response time) is measured
 * NOTE: this client implementation has been adapted from the
 * $TAO_ROOT/performance-results/Latency/Single_Threaded/
 *
 * @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//==============================================================
#include "RoundTripClientC.h"

const char *ior = "file://test.ior";

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            ""
                                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Resolve HomeFinder interface
      CORBA::Object_var obj
        = orb->string_to_object (ior ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      Benchmark::RoundTripClient_var test =
	      Benchmark::RoundTripClient::_narrow(obj.in());
      //Get the RoundTrip reference
      Benchmark::Controller_var trigger = test->provide_controller ();

      //Send a trigger to start the test
      trigger->start();

    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception Handled:");
      return 1;
    }

  ACE_ENDTRY;

  return 0;
}
