// $Id$

#include "Coordinator.h"
#include "Session_Control.h"
#include "ace/Get_Opt.h"

ACE_RCSID(Big_Oneways, server, "$Id$")

const char *ior_output_file = "test.ior";
CORBA::ULong peer_count    = 4;
CORBA::ULong payload_size  = 1024;
CORBA::ULong message_count = 1000;
CORBA::ULong thread_count  = 4;

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, "o:p:b:i:n:");
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'o':
        ior_output_file = get_opts.opt_arg ();
        break;

      case 'p':
        peer_count  = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'b':
        payload_size  = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'i':
        message_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case 'n':
        thread_count = ACE_OS::atoi (get_opts.opt_arg ());
        break;

      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-o <iorfile> "
                           "-p <peer_count> "
                           "-b <payload_size> "
                           "-i <message_count> "
                           "-n <thread_count> "
                           "\n",
                           argv [0]),
                          -1);
      }
  // Indicates sucessful parsing of the command line
  return 0;
}

int
main (int argc, char *argv[])
{
  ACE_TRY_NEW_ENV
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic got a nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      if (parse_args (argc, argv) != 0)
        return 1;

      Coordinator *coordinator_impl;
      ACE_NEW_RETURN (coordinator_impl,
                      Coordinator (peer_count),
                      1);
      PortableServer::ServantBase_var coordinator_owner_transfer(coordinator_impl);

      Test::Coordinator_var coordinator =
        coordinator_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      CORBA::String_var ior =
        orb->object_to_string (coordinator.in () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG, "Waiting for peers . . . "));
      for (int i = 0;
           i != 60 && !coordinator_impl->has_all_peers ();
           ++i)
        {
          ACE_Time_Value tv (1, 0);
          orb->run (tv ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_DEBUG ((LM_DEBUG, "done.\n"));

      if (!coordinator_impl->has_all_peers ())
        {
          ACE_ERROR ((LM_DEBUG,
                      "ERROR: timeout, some peers failed to register\n"));
          return 1;
        }

      Session_Control *session_control_impl;
      ACE_NEW_RETURN (session_control_impl,
                      Session_Control (peer_count),
                      1);
      PortableServer::ServantBase_var session_control_owner_transfer(session_control_impl);

      Test::Session_Control_var session_control =
        session_control_impl->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      Test::Session_List session_list;
      coordinator_impl->create_session_list (session_control.in (),
                                             payload_size,
                                             thread_count,
                                             message_count,
                                             session_list
                                             ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_ASSERT (session_list.length () == peer_count);

      CORBA::ULong j;
      for (j = 0; j != peer_count; ++j)
        {
          // Make a copy of the sessions, excluding the j-th element
          Test::Session_List other_sessions (peer_count - 1);
          other_sessions.length (peer_count - 1);
          CORBA::ULong count = 0;
          for (CORBA::ULong k = 0; k != peer_count; ++k)
            {
              if (k == j)
                continue;
              other_sessions[count++] =
                Test::Session::_duplicate (session_list[k]);
            }

          session_list[j]->start (other_sessions
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }

      ACE_DEBUG ((LM_DEBUG, "Waiting for sessions . . . \n"));
      for (int k = 0;
           k != 300 && !session_control_impl->all_sessions_finished ();
           ++k)
        {
          ACE_Time_Value tv (1, 0);
          orb->run (tv ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }

      if (!session_control_impl->all_sessions_finished ())
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: timeout waiting for sessions\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "All sessions finished . . . \n"));

      for (j = 0; j != peer_count; ++j)
        {
          session_list[j]->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_TRY_CHECK;

        }

      coordinator_impl->shutdown_all_peers (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      root_poa->destroy (1, 1 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      orb->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception caught:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
