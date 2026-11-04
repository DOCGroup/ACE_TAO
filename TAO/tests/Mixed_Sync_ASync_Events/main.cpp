//=============================================================================
/**
 *  @file    main.cpp
 *
 *  Implementation of the server.
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "test_i.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include "ace/SString.h"
#include "ace/OS_NS_unistd.h"
#include <memory>

struct Options
{
  ACE_TString ior_output_file;
  ACE_TString input_ior;
  A::RunMode mode {A::RM_SLAVE};
  CORBA::ULong max_count {20};
};

int
parse_args (int argc, ACE_TCHAR *argv[], Options &options)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("o:k:c:dm"));
  int c {};

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        options.ior_output_file = get_opts.opt_arg ();
        break;
      case 'k':
        options.input_ior = get_opts.opt_arg ();
        break;
      case 'm':
        options.mode = A::RM_MASTER;
        break;
      case 'c':
        options.max_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case 'd':
        TAO_debug_level++;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile>"
                           "\n",
                           argv [0]),
                          -1);
        break;
      }

  if (options.ior_output_file.empty ())
    options.ior_output_file = options.mode == A::RM_MASTER
      ? ACE_TEXT ("master.ior") : ACE_TEXT ("slave.ior");

  if (options.input_ior.empty ())
    options.input_ior = options.mode == A::RM_SLAVE
      ? ACE_TEXT ("file://master.ior") : ACE_TEXT ("file://slave.ior");

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  try
    {
      Options options;
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, argv);

      CORBA::Object_var const poa_object = orb->resolve_initial_references ("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var const root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var const poa_manager = root_poa->the_POAManager ();

      if (parse_args (argc, argv, options) != 0)
        return 1;

      // create, activate and initialize AMI reply handler
      Test_Reply_i test_i_rh_srv (orb.in (), options.max_count, options.mode);
      PortableServer::ObjectId_var const handler_id =
        root_poa->activate_object (&test_i_rh_srv);

      CORBA::Object_var const handler_object = root_poa->id_to_reference (handler_id.in ());

      A::AMI_TestHandler_var const rh = A::AMI_TestHandler::_narrow (handler_object.in ());

      test_i_rh_srv.test_handler ().set_reply_handler (rh.in ());

      // create and activate test servant
      Test_i test_i_srv (orb.in (), rh.in (), options.mode);

      PortableServer::ObjectId_var const test_id = root_poa->activate_object (&test_i_srv);

      CORBA::Object_var const test_object = root_poa->id_to_reference (test_id.in ());

      A::Test_var const test_var = A::Test::_narrow (test_object.in ());

      CORBA::String_var const ior = orb->object_to_string (test_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Servant activated\n"));

      // Write the IOR to the configured output file.
      {
        std::unique_ptr<FILE, int (*) (FILE *)> output_file (
          ACE_OS::fopen (options.ior_output_file.c_str (), ACE_TEXT ("w")),
          ACE_OS::fclose);
        if (!output_file)
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("Cannot open output file for writing IOR: %s"),
                             options.ior_output_file.c_str ()),
                            1);
        ACE_OS::fprintf (output_file.get (), "%s", ior.in ());
      }

      poa_manager->activate ();

      A::Test_var opponent;
      do
        {
          if (options.mode == A::RM_SLAVE)
            ACE_OS::sleep (ACE_Time_Value (0, 100));

          // Get object reference for opponent.
          CORBA::Object_var const object =
            orb->string_to_object (ACE_TEXT_ALWAYS_CHAR (options.input_ior.c_str ()));
          opponent = A::Test::_narrow (object.in ());
        }
      while (options.mode == A::RM_SLAVE && CORBA::is_nil (opponent.in ()));

      if (CORBA::is_nil (opponent.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Cannot resolve opponent IOR: %s"),
                           options.input_ior.c_str ()),
                          1);

      // register opponent
      test_i_srv.set_opponent (opponent.in ());
      test_i_rh_srv.test_handler ().set_opponent (opponent.in ());

      // start the show
      if (options.mode == A::RM_MASTER)
        test_i_rh_srv.test_handler ().start ();

      orb->run ();

      root_poa->destroy (true,  // ethernalize objects
                         false); // wait for completion

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
