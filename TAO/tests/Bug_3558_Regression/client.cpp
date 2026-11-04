#include "TestC.h"
#include "ace/Barrier.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "tao/ORB_Core.h"
#include "tao/Thread_Lane_Resources.h"
#include "tao/Transport_Cache_Manager.h"
#include <string>
#include <vector>
  struct Client_Context
  {
    Client_Context (CORBA::ORB_ptr orb, ACE_TCHAR const *ior1, ACE_TCHAR const *ior2)
      : orb (CORBA::ORB::_duplicate (orb)),
        ior1 (ior1),
        ior2 (ior2),
        barrier (2)
    {
    }

    CORBA::ORB_var const orb;
    ACE_TCHAR const *const ior1;
    ACE_TCHAR const *const ior2;
    ACE_Barrier barrier;
  };

  struct Thread_Argument
  {
    Client_Context &context;
    int server_number;
  };

int
parse_args (int argc, ACE_TCHAR *argv[], ACE_TCHAR const *&ior1, ACE_TCHAR const *&ior2,
            int &debug_level)
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:l:d:"));
  int option {};

  while ((option = get_opts ()) != -1)
    switch (option)
      {
      case 'k':
        ior1 = get_opts.opt_arg ();
        break;
      case 'l':
        ior2 = get_opts.opt_arg ();
        break;
      case 'd':
        debug_level = ACE_OS::atoi (get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage: %s -k <ior1> -l <ior2> -d <debug_level>\n"),
                           argv[0]),
                          -1);
      }
  return 0;
}

ACE_THR_FUNC_RETURN
test_server (void *arg)
{
  Thread_Argument &thread_argument = *static_cast<Thread_Argument *> (arg);
  Client_Context &context = thread_argument.context;
  int const server_number = thread_argument.server_number;
  ACE_TCHAR const *const ior = server_number == 1 ? context.ior1 : context.ior2;

  if (server_number == 2)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D Expecting an exception for server %d\n"), server_number));

  Test::Hello_var hello;
  try
    {
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D Starting connection with server %d\n"), server_number));
      CORBA::Object_var const server = context.orb->string_to_object (ior);
      hello = Test::Hello::_narrow (server.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_ERROR ((LM_ERROR, ACE_TEXT ("%D (%t) Nil Test::Hello reference <%s> for server %d\n"),
                      ior, server_number));
          return ACE_THR_FUNC_RETURN_NULL;
        }

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%D Transport data before invocation to server %d: current size: %d\n"),
                  server_number,
                  context.orb->orb_core ()->lane_resources ().transport_cache ().current_size ()));
      CORBA::String_var const the_string = hello->get_string ();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%D Transport data after invocation to server %d: current size: %d\n"),
                  server_number,
                  context.orb->orb_core ()->lane_resources ().transport_cache ().current_size ()));
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D (%P|%t) - string <%C> returned from server %d\n"),
                  the_string.in (), server_number));
    }
  catch (CORBA::TRANSIENT const &exception)
    {
      if (server_number == 2)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D Expected exception for server %d: %C\n"),
                    server_number, exception._info ().c_str ()));
      else
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%D Unexpected exception for server %d: %C\n"),
                    server_number, exception._info ().c_str ()));
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%D Unexpected exception for server %d\n"), server_number));
    }

  if (server_number == 2)
    context.barrier.wait ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D Shutdown server %d\n"), server_number));
  if (!CORBA::is_nil (hello.in ()))
    {
      try
        {
          hello->shutdown ();
        }
      catch (...)
        {
        }
    }

  if (server_number == 1)
    context.barrier.wait ();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D Thread ended for server %d\n"), server_number));
  return ACE_THR_FUNC_RETURN_NULL;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_TCHAR const *ior1 = ACE_TEXT ("file://server1.ior");
  ACE_TCHAR const *ior2 = ACE_TEXT ("file://server2.ior");
  int debug_level {};

  try
    {
      if (parse_args (argc, argv, ior1, ior2, debug_level) != 0)
        return 1;

      std::vector<std::basic_string<ACE_TCHAR>> extra_arguments {
        ACE_TEXT ("-ORBSvcConfDirective"),
        ACE_TEXT ("static Resource_Factory \"-ORBConnectionCacheMax 1 -ORBConnectionCachePurgePercentage 100\""),
        ACE_TEXT ("-ORBSvcConfDirective"),
        ACE_TEXT ("static Client_Strategy_Factory \"-ORBTransportMuxStrategy EXCLUSIVE\"")
      };
      if (debug_level > 0)
        {
          extra_arguments.emplace_back (ACE_TEXT ("-ORBdebuglevel"));
          extra_arguments.emplace_back (ACE_TEXT ("10"));
        }

      std::vector<ACE_TCHAR *> orb_arguments;
      orb_arguments.reserve (argc + extra_arguments.size ());
      for (int index {}; index < argc; ++index)
        orb_arguments.push_back (argv[index]);
      for (std::basic_string<ACE_TCHAR> &argument : extra_arguments)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D client adding arg: %s\n"), argument.c_str ()));
          orb_arguments.push_back (argument.data ());
        }

      argc = static_cast<int> (orb_arguments.size ());
      CORBA::ORB_var const orb = CORBA::ORB_init (argc, orb_arguments.data ());
      Client_Context context (orb.in (), ior1, ior2);
      Thread_Argument first_thread {context, 1};
      Thread_Argument second_thread {context, 2};

      if (ACE_Thread_Manager::instance ()->spawn (test_server,
                                                   &first_thread,
                                                   THR_NEW_LWP | THR_JOINABLE) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Error spawning thread 1\n")), 1);

      ACE_OS::sleep (2);
      if (ACE_Thread_Manager::instance ()->spawn (test_server,
                                                   &second_thread,
                                                   THR_NEW_LWP | THR_JOINABLE) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("Error spawning thread 2\n")), 1);

      ACE_Thread_Manager::instance ()->wait ();
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D Transport data when done: current size: %d\n"),
                  orb->orb_core ()->lane_resources ().transport_cache ().current_size ()));
      orb->destroy ();
    }
  catch (CORBA::Exception const &exception)
    {
      exception._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
