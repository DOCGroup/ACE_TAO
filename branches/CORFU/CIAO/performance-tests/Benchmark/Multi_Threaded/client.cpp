//
// $Id$
//

//==============================================================
/**
 * @file client.cpp
 *
 * This is a client program that also acts as an Event Trigger. Four client tasks simultaneously
 * send out trigger to the ClientRoundTrip component to start the latency measurements in
 * parallel. This test is used to test scalability as the number of client components increase
 *
 * @author Arvind S. Krishna <arvindk@dre.vanderbilt.edu>
 */
//===============================================================

#include "Client_Task.h"

//IOR of the components
const char *ior1 = "file://comp1.ior";
const char *ior2 = "file://comp2.ior";
const char *ior3 = "file://comp3.ior";
const char *ior4 = "file://comp4.ior";


int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {

      // Initialize orb
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv);

      // Resolve HomeFinder interface
      CORBA::Object_var obj1
        = orb->string_to_object (ior1);

      CORBA::Object_var obj2
        = orb->string_to_object (ior2);

      CORBA::Object_var obj3
        = orb->string_to_object (ior3);

      CORBA::Object_var obj4
        = orb->string_to_object (ior4);

      if (CORBA::is_nil (obj1.in ()) ||
          CORBA::is_nil (obj2.in ()) ||
          CORBA::is_nil (obj3.in ()) ||
          CORBA::is_nil (obj4.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil Benchmark::RoundtripClient reference \n"),
                            1);
        }

      //Narrow to appropriate interfaces
      Benchmark::RoundTripClient_var client1=
        Benchmark::RoundTripClient::_narrow (obj1.in());

      Benchmark::RoundTripClient_var client2=
        Benchmark::RoundTripClient::_narrow (obj1.in());

      Benchmark::RoundTripClient_var client3=
        Benchmark::RoundTripClient::_narrow (obj1.in());

      Benchmark::RoundTripClient_var client4=
        Benchmark::RoundTripClient::_narrow (obj1.in());

      //Create Tasks
      Client_Task task1(client1.in());
      Client_Task task2(client2.in());
      Client_Task task3(client3.in());
      Client_Task task4(client4.in());

      task1.activate(THR_NEW_LWP | THR_JOINABLE);
      task2.activate(THR_NEW_LWP | THR_JOINABLE);
      task3.activate(THR_NEW_LWP | THR_JOINABLE);
      task4.activate(THR_NEW_LWP | THR_JOINABLE);

      task1.thr_mgr()->wait();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception Caught:");
      return 1;
    }
  return 0;
}
