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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "");

      // Resolve HomeFinder interface
      CORBA::Object_var obj
        = orb->string_to_object (ior);
      Benchmark::RoundTripClient_var test =
        Benchmark::RoundTripClient::_narrow(obj.in());
      //Get the RoundTrip reference
      Benchmark::Controller_var trigger = test->provide_controller ();

      //Send a trigger to start the test
      trigger->start();

    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception Handled:");
      return 1;
    }


  return 0;
}
