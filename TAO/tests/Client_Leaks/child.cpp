// $Id$

#include "Process.h"
#include "ace/Get_Opt.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      if (argc < 2)
        {
          // Paranoia, we should have an auto_ptr-like gadget for
          // this.
          orb->destroy ();

          ACE_ERROR_RETURN ((LM_ERROR,
                             "Usage: %s <ior>\n",
                             argv[0]), 1);
        }

      CORBA::Object_var object =
        orb->string_to_object (argv[1]);
      Test::Startup_Callback_var startup_callback =
        Test::Startup_Callback::_narrow (object.in ());
      if (CORBA::is_nil (startup_callback.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "ERROR: nil startup callback\n"),
                            1);
        }

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (root_poa.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic: nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      Process *process_impl;
      ACE_NEW_RETURN (process_impl,
                      Process (orb.in ()),
                      1);
      PortableServer::ServantBase_var owner_transfer(process_impl);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (process_impl);

      CORBA::Object_var object_act = root_poa->id_to_reference (id.in ());

      Test::Process_var process =
        Test::Process::_narrow (object_act.in ());

      poa_manager->activate ();

      startup_callback->started (process.in ());

      ACE_Time_Value tv (50, 0);
      orb->run (tv);

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception&)
    {
      // Do not print error messages, they only make the test output
      // confusing.
      return 1;
    }

  return 0;
}
