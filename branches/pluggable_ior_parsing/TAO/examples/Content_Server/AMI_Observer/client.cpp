// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/Process_Manager.h"
#include "orbsvcs/CosNamingC.h"
#include "Push_Web_ServerC.h"
#include "Push_Iterator_Handler.h"

ACE_RCSID(AMI_Observer, client, "$Id$")

int
main (int argc, char *argv[])
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (argc < 2)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Usage: client filename [filename ...]\n"),
                          -1);

      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Mighty ORB",
                                            ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get the Root POA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA",
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Activate the POA manager.
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Get a reference to the Name Service.
      obj = orb->resolve_initial_references ("NameService",
                                             ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Narrow to a Naming Context
      CosNaming::NamingContext_var nc = 
        CosNaming::NamingContext::_narrow (obj.in (), ACE_TRY_ENV);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (obj.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Nil reference to Name Service\n"),
                            -1);
        }

      // Create a name.
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("Push_Iterator_Factory");
      name[0].kind = CORBA::string_dup ("");

      obj = nc->resolve (name, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now narrow to an Iterator_Factory reference.
      Web_Server::Iterator_Factory_var factory =
        Web_Server::Iterator_Factory::_narrow (obj.in ());
      if (CORBA::is_nil (factory.in ()))
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Object pointed to by:\n  %s\n"
                             "is not an Iterator_Factory object.\n",
                             argv[1]),
                            -1);
        }

      // Variable used to keep track of when file retrieval has
      // completed.
      int request_count = 0;

      // Activate and run the reply handlers.
      for (int i = 0;
           i < argc - 1;  // Don't include the program name.
           ++i)
        {
          Push_Iterator_Handler *handler = 0;
          ACE_NEW_RETURN (handler,
                          Push_Iterator_Handler,
                          -1);

          // Transfer ownership to the POA.
          PortableServer::ServantBase_var tmp_handler (handler);

          handler->run (&request_count,
                        argv[i + 1],
                        factory.in (),
                        ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      // 1 millisecond delay to reduce "busy waiting" in ORB event
      // loop.
      ACE_Time_Value tv (0, 1000);

      // Run the ORB event loop.
      while (request_count > 0)
        {
          if (orb->work_pending (ACE_TRY_ENV))
            {
              orb->perform_work (ACE_TRY_ENV);
              ACE_TRY_CHECK;
            }
          else
            ACE_OS::sleep (tv);
        }
      ACE_TRY_CHECK;

      orb->shutdown (0, ACE_TRY_ENV);
      ACE_TRY_CHECK;

      //orb->destroy (ACE_TRY_ENV);
      //ACE_TRY_CHECK;
    }
  ACE_CATCH (Web_Server::Error_Result, exc)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Caught Web Server exception with status %d\n",
                         exc.status),
                        -1);
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught unexpected exception:");

      return -1;
    }
  ACE_ENDTRY;

  // Wait for all spawned viewers to exit.
  ACE_Process_Manager::instance ()->wait ();

  return 0;
}
