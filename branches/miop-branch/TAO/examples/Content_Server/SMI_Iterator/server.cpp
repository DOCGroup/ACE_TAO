// -*- C++ -*-
// $Id$

// Ossama Othman <ossama@uci.edu>

#include "orbsvcs/CosNamingC.h"
#include "Web_ServerS.h"

#include "Iterator_Factory_i.h"

ACE_RCSID(SMI_Iterator, server, "$Id$")

int
main (int argc, char *argv[])
{
  TAO_ENV_DECLARE_NEW_ENV;
  ACE_TRY
    {
      // Initialize the ORB.
      CORBA::ORB_var orb = CORBA::ORB_init (argc,
                                            argv,
                                            "Mighty ORB"
                                            TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get the Root POA.
      CORBA::Object_var obj =
        orb->resolve_initial_references ("RootPOA"
                                         TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var poa =
        PortableServer::POA::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Activate the POA manager.
      PortableServer::POAManager_var mgr = poa->the_POAManager ();
      mgr->activate (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create the Iterator_Factory servant and object.
      // It activates and deactivates the Content_Iterator object.
      Iterator_Factory_i factory_servant;
      Web_Server::Iterator_Factory_var factory =
        factory_servant._this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Get a reference to the Name Service.
      obj = orb->resolve_initial_references ("NameService"
                                             TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Narrow to a Naming Context
      CosNaming::NamingContext_var nc;
      nc = CosNaming::NamingContext::_narrow (obj.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Create a name.
      CosNaming::Name name;
      name.length (1);
      name[0].id = CORBA::string_dup ("Iterator_Factory");
      name[0].kind = CORBA::string_dup ("");

      nc->bind (name, factory.in () TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Some debugging output.
      CORBA::String_var IOR = orb->object_to_string (factory.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Bound <%s> to <%s> in Name Service.\n"),
                  name[0].id.in (),
                  IOR.in ()));

      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("Accepting requests.\n")));

      // Accept requests.
      orb->run (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           ACE_TEXT ("Caught unexpected exception:"));

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}
