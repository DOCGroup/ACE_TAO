// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/orbsvcs/tests/Logger
//
// = FILENAME
//    svr.cpp
//
// = DESCRIPTION
//    This program is an implementation of a simple logger service.
//    Whatever is sent to it through its interface is displayed on stdout.
//    It uses the Logger_Factory server to create logger objects.
//
// = AUTHORS
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Log/Logger_i.h"

int
main (int argc, char ** argv)
{
  TAO_TRY
    {
      CORBA::ORB_var orb = CORBA::ORB_init (argc, argv, 0, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Initialize the Object Adapter
      CORBA::Object_var poa_object =
	orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil(poa_object.in()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to initialize the POA.\n"),
			  1);

      PortableServer::POA_var root_poa =
	PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
	root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var naming_obj =
	orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
	ACE_ERROR_RETURN ((LM_ERROR,
			   " (%P|%t) Unable to resolve the Name Service.\n"),
			  1);

      ACE_DEBUG ((LM_DEBUG, "Naming Service resolved.\n"));

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "CosNaming::NamingContext::_narrow()  ok.\n"));

      // create a factory implementation
      Logger_Factory_i factory_impl;

      Logger_Factory_var factory =
	factory_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::String_var str =
	orb->object_to_string (factory.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "The factory IOR is <%s>\n", str.in ()));

      // Register the servant with the Naming Context....
      CosNaming::Name factory_name (1);
      factory_name.length (1);
      factory_name[0].id = CORBA::string_dup ("logger_factory");
      naming_context->bind (factory_name, factory.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

//      naming_context->bind (factory_name, factory.in (), TAO_TRY_ENV);
//      if (TAO_TRY_ENV.exception () != 0)
//	{ TAO_TRY_ENV.print_exception ("bind: name already bound\n");
//	return -1;
//	}

      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_DEBUG ((LM_DEBUG, "running logging service\n"));
      if (orb->run () == -1)
	ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), 1);


      // unbind the logger factory name
      naming_context->unbind (factory_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("logger_service");
    }
  TAO_ENDTRY;

  return 0;
}
