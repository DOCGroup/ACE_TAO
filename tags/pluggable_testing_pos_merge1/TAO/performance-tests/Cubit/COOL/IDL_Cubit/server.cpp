// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/IDL_Cubit
//
// = FILENAME
//    svr.cpp
//
// = AUTHOR
//    Andy Gokhale, Sumedh Mungee and Sergio Flores-Gaitan
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

#include "cubit_i.h"

#include <api/binding.H>
#include <api/api.H>
#include "cubit.H"
#include "sk_cubit.H"
#include "sk_cubit.C"

ACE_RCSID(IDL_Cubit, server, "$Id$")

void print_exception (const CORBA_Environment &env,
		      const char * str)
{
  CORBA::SystemException* ex;
    
  ex = CORBA::SystemException::_narrow(env.exception());
  
  if (ex) 
    {
      CORBA::String_var msg = ex->message();
      fprintf(stderr, "%s: %s.\n", str, (const char *) msg);
    } 
  else 
    {
      fprintf(stderr, "Unknown user exception.\n");
    }
}


// Global Variables
static int num_of_objs = 1;

u_int _debug_level = 0;

// Parses the command line arguments and returns an error status

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "dn:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        _debug_level++;
        break;
      case 'n':	// number of cubit objects we hold
        num_of_objs = ACE_OS::atoi (get_opts.optarg);
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s"
                           " [-d]"
                           " [-n] <num of cubit objects>"
                           "\n", argv [0]), 1);
      }

  return 0;  // Indicates successful parsing of command line
}

// Standard command line parsing utilities used.

int
main (int argc, char *argv[])
{
  CORBA::Environment env;

  COOL::EOA::bind(argv[1], env);
  if (env.exception()) {
    ACE_DEBUG ((LM_ERROR, "Impossible to bind the ORB to the description:%s\n", argv[1]));
    return -1;
  }

  CORBA::ORB_ptr orb_ptr = CORBA::ORB_init (argc, argv, 0, env);

  if (env.exception () != 0)
    {
      print_exception (env, "ORB init");
      return 1;
    }

  // Initialize the Object Adapter
  CORBA_BOA_ptr oa_ptr = orb_ptr->OA_init (argc, argv, 0, env);

  if (env.exception() != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
		       " (%P|%t) Unable to initialize the POA.\n"),
		      1);

  // Parse remaining command line and verify parameters.
  parse_args (argc, argv);

  // create a factory implementation
  Cubit_Factory_ptr factory;
  Cubit_Factory_i *factory_i;
  COOL::EOABindingData bindingData;

  ACE_NEW_RETURN (factory_i, Cubit_Factory_i ("factory", num_of_objs), 1);

  COOL_bind (*factory_i, factory, bindingData, env);

  if (_debug_level > 0)
    {
      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.

      CORBA::String str;

      str = orb_ptr->object_to_string (factory, env);

      if (env.exception () != 0)
        {
          print_exception (env, "object_to_string");
          return -1;
        }

      ACE_OS::puts ((char *) str);
      ACE_OS::fflush (stdout);

      ACE_DEBUG ((LM_DEBUG, "listening as object '%s'\n", str));
    }

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  ACE_DEBUG ((LM_DEBUG, "Entering boa->run ().\n"));
  oa_ptr->run ();

  return 0;
}
