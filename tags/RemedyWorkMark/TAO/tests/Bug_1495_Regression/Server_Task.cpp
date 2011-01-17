/**
 * @file Server_Task.cpp
 * @author Will Otte <wotte@dre.vanderbilt.edu>
 *
 * $Id$
 *
 * Implements the Server_Task class which acts as the process colocated
 * corba server for Bug_1495_Regression test.
 */

#include "Server_Task.h"
#include "ace/OS_NS_unistd.h"
#include "test_i.h"

#include "ace/Manual_Event.h"


Server_Task::Server_Task (const ACE_TCHAR *output,
                          CORBA::ORB_ptr sorb,
                          ACE_Manual_Event &me,
                          ACE_Thread_Manager *thr_mgr)
  : ACE_Task_Base (thr_mgr),
    output_ (output),
    me_ (me),
    sorb_ (CORBA::ORB::_duplicate (sorb))
{
}


int
Server_Task::svc (void)
{
  try
    {
      CORBA::Object_var poa_object =
        sorb_->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        {
          ACE_ERROR ((LM_ERROR,
                      " (%P|%t) Unable to initialize the POA\n"));
          return 1;
        }

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      Bug1495_i *server_impl = 0;
      ACE_NEW_RETURN (server_impl,
                      Bug1495_i (sorb_.in ()),
                      0);
      PortableServer::ServantBase_var owner_transfer (server_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (server_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Bug1495_Regression::Bug1495_var bug1495 =
        Bug1495_Regression::Bug1495::_narrow (object.in ());

      CORBA::String_var ior = sorb_->object_to_string (bug1495.in ());

      if (output_ != 0)
        {
          FILE *output_file = ACE_OS::fopen (output_, "w");

          if (output_file == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          "Cannot open output file for writing the "
                          "thread server IOR: %s", output_));
              return 1;
            }

          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      // sleep for a few seconds and hope the remote server picks up the
      // ior.
      ACE_OS::sleep (5);

      // Signal the manual event to wake the main thread up.
      me_.signal ();

      // The ORB will run for 15 seconds and shut down.
      ACE_Time_Value tv (15, 0);
      sorb_->run (tv);

      ACE_DEBUG ((LM_DEBUG,
                   "Event loop finished for the thread server.\n"));

      root_poa->destroy (1, 1);

      sorb_->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught an exception in server task: ");
      return 1;
    }

  return 0;
}


