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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Resolve HomeFinder interface
      CORBA::Object_var obj
        = orb->string_to_object (ior);
      Benchmark::RoundTripClient_var test =
        Benchmark::RoundTripClient::_narrow(obj.in());

      if (CORBA::is_nil (test.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire RoundTripClient objref\n"), -1);

      // Get the RoundTrip reference
      obj = test->provide_facet ("controller");
      Benchmark::Controller_var trigger =
        Benchmark::Controller::_narrow (obj.in ());

      if (CORBA::is_nil (trigger.in ()))
        ACE_ERROR_RETURN ((LM_ERROR, "Unable to acquire Controller objref\n"), -1);

      // Send a trigger to start the test
      trigger->start();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception Handled:");
      return 1;
    }


  return 0;
}
