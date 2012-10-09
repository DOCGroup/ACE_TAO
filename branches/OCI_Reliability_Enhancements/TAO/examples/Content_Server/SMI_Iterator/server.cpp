// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "orbsvcs/CosNamingC.h"
#include "Web_ServerS.h"

#include "Iterator_Factory_i.h"

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
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

      // Create the Iterator_Factory servant and object.
      // It activates and deactivates the Content_Iterator object.
      Iterator_Factory_i factory_servant;
      Web_Server::Iterator_Factory_var factory =
        factory_servant._this ();

      // Get a reference to the Name Service.
      obj = orb->resolve_initial_references ("NameService");

      // Narrow to a Naming Context
      CosNaming::NamingContext_var nc;
      nc = CosNaming::NamingContext::_narrow (obj.in ());

      // Create a name.
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("Iterator_Factory");
      name[0].kind = CORBA::string_dup ("");

      nc->bind (name, factory.in ());

      // Some debugging output.
      CORBA::String_var IOR = orb->object_to_string (factory.in ());
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Bound <%s> to <%s> in Name Service.\n"),
                  name[0].id.in (),
                  IOR.in ()));

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Accepting requests.\n")));

      // Accept requests.
      orb->run ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception (ACE_TEXT ("Caught unexpected exception:"));

      return -1;
    }

  return 0;
}
