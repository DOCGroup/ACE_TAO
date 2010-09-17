// $Id$

#include "Coordinator.h"
#include "Session_Control.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_stdio.h"
#include "ace/OS_NS_unistd.h"

const ACE_TCHAR *ior_output_file = ACE_TEXT("test.ior");
CORBA::ULong peer_count    = 4;
CORBA::ULong payload_size  = 1024;
CORBA::ULong message_count = 1000;
CORBA::ULong thread_count  = 4;

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("o:p:b:i:n:"));
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

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Panic got a nil RootPOA\n"),
                          1);

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      if (parse_args (argc, argv) != 0)
        return 1;

      Coordinator *coordinator_impl;
      ACE_NEW_RETURN (coordinator_impl,
                      Coordinator (peer_count),
                      1);

      PortableServer::ObjectId_var id =
        root_poa->activate_object (coordinator_impl);

      CORBA::Object_var object = root_poa->id_to_reference (id.in ());

      Test::Coordinator_var coordinator =
        Test::Coordinator::_narrow (object.in ());

      CORBA::String_var ior =
        orb->object_to_string (coordinator.in ());

      // If the ior_output_file exists, output the ior to it
      FILE *output_file= ACE_OS::fopen (ior_output_file, "w");
      if (output_file == 0)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Cannot open output file for writing IOR: %s",
                           ior_output_file),
                              1);
      ACE_OS::fprintf (output_file, "%s", ior.in ());
      ACE_OS::fclose (output_file);

      poa_manager->activate ();

      ACE_DEBUG ((LM_DEBUG, "Waiting for peers . . . "));
      for (int i = 0;
           i != 30 && !coordinator_impl->has_all_peers ();
           ++i)
        {
          ACE_Time_Value tv (1, 0);
          orb->run (tv);
        }
      ACE_DEBUG ((LM_DEBUG, "done.\n"));

      if (!coordinator_impl->has_all_peers ())
        {
          ACE_ERROR ((LM_DEBUG,
                      "ERROR: timeout, some peers failed to register\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "Building session list . . . "));

      Session_Control *session_control_impl = 0;
      ACE_NEW_RETURN (session_control_impl,
                      Session_Control (peer_count),
                      1);

      id = root_poa->activate_object (session_control_impl);

      object = root_poa->id_to_reference (id.in ());

      Test::Session_Control_var session_control =
        Test::Session_Control::_narrow (object.in ());

      Test::Session_List session_list;
      coordinator_impl->create_session_list (session_control.in (),
                                             payload_size,
                                             thread_count,
                                             message_count,
                                             session_list);

      ACE_ASSERT (session_list.length () == peer_count);
      ACE_DEBUG ((LM_DEBUG, "done.\n"));

      ACE_DEBUG ((LM_DEBUG, "Giving start signal . . . "));
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

          session_list[j]->start (other_sessions);
        }

      ACE_DEBUG ((LM_DEBUG, "done.\n"));

      ACE_DEBUG ((LM_DEBUG, "Waiting for sessions . . .\n"));
      for (int k = 0;
           k != 300 && !session_control_impl->all_sessions_finished ();
           ++k)
        {
          ACE_Time_Value tv (1, 0);
          orb->run (tv);
        }

      if (!session_control_impl->all_sessions_finished ())
        {
          ACE_ERROR ((LM_ERROR,
                      "ERROR: timeout waiting for sessions\n"));
          return 1;
        }

      ACE_DEBUG ((LM_DEBUG, "All sessions finished . . .\n"));

      for (j = 0; j != peer_count; ++j)
        {
          session_list[j]->destroy ();

        }

      ACE_DEBUG ((LM_DEBUG, "Shutdown all peers . . .\n"));

      coordinator_impl->shutdown_all_peers ();
      ACE_OS::sleep (5); // Allow the shutdown message to be processed.
      coordinator_impl->_remove_ref();
      session_control_impl->_remove_ref();

      ACE_DEBUG ((LM_DEBUG, "Shutdown poa and orb . . .\n"));

      root_poa->destroy (1, 1);

      orb->destroy ();
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
