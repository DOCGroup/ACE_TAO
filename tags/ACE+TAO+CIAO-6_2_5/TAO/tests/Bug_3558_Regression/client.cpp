// $Id$

#include "TestC.h"
#include "ace/Get_Opt.h"
#include "ace/Task.h"
#include "ace/Barrier.h"
#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_strings.h"

#include "tao/ORB_Core.h"
#include "tao/Transport_Cache_Manager.h"
#include "tao/Thread_Lane_Resources.h"

const ACE_TCHAR *ior1 = ACE_TEXT ("file://server1.ior");
const ACE_TCHAR *ior2 = ACE_TEXT ("file://server2.ior");

int debug_level = 0;

CORBA::ORB_var orb;

ACE_Barrier barrier(2);

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT ("k:l:d:"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'k':
        ior1 = get_opts.opt_arg ();
        break;

      case 'l':
        ior2 = get_opts.opt_arg ();
        break;
      case 'd':
        debug_level = ACE_OS::atoi( get_opts.opt_arg ());
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("usage:  %s ")
                           ACE_TEXT ("-k <ior1>\n")
                           ACE_TEXT ("-l <ior2>\n"),
                           ACE_TEXT ("-d <debug_level>\n"),
                           argv [0]),
                          -1);
      }
  // Indicates successful parsing of the command line
  return 0;
}


static void *
test_server (void *arg)
{
#if defined (ACE_OPENVMS) && (!defined (__INITIAL_POINTER_SIZE) || (__INITIAL_POINTER_SIZE < 64))
  int server_nr = reinterpret_cast<int> (arg);
#else
  intptr_t server_nr = reinterpret_cast<intptr_t> (arg);
#endif

  ACE_TCHAR ior[256];

  if (server_nr == 1)
    ACE_OS::sprintf(ior, ACE_TEXT ("%s"), ior1);
  else if (server_nr == 2)
    {
      ACE_OS::sprintf(ior, ACE_TEXT ("%s"), ior2);
      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT("%D Expecting an exception for server %d\n"),
                 server_nr));
    }

  Test::Hello_var hello;
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%D Starting connection with server %d\n"),
                  server_nr));

      CORBA::Object_var server = orb->string_to_object(ior);

      hello = Test::Hello::_narrow(server.in ());

      if (CORBA::is_nil (hello.in ()))
        {
          ACE_Thread::exit ();
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("%D (%t) Nil Test::Hello reference <%s> for server %d\n"),
                      ior, server_nr));
          return 0;
        }

      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT ("%D Transport data before invocation to server %d: current size: %d\n"),
                 server_nr,
                 orb->orb_core ()->lane_resources ().transport_cache ().current_size ()));

      CORBA::String_var the_string = hello->get_string ();

      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT ("%D Transport data after invocation to server %d: current size: %d\n"),
                 server_nr,
                 orb->orb_core ()->lane_resources ().transport_cache ().current_size ()));

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%D (%P|%t) - string <%C> returned from server %d\n"),
                  the_string.in (), server_nr));
    }
  catch (const CORBA::TRANSIENT &e)
    {
      if (server_nr == 2)
       {
          ACE_DEBUG((LM_DEBUG,
                     ACE_TEXT ("%D Expected exception for server %d: %C\n"),
                     server_nr,
                     e._info().c_str()));
        }
      else
        {
          ACE_ERROR((LM_ERROR,
                     ACE_TEXT ("%D Unexpected exception for server %d: %C\n"),
                     server_nr,
                     e._info().c_str()));
        }
    }
  catch (...)
    {
      ACE_ERROR((LM_ERROR,
                   ACE_TEXT ("%D Unexpected exception for server %d\n"),
                   server_nr));
    }


  if (server_nr == 2)
    barrier.wait();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%D Shutdown server %d\n"),
              server_nr));

  if (!CORBA::is_nil(hello.in ()))
    {
      try
        {
          hello->shutdown ();
        }
      catch (...)
        {
        }
     }

  if (server_nr == 1)
    barrier.wait();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%D Thread ended for server %d\n"),
              server_nr));

  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  int extra_argc = 4;

  try
    {
      if (parse_args (argc, argv) != 0)
        return 1;

      if (debug_level > 0)
        extra_argc = 6;

      ACE_TCHAR **extra = 0;
      ACE_NEW_RETURN (extra, ACE_TCHAR *[extra_argc], -1);
      extra[0] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective"));
      extra[1] = ACE::strnew (ACE_TEXT ("static Resource_Factory \"-ORBConnectionCacheMax 1 -ORBConnectionCachePurgePercentage 100\""));
      extra[2] = ACE::strnew (ACE_TEXT ("-ORBSvcConfDirective"));
      extra[3] = ACE::strnew (ACE_TEXT ("static Client_Strategy_Factory \"-ORBTransportMuxStrategy EXCLUSIVE\""));
      if (debug_level > 0)
        {
          extra[4] = ACE::strnew (ACE_TEXT ("-ORBdebuglevel"));
          extra[5] = ACE::strnew (ACE_TEXT ("10"));
        }

      ACE_TCHAR **largv = new ACE_TCHAR *[argc+extra_argc];
      for (int i = 0; i < argc; i++)
        largv[i] = argv[i];

      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%D client adding args:\n")));
      for (int i = 0; i < extra_argc; i++)
        {
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\t\t%s\n"), extra[i]));
          largv[argc+i] = extra[i];
        }

      argc += extra_argc;

      orb = CORBA::ORB_init (argc, largv);

      if (ACE_Thread_Manager::instance ()->spawn
          ((ACE_THR_FUNC) test_server,
           (void *) 1,
           THR_NEW_LWP | THR_JOINABLE,
           0) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("Error spawning thread 1\n")),
                            1);
        }

      ACE_OS::sleep(2);

      if (ACE_Thread_Manager::instance ()->spawn
          ((ACE_THR_FUNC) test_server,
           (void *) 2,
           THR_NEW_LWP | THR_JOINABLE,
           0) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                            ACE_TEXT ("Error spawning thread 2")),
                            1);
        }

      ACE_Thread_Manager::instance()->wait ();

      ACE_DEBUG((LM_DEBUG,
                 ACE_TEXT ("%D Transport data when done: current size: %d\n"),
                 orb->orb_core ()->lane_resources ().transport_cache ().current_size ()));

      orb->destroy ();
      for (int i = 0; i < extra_argc; i++)
        ACE::strdelete (extra[i]);
      delete [] extra;
      delete [] largv;
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Exception caught:");
      return 1;
    }

  return 0;
}
