// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Param_Test
//
// = FILENAME
//    server.cpp
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"

#include "param_test_i.h"

// Parses the command line arguments and returns an error status

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "d");
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
                           "\n", argv [0]), 1);
      }

  return 0;  // Indicates successful parsing of command line
}

// Standard command line parsing utilities used.

int
main (int argc, char *argv[])
{
  CORBA::Environment env; // for any exceptions
  char *orb_name = "internet"; // unused by TAO

  // get the underlying ORB
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

  // initialize a param_test target object and register it with the object
  // adapter
  Param_Test_i *param_test;

  // implicitly initializes with the OA
  ACE_NEW_RETURN (param_test, Param_Test_i ("param_test"), 1);

  if (TAO_debug_level > 0)
    {
      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.

      CORBA::String str;

      str = orb_ptr->object_to_string (param_test, env);

      if (env.exception () != 0)
        {
          env.print_exception ("object2string");
          return 1;
        }

      ACE_OS::puts ((char *) str);
      ACE_OS::fflush (stdout);
      dmsg1 ("listening as object '%s'", str);
    }

  if (orb_ptr->run () == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "run"), -1);
    }

  // Free resources
  delete param_test;
  CORBA::release (oa_ptr);
  CORBA::release (orb_ptr);

  return 0;
}
