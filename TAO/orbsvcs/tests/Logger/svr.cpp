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

#include <iostream.h>
#include "loggerS.h"
#include "logger_i.h"
#include "CosNamingC.h"

int
main (int argc, char ** argv)
{
  CORBA::Environment env;
  char *orb_name = "logger";

  CORBA::ORB_ptr orb_ptr = CORBA::ORB_init (argc, argv, orb_name, env);

  if (env.exception () != 0)
    {
      env.print_exception ("ORB init");
      return 1;
    }

  // Initialize the Object Adapter
  CORBA::POA_ptr oa_ptr = orb_ptr->POA_init (argc, argv, "POA");

  if (oa_ptr == 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize the POA.\n"),
		      1);

  // create a factory implementation
  Logger_Factory_ptr f;

  ACE_NEW_RETURN (f, Logger_Factory_i ("factory"), -1);

  // Stringify the objref we'll be implementing, and print it to
  // stdout.  Someone will take that string and give it to a
  // client.  Then release the object.
  
  CORBA::String str;
  
  str = orb_ptr->object_to_string (f, env);
  
  if (env.exception () != 0)
    {
      env.print_exception ("object2string");
      return 1;
    }
  
  ACE_DEBUG ((LM_DEBUG, "listening as object '%s'\n", str)); 

  int name_service_flag = 0;
  CosNaming::NamingContext_ptr naming_service = 0;
  CosNaming::Name n(1);

  do {
    CORBA::Object_ptr  obj_ptr = 
      orb_ptr->resolve_initial_references ("NameService");
  
    // Use the naming service to advertise us, if we could successfully get a reference.
    if (CORBA::is_nil (obj_ptr) == CORBA::B_TRUE)
      {
	ACE_ERROR ((LM_ERROR, "resolve_initial_references failed.\n"));
	break;
      }
    
    // resolve the naming service
    naming_service = CosNaming::NamingContext::_narrow (obj_ptr, env);

    if (CORBA::is_nil (naming_service) == CORBA::B_TRUE)
      {
	ACE_ERROR ((LM_ERROR, "_narrow failed\n"));
	break;
      }

    // The name of the logger factory in the naming service.
    n.length (1);
    n[0].id = CORBA::string_dup ("logger_factory");
    
    // bind the logger factory to a name using the naming service.
    naming_service->bind (n, f, env);
    
    // check for errors!
    if (env.exception () != 0)
      {
	env.print_exception ("name_service->bind()");
	break;
      }
    else
      {
	ACE_DEBUG ((LM_DEBUG, "Success using the naming service!! to bind the logger factory!\n"));
	name_service_flag = 1;
      }

  } while (0);

  if (name_service_flag == 0)
    {
      ACE_ERROR ((LM_ERROR, "Unable to use the naming service.\n"));
    }

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  if (orb_ptr->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  // unbind the logger factory name 
  naming_service->unbind (n, env);

  // check for errors!
  if (env.exception () != 0)
    {
      env.print_exception ("name_service->unbind()");
    }

  // release the object reference for the naming service.
  CORBA::release (naming_service);
  
  cout << "Server logger_factory is terminating" << endl;
  return 0;
}

