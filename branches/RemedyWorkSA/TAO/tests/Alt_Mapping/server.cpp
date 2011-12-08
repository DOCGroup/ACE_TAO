
//=============================================================================
/**
 *  @file    server.cpp
 *
 *  $Id$
 *
 *  @author Jeff Parsons
 */
//=============================================================================


#include "alt_mapping_i.h"
#include "tao/Codeset/Codeset.h"
#include "tao/debug.h"
#include "ace/Get_Opt.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_stdio.h"

// Parses the command line arguments and returns an error status.
static FILE *ior_output_file = 0;
static const ACE_TCHAR *ior_output_filename = ACE_TEXT("test.ior");

static int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("do:"));
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
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  PortableServer::POA_var oa_ptr;
  Alt_Mapping_i *svnt = 0;

  try
    {
      const char *orb_name = "";
      CORBA::ORB_var orb_ptr =
        CORBA::ORB_init (argc, argv, orb_name);

      CORBA::Object_var temp; // holder for the myriad of times we get
                              // an object which we then have to narrow.

      // Get the Root POA

      temp = orb_ptr->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (temp.in()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "(%P|%t) Unable to get root poa reference.\n"),
                          1);

      oa_ptr = PortableServer::POA::_narrow (temp.in());

      PortableServer::POAManager_var poa_manager =
        oa_ptr->the_POAManager ();

      // Parse remaining command line and verify parameters.
      parse_args (argc, argv);

      // initialize a param_test target object and register it with the object
      // adapter

      // Create the implementation object
      ACE_NEW_RETURN (svnt,
                      Alt_Mapping_i (orb_ptr.in ()),
                     -1);

      // Register with GoodPOA with a specific name
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId ("alt_mapping");
      oa_ptr->activate_object_with_id (id.in (),
                                       svnt);

      // Stringify the objref we'll be implementing, and print it to
      // stdout.  Someone will take that string and give it to a
      // client.  Then release the object.

      temp = oa_ptr->id_to_reference (id.in ());

      CORBA::String_var str =
        orb_ptr->object_to_string (temp.in ());

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
      poa_manager->activate ();

      orb_ptr->run ();

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

  /// Free resources.
  delete svnt;

  return 0;
}
