// $Id$

#include "orbsvcs/CosNamingC.h"
#include "orbsvcs/Log/Logger_i.h"
#include "tao/TAO.h"

// Forward declaration
int parse_args (int argc_, char** argv_);

void setup_naming (CORBA::ORB_var orb,
		   CosNaming::NamingContext_var naming_context,
		   CORBA::Environment &env);

int
main (int argc, char ** argv)
{
  // Get command line information
  parse_args(argc, argv);
  
  TAO_TRY
    {
      // Set up the ORB
      TAO_ORB_Manager orb_manager;
      orb_manager.init (argc, argv, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Get access to the underlying ORB and POA
      CORBA::ORB_var orb = orb_manager.orb ();
      PortableServer::POA_var poa = orb_manager.root_poa (); 
      
      // Setup the interaction with the naming service
      CosNaming::NamingContext_var naming_context;
      setup_naming (orb, naming_context, TAO_TRY_ENV);
      TAO_CHECK_ENV;
	
      // create a factory implementation
      Logger_Factory_i factory_impl;
      Logger_Factory_var factory = factory_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      
      // If debugging, output the factory IOR
      if (TAO_debug_level > 0)
	{
	  CORBA::String_var str =
	    orb.in ()->object_to_string (factory.in (), TAO_TRY_ENV);
	  TAO_CHECK_ENV;
	  ACE_DEBUG ((LM_DEBUG, "The factory IOR is <%s>\n", str.in ()));
	}

      
      // Register the servant with the Naming Context....
      CosNaming::Name factory_name (1);
      factory_name.length (1);
      factory_name[0].id = CORBA::string_dup ("logger_factory");
      naming_context->bind (factory_name, factory.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_TRY_ENV.exception () != 0)
	{
	  TAO_TRY_ENV.print_exception ("bind: name already bound\n");
	  return -1;
	}

      PortableServer::POAManager_var poa_manager =
	poa->the_POAManager(TAO_TRY_ENV);
      
      // Activate the servant
      poa_manager->activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (TAO_debug_level > 0)
	ACE_DEBUG ((LM_DEBUG, "running logging service\n"));
      
      orb_manager.run (TAO_TRY_ENV);
      TAO_CHECK_ENV;

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

// Parses the command line arguments and returns an error status
int
parse_args (int argc_, char** argv_)
{
  ACE_Get_Opt get_opts (argc_, argv_, "d");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;	
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
			   "\n"
                           "   -d: increase debug level\n",
			   argv_ [0]),
			  -1);
      }

  // Indicates successful parsing of command line.
  return 0;
}

void
setup_naming (CORBA::ORB_var orb,
	      CosNaming::NamingContext_var naming_context,
	      CORBA::Environment &env)
{
  CORBA::Object_var naming_obj =
    orb->resolve_initial_references ("NameService");
  
  naming_context =
    CosNaming::NamingContext::_narrow (naming_obj.in (), env);
}
  
