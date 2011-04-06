
//=============================================================================
/**
 *  @file    Reactor_Fairness_Test.cpp
 *
 *  $Id$
 *
 *  This test is used to time the dispatching mechanisms of the
 *  <ACE_Reactor>s. Both the <ACE_WFMO_Reactor> and
 *  <ACE_Select_Reactor> can be tested.
 *
 *
 *  @author Irfan Pyarali <irfan@cs.wustl.edu>
 */
//=============================================================================


#include "test_config.h"
#include "Reactor_Fairness_Test.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/WFMO_Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/TP_Reactor.h"
#include "ace/Auto_Ptr.h"
#include "ace/Numeric_Limits.h"
#include "ace/Signal.h"
#include "ace/Atomic_Op.h"
#include "ace/Thread_Mutex.h"



#if defined (ACE_HAS_THREADS)

namespace {

  const char ACE_ALPHABET[] = "abcdefghijklmnopqrstuvwxyz";

  // Number of connections to run
  int opt_nconnections = 5;

  // How many seconds to run the test on each reactor
  int opt_secs = 30;

  // How many thread to run in the reactor loop
  int opt_reactor_threads = 3;

  // Extra debug messages
  int opt_debug = 0;

  ACE_Atomic_Op<ACE_Thread_Mutex, int> reactor_thread_nr = 0;

  // Class to collect and report on data handling for each test pass.
  struct Result_Set {
    int nr_conns;
    typedef ACE_Array_Map<ACE_HANDLE, unsigned int> report_map;
    report_map reports;

    void reset (int n_connections)   // Reset for next run
    {
      reports.clear ();
      nr_conns = n_connections;
    }

    void report (ACE_HANDLE h, unsigned int chunks)
    {
      std::pair<ACE_HANDLE, unsigned int> newval (h, chunks);
      reports.insert (newval);
    }

    // Return 1 if this looks like a failure wrt fairness.
    int analyze_reports (void)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Results (%d entries):\n"),
                  reports.size()));
      unsigned int max_chunks = 0;
      unsigned int min_chunks = ACE_Numeric_Limits<unsigned int>::max();
      for (report_map::iterator iter = reports.begin();
           iter != reports.end ();
           ++iter)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("   handle %d: %u\n"),
                      (*iter).first, (*iter).second));
          if ((*iter).second > max_chunks)
            max_chunks = (*iter).second;
          if ((*iter).second < min_chunks)
            min_chunks = (*iter).second;
        }
      if ((max_chunks - min_chunks) > max_chunks / 10)
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("Too much unfairness (max %u, min %u)\n"),
                           max_chunks,
                           min_chunks),
                          1);
      return 0;
    }
  };
  Result_Set results;
}

// Handle incoming data
int
Read_Handler::handle_input (ACE_HANDLE h)
{
  char buf[BUFSIZ];
  ssize_t result = this->peer ().recv (buf, ACE_OS::strlen(ACE_ALPHABET));
  if (opt_debug)
    ACE_DEBUG((LM_DEBUG,
               ACE_TEXT ("(%t) Read_Handler::handle_input h %d, result %b\n"),
               h, result));
  if (result > 0)
    {
      if (opt_debug)
        {
          buf[result] = 0;
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Read_Handler::handle_input: h %d: %C\n"),
                      h,
                      buf));
        }
      ++this->chunks_in;
    }
  else if (result < 0)
    {
      if (errno == EWOULDBLOCK)
        return 0;
      else
        {
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("handle_input: h %d: %p (errno: %d)\n"),
                      h, ACE_TEXT ("recv"), ACE_ERRNO_GET));

          // This will cause handle_close to get called.
          return -1;
        }
    }
  else // result == 0
    {
      if (opt_debug)
        ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Handle %d closing\n"), h));
      // This will cause handle_close to get called.
      return -1;
    }

  return 0;
}

// Handle connection shutdown.

int
Read_Handler::handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask /*close_mask*/)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Read_Handler handle %d close; %u chunks\n"),
              handle, chunks_in));
  results.report (handle, this->chunks_in);

  // Shutdown
  this->destroy ();
  return 0;
}

// Pump data as fast as possible to all the sockets.
ACE_THR_FUNC_RETURN
sender (void *arg)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) running sender\n")));

  // Ensure an error, not a signal, on broken pipe.
  ACE_Sig_Action no_sigpipe ((ACE_SignalHandler) SIG_IGN);
  ACE_Sig_Action original_action;
  no_sigpipe.register_action (SIGPIPE, &original_action);

  ACE_INET_Addr *connection_addr =
    reinterpret_cast<ACE_INET_Addr *> (arg);

  int i;

  // Automagic memory cleanup.
  ACE_SOCK_Stream *temp_socks = 0;
  ACE_NEW_RETURN (temp_socks,
                  ACE_SOCK_Stream [opt_nconnections],
                  0);
  ACE_Auto_Basic_Array_Ptr <ACE_SOCK_Stream> socks (temp_socks);

  // Connection all <opt_nconnections> connections before sending data.
  ACE_SOCK_Connector c;
  for (i = 0; i < opt_nconnections; i++)
    {
      if (c.connect (socks[i], *connection_addr) == -1)
        {
          if (errno != ECONNREFUSED || i == 0)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("(%t) conn %d %p\n"),
                          ACE_TEXT ("connect")));
              while (--i >= 0)
                socks[i].close ();
              break;
            }
        }
      socks[i].enable (ACE_NONBLOCK);
    }
  if (i < opt_nconnections)
    return 0;

  // Keep blasting data on all possible connections until this thread
  // is canceled. If we manage to overrun the receiver on all sockets,
  // sleep a bit for the receivers to catch up.
  ACE_thread_t me = ACE_Thread::self ();
  ACE_Thread_Manager *tm = ACE_Thread_Manager::instance ();
  size_t send_cnt = ACE_OS::strlen (ACE_ALPHABET);
  bool fail = false;
  while (!tm->testcancel (me) && !fail)
    {
      bool sent_something = false;
      for (i = 0; i < opt_nconnections; i++)
        {
          ssize_t cnt = socks[i].send (ACE_ALPHABET, send_cnt);
          if (opt_debug)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("(%t) h %d sent %b\n"),
                        socks[i].get_handle(),
                        cnt));
          if (cnt > 0)
            {
              sent_something = true;
              continue;
            }
          if (errno == EWOULDBLOCK)
            continue;
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) %p; giving up\n"),
                      ACE_TEXT ("sender")));
          fail = true;
          break;
        }
      if (!fail && !sent_something)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Full sockets... pausing...\n")));
          ACE_OS::sleep (1);
          ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Resuming sending.\n")));
        }
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Done sending.\n")));
  for (i = 0; i < opt_nconnections; i++)
    socks[i].close ();
  return 0;
}

ACE_THR_FUNC_RETURN
reactor_loop (void *p)
{
  ACE_Reactor *r = reinterpret_cast<ACE_Reactor *> (p);
  int me = reactor_thread_nr++;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) Reactor loop %d starting...\n"), me));
  if (me == 0)
    r->owner (ACE_Thread::self ());
  if (r->run_reactor_event_loop () == -1)
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%t) %p\n"), ACE_TEXT ("reactor")));
  else
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) reactor thread %d ending\n"), me));
  return 0;
}

void
run (ACE_Reactor_Impl &ri, const ACE_TCHAR *what, bool tp = true)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Starting test with %s\n"), what));

  ACE_Reactor r (&ri);
  ACE_Thread_Manager *tm = ACE_Thread_Manager::instance ();
  ACE_Acceptor<Read_Handler, ACE_SOCK_ACCEPTOR> acceptor;

  // Bind acceptor to any port and then find out what the port was.
  ACE_INET_Addr server_addr;
  ACE_INET_Addr local_addr (ACE_sap_any_cast (const ACE_INET_Addr &));
  if (acceptor.open (local_addr, &r) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%t) %p\n"),
                  ACE_TEXT ("acceptor open")));
      return;
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) starting server at port %d\n"),
              server_addr.get_port_number ()));

  reactor_thread_nr = 0;   // Reset for new set
  if (-1 == tm->spawn_n (tp ? opt_reactor_threads : 1, reactor_loop, &r))
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("reactor thread spawn")));
      acceptor.close();
      return;
    }

  ACE_INET_Addr connection_addr (server_addr.get_port_number (),
                                 ACE_DEFAULT_SERVER_HOST);

  int sender_grp = tm->spawn (sender, &connection_addr);
  if (-1 == sender_grp)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("sender spawn")));
    }
  else
    {
      ACE_OS::sleep (opt_secs);
      tm->cancel_grp (sender_grp);
    }
  r.end_reactor_event_loop ();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) waiting for the test threads...\n")));
  tm->wait ();
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Reactor_Fairness_Test"));

  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("c:s:t:d"), 1);
  for (int c; (c = getopt ()) != -1; )
  //FUZZ: enble check_for_lack_ACE_OS
    switch (c)
      {
      case 'c':
        opt_nconnections = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 's':
        opt_secs = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 't':
        opt_reactor_threads = ACE_OS::atoi (getopt.opt_arg ());
        break;
      case 'd':
        opt_debug = 1;
        break;
      }

  // Run the test once for each reactor type available.
  int fails = 0;
  results.reset (opt_nconnections);
  {
    ACE_Select_Reactor r;
    run (r, ACE_TEXT ("Select Reactor"), false);  // No thread pool
  }
  fails += results.analyze_reports ();

  results.reset (opt_nconnections);
  {
    ACE_TP_Reactor r;
    run (r, ACE_TEXT ("TP Reactor"));
  }
  fails += results.analyze_reports ();

#if defined (ACE_HAS_EVENT_POLL) || defined (ACE_HAS_DEV_POLL)
  results.reset (opt_nconnections);
  {
    ACE_Dev_Poll_Reactor r;
    run (r, ACE_TEXT ("Dev_Poll Reactor"));
  }
  fails += results.analyze_reports ();
#endif /* ACE_HAS_EVENT_POLL || ACE_HAS_DEV_POLL */

#if defined (ACE_WIN32)
  results.reset (opt_nconnections);
  {
    ACE_WFMO_Reactor r;
    run (r, ACE_TEXT ("WFMO Reactor"));
  }
  fails += results.analyze_reports ();
#endif /* ACE_WIN32 */

  ACE_END_TEST;
  return fails;
}

#else
int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Reactor_Performance_Test"));

  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */
