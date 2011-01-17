// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "ace/Process_Manager.h"
#include "ace/OS_NS_unistd.h"
#include "orbsvcs/CosNamingC.h"
#include "Web_ServerC.h"
#include "Iterator_Handler.h"

// Obtain reference to Iterator_Factory
Web_Server::Iterator_Factory_ptr
get_iterator (CORBA::ORB_ptr orb);

// Perform file requests
void invoke_requests (int argc,
                      ACE_TCHAR *argv[],
                      int *request_count,
                      Web_Server::Iterator_Factory_ptr f);

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      if (argc < 2)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Usage: client filename ")
                           ACE_TEXT ("[filename ...]\n")),
                          -1);
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Mighty ORB");

      // Get the Root POA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA");

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in ());

      // Activate the POA manager.
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate ();

      // Get an Iterator_Factory reference.
      Web_Server::Iterator_Factory_var factory =
        ::get_iterator (orb.in ());

      if (CORBA::is_nil (factory.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Object pointed to by:\n  %s\n")
                           ACE_TEXT ("is not an Iterator_Factory object.\n"),
                           argv[1]),
                          -1);

      // 1 millisecond delay to reduce "busy waiting" in ORB event
      // loop.  (simulating "work")
      ACE_Time_Value tv (0, 1000);

      // Variable used to keep track of when file retrieval has
      // completed.
      int request_count = 0;

      ::invoke_requests (argc,
                         argv,
                         &request_count,
                         factory.in ());

      // Run the ORB event loop.
      while (request_count > 0)
        {
          CORBA::Boolean more_work;

          more_work = orb->work_pending ();

          if (more_work)
            {
              orb->perform_work ();
            }
          else
            ACE_OS::sleep (tv);
        }

      orb->shutdown (0);

      orb->destroy ();
    }
  catch (const Web_Server::Error_Result& exc)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("Caught Web Server exception with ")
                         ACE_TEXT ("status %d\n"),
                         exc.status),
                        -1);
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Caught unexpected exception:"));

      return -1;
    }

  // Wait for all children to exit.
  ACE_Process_Manager::instance ()->wait ();

  return 0;
}

Web_Server::Iterator_Factory_ptr
get_iterator (CORBA::ORB_ptr o)
{
  CORBA::ORB_var orb = CORBA::ORB::_duplicate (o);

  // Get a reference to the Name Service.
  CORBA::Object_var obj =
    orb->resolve_initial_references ("NameService");

  // Narrow to a Naming Context
  CosNaming::NamingContext_var nc =
    CosNaming::NamingContext::_narrow (obj.in ());

  if (CORBA::is_nil (obj.in ()))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Nil reference to Name Service\n")));
      return Web_Server::Iterator_Factory::_nil ();
    }

  // Create a name.
  CosNaming::Name name;
  name.length (1);
  name[0].id = CORBA::string_dup ("Iterator_Factory");
  name[0].kind = CORBA::string_dup ("");

  obj = nc->resolve (name);

  Web_Server::Iterator_Factory_ptr factory =
    Web_Server::Iterator_Factory::_narrow (obj.in ());

  return factory;
}

void invoke_requests (int argc,
                      ACE_TCHAR *argv[],
                      int *request_count,
                      Web_Server::Iterator_Factory_ptr f)
{
  Web_Server::Iterator_Factory_var factory =
    Web_Server::Iterator_Factory::_duplicate (f);

  // Activate and run the reply handlers.
  for (int i = 0;
       i < argc - 1;  // Don't include the program name.
       ++i)
    {
      Iterator_Handler *handler = 0;
      ACE_NEW_THROW_EX (handler,
                        Iterator_Handler,
                        CORBA::NO_MEMORY ());

      // Transfer ownership to the POA.
      PortableServer::ServantBase_var tmp (handler);

      // This ends up being an AMI call, so it won't block.
      handler->run (request_count,
                    ACE_TEXT_ALWAYS_CHAR(argv[i + 1]),
                    factory.in ());
    }
}
