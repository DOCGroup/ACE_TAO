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

#include "param_test_i.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

ACE_RCSID (Param_Test,
           server,
           "$Id$")

// Parses the command line arguments and returns an error status.
static FILE *ior_output_file = 0;
static const char *ior_output_filename = "test.ior";

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "do:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'd':  // debug flag
        TAO_debug_level++;
        break;
      case 'o':
        ior_output_filename = get_opts.opt_arg ();
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
  PortableServer::POA_var oa_ptr;
  Param_Test_i *param_test = 0;

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      const char *orb_name = "";
      CORBA::ORB_var orb_ptr =
        CORBA::ORB_init (argc, argv, orb_name ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var temp; // holder for the myriad of times we get
                              // an object which we then have to narrow.

      // Get the Root POA

      temp = orb_ptr->resolve_initial_references ("RootPOA"
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (temp.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to get root poa reference.\n"),
                          1);

      oa_ptr = PortableServer::POA::_narrow (temp.in() ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POAManager_var poa_manager =
        oa_ptr->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::PolicyList policies (2);
      policies.length (2);
      policies[0] =
        oa_ptr->create_id_assignment_policy (PortableServer::USER_ID
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      policies[1] =
        oa_ptr->create_lifespan_policy (PortableServer::PERSISTENT
                                        ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // We use a different POA, otherwise the user would have to
      // change the object key each time it invokes the server.
      PortableServer::POA_var good_poa =
        oa_ptr->create_POA ("child_poa",
                            poa_manager.in (),
                            policies
                            ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Parse remaining command line and verify parameters.
      parse_args (argc, argv);

      // initialize a param_test target object and register it with the object
      // adapter

      // Create the implementation object
      ACE_NEW_RETURN (param_test,
                      Param_Test_i ("unknown",
                                    orb_ptr.in ()), 1);

      // Register with GoodPOA with a specific name
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("param_test");
      good_poa->activate_object_with_id (id.in (),
                                         param_test
                                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.

      temp = good_poa->id_to_reference (id.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var str =
        orb_ptr->object_to_string (temp.in ()
                                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) The IOR is <%s>\n",
                      str.in ()));
        }

      ior_output_file = ACE_OS::fopen (ior_output_filename, "w");

      if (ior_output_file == 0)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Unable to open %s for writing: %p\n",
                             ior_output_filename),
                            -1);
        }

      ACE_OS::fprintf (ior_output_file,
                       "%s",
                       str.in ());
      ACE_OS::fclose (ior_output_file);


      // Make the POAs controlled by this manager active
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb_ptr->run (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      good_poa->destroy (1,
                         1
                         ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      oa_ptr->destroy (1,
                       1
                       ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::SystemException, sysex)
    {
      ACE_PRINT_EXCEPTION (sysex, "System Exception");
      return -1;
    }
  ACE_CATCH (CORBA::UserException, userex)
    {
      ACE_PRINT_EXCEPTION (userex, "User Exception");
      return -1;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (-1);
  // Free resources
  delete param_test;

  return 0;
}
