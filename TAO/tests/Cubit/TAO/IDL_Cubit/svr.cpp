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
#include "tao/corba.h"
#include "cubit_i.h"

// Global Variables
static int num_of_objs = 1;

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
        TAO_debug_level++;
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
  char *orb_name = "internet";

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

  // Parse remaining command line and verify parameters.
  parse_args (argc, argv);

  // create a factory implementation
  POA_Cubit_Factory_ptr factory;

  ACE_NEW_RETURN (factory, Cubit_Factory_i ("factory", num_of_objs), 1);

  if (TAO_debug_level > 0)
    {
      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.

      CORBA::Object_ptr obj = factory->_this (env);
      if (env.exception () != 0)
        {
          env.print_exception ("factory::_this()");
          return 1;
        }
      
      CORBA::String str = orb_ptr->object_to_string (obj, env);
      CORBA::release (obj);

      if (env.exception () != 0)
        {
          env.print_exception ("object2string");
          return 1;
        }

      ACE_OS::puts ((char *) str);
      ACE_OS::fflush (stdout);
      //dmsg1 ("Object Created at: '%ul'", obj);
      dmsg1 ("listening as object '%s'", str);
    }

  // Handle requests for this object until we're killed, or one of the
  // methods asks us to exit.
  if (orb_ptr->run () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);

  // Free resources
  CORBA::release (oa_ptr);
  CORBA::release (orb_ptr);

  return 0;
}
