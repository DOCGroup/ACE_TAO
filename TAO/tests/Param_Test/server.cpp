
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  @author Aniruddha Gokhale
 */
//=============================================================================


#include "param_test_i.h"
#include "tao/Codeset/Codeset.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"
#include <memory>

// Parses the command line arguments and returns an error status.
struct Server_Options
{
  ACE_TString ior_output_filename {ACE_TEXT ("test.ior")};
  int debug_level {};
};

static int
parse_args (int argc, ACE_TCHAR *argv[], Server_Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("do:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'd':  // debug flag
        ++options.debug_level;
        break;
      case 'o':
        options.ior_output_filename = get_opts.opt_arg ();
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
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  std::unique_ptr<Param_Test_i> param_test;

  try
    {
      char const * const orb_name = "";
      CORBA::ORB_var const orb_ptr =
        CORBA::ORB_init (argc, argv, orb_name);

      CORBA::Object_var temp; // holder for the myriad of times we get
                              // an object which we then have to narrow.

      // Get the Root POA
      temp = orb_ptr->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (temp.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to get root poa reference.\n"),
                          1);

      PortableServer::POA_var const oa_ptr = PortableServer::POA::_narrow (temp.in ());

      PortableServer::POAManager_var const poa_manager =
        oa_ptr->the_POAManager ();

      CORBA::PolicyList policies (2);
      policies.length (2);
      policies[0] =
        oa_ptr->create_id_assignment_policy (PortableServer::USER_ID);

      policies[1] =
        oa_ptr->create_lifespan_policy (PortableServer::PERSISTENT);

      // We use a different POA, otherwise the user would have to
      // change the object key each time it invokes the server.
      PortableServer::POA_var const good_poa =
        oa_ptr->create_POA ("child_poa",
                            poa_manager.in (),
                            policies);

      // Parse remaining command line and verify parameters.
      Server_Options options;
      if (parse_args (argc, argv, options) != 0)
        return 1;
      TAO_debug_level += options.debug_level;

      // initialize a param_test target object and register it with the object
      // adapter

      // Create the implementation object
      param_test = std::make_unique<Param_Test_i> ("unknown", orb_ptr.in ());

      // Register with GoodPOA with a specific name
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("param_test");
      good_poa->activate_object_with_id (id.in (),
                                         param_test.get ());

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.

      temp = good_poa->id_to_reference (id.in ());

      CORBA::String_var const str =
        orb_ptr->object_to_string (temp.in ());

      if (TAO_debug_level > 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) The IOR is <%s>\n",
                      str.in ()));
        }

      {
        std::unique_ptr<FILE, int (*) (FILE *)> const ior_output_file (
          ACE_OS::fopen (options.ior_output_filename.c_str (), ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!ior_output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Unable to open %s for writing: %p\n"),
                             options.ior_output_filename.c_str ()),
                            -1);

        ACE_OS::fprintf (ior_output_file.get (), "%s", str.in ());
      }

      // Make the POAs controlled by this manager active
      poa_manager->activate ();

      orb_ptr->run ();

      good_poa->destroy (true, true);

      oa_ptr->destroy (true, true);
    }
  catch (const CORBA::SystemException& sysex)
    {
      sysex._tao_print_exception ("System Exception");
      return -1;
    }
  catch (const CORBA::UserException& userex)
    {
      userex._tao_print_exception ("User Exception");
      return -1;
    }
  return 0;
}
