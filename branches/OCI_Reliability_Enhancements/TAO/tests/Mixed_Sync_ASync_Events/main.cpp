//=============================================================================
/**
 *  @file    main.cpp
 *
 *  $Id$
 *
 *  Implementation of the server.
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#include "test_i.h"
#include "tao/debug.h"
#include "ace/OS_NS_stdio.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior_output_file = 0;
const ACE_TCHAR *input_ior = 0;
A::RunMode mode_flag = A::RM_SLAVE;
CORBA::ULong max_count = 20;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:k:c:dm"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;
      case 'k':
        input_ior = get_opts.opt_arg ();
        break;
      case 'm':
        mode_flag = A::RM_MASTER;
        if (ior_output_file == 0)
          ior_output_file = ACE_TEXT ("master.ior");
        break;
      case 'c':
        max_count = ACE_OS::atoi (get_opts.opt_arg ());
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

  if (ior_output_file == 0)
    ior_output_file = ACE_TEXT ("slave.ior");

  if (input_ior == 0)
    input_ior = (mode_flag == A::RM_SLAVE ? ACE_TEXT ("file://master.ior") : ACE_TEXT ("file://slave.ior"));

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      // create, activate and initialize AMI reply handler
      Test_Reply_i test_i_rh_srv(orb.in (),
                                 max_count,
                                 mode_flag);
      PortableServer::ObjectId_var id =
        root_poa->activate_object (&test_i_rh_srv);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      A::AMI_TestHandler_var rh =
        A::AMI_TestHandler::_narrow (object.in ());

      test_i_rh_srv.test_handler ().set_reply_handler (rh.in ());

      // create and activate test servant
      Test_i test_i_srv (orb.in (), rh.in (), max_count, mode_flag);

      id = root_poa->activate_object (&test_i_srv);

      object = root_poa->id_to_reference (id.in ());

      A::Test_var test_var =
        A::Test::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (test_var.in ());

      ACE_DEBUG ((LM_DEBUG, "Servant activated\n"));

      // If the ior_output_file exists, output the ior to it
      if (ior_output_file != 0)
        {
          FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
          if (output_file == 0)
            ACE_ERROR_RETURN ((LM_ERROR,
                               "Cannot open output file for writing IOR: %s",
                               ior_output_file),
                              1);
          ACE_OS::fprintf (output_file, "%s", ior.in ());
          ACE_OS::fclose (output_file);
        }

      poa_manager->activate ();

      A::Test_var opponent;
      do {
        if (mode_flag == A::RM_SLAVE)
          ACE_OS::sleep (ACE_Time_Value (0, 100));

        // get object reference for opponent
        object = orb->string_to_object (input_ior);
        opponent =  A::Test::_narrow (object.in ());
      } while (mode_flag == A::RM_SLAVE && CORBA::is_nil (opponent.in ()));

      if (CORBA::is_nil (opponent.in ()))
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot resolve opponent IOR: %s",
                           input_ior),
                           1);
      }

      // register opponent
      test_i_srv.set_opponent (opponent.in ());
      test_i_rh_srv.test_handler ().set_opponent (opponent.in ());

      // start the show
      if (mode_flag == A::RM_MASTER)
        test_i_rh_srv.test_handler ().start ();

      orb->run ();

      root_poa->destroy (1,  // ethernalize objects
                         0  // wait for completion
                        );

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}
