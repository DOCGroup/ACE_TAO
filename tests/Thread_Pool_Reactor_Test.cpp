// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Thread_Pool_Reactor_Test.cpp
//
// = DESCRIPTION
//      This program is a torture test of thread poll reactors.
//      It starts by spawning several server threads waiting to handle
//      events.  Several other client threads are spawned right after
//      to initiate connections to server threads.  Each connection
//      adds a new Svc_Handler into the TP_Reactor and sends out
//      several "requests" to the server thread.  After the connection
//      is closed, the Svc_Handler is removed from the TP_Reactor.
//      Each message is treated as a separate request by the server so
//      two consecutive requests might be serviced by two different
//      threads.
//
//      Usage: Thread_Pool_Reactor_Test [-r <hostname:port#>]
//                [-s <server thr#>] [-c <client thr#>] [-d <delay>]
//                [-i <client conn attempt#>] [-n <client request# per conn>]
//
//      Default value:
//          <hostname:port#>:       ACE_DEFAULT_RENDEZVOUS
//          <server thr#>:          ACE_MAX_THREADS
//          <client thr#>:          ACE_MAX_ITERATIONS
//          <client conn attempt#>: ACE_MAX_ITERATIONS
//          <client req# per conn>: ACE_MAX_THREADS
//          <delay>:                50 usec
//
// = AUTHOR
//      Irfan Pyarali <irfan@cs.wustl.edu>
//      Nanbor Wang <nanbor@cs.wustl.edu>
//
// ============================================================================

#include "tests/test_config.h"
#include "ace/Get_Opt.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Acceptor.h"
#include "ace/Thread_Manager.h"
#include "ace/TP_Reactor.h"

ACE_RCSID(tests, Atomic_Op_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

#if defined (ACE_HAS_THREADS)

static ASYS_TCHAR *rendezvous = ASYS_TEXT ("localhost:10010");
static size_t svr_thrno = ACE_MAX_THREADS;
static size_t cli_thrno = ACE_MAX_ITERATIONS;
static size_t cli_conn_no = ACE_MAX_ITERATIONS;
static size_t cli_req_no = ACE_MAX_THREADS;
static int req_delay = 50;
static int main_event_loop = 1;
static ACE_Reactor *main_reactor = 0;

void
parse_arg (int argc, ASYS_TCHAR *argv[])
{
  // @@ TODO: Support command line arguments stated above.
  ACE_Get_Opt getopt (argc, argv, ASYS_TEXT ("r:s:c:d:i:n:"));

  int c;

  while ((c = getopt ()) != -1)
    {
      switch (c)
        {
        case 'r':                   // hostname:port
          rendezvous = getopt.optarg;
          break;
        case 's':
          svr_thrno = ACE_OS::atoi (getopt.optarg);
          break;
        case 'c':
          cli_thrno = ACE_OS::atoi (getopt.optarg);
          break;
        case 'd':
          req_delay = ACE_OS::atoi (getopt.optarg);
          break;
        case 'i':
          cli_conn_no = ACE_OS::atoi (getopt.optarg);
          break;
        case 'n':
          cli_req_no = ACE_OS::atoi (getopt.optarg);
          break;
        default:
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("Invalid command line argument: %c\n"), c));
          break;
        }
    }
}

class Acceptor_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
{
  // = TITLE
  //   This class is the Svc_Handler used by <Acceptor>.
public:
  Acceptor_Handler (ACE_Thread_Manager *tm = 0);
  // The default constructor makes sure the right reactor is used.

protected:
  virtual int handle_input (ACE_HANDLE fd = ACE_INVALID_HANDLE);
  virtual int handle_close (ACE_HANDLE fd, ACE_Reactor_Mask = 0);
};

typedef ACE_Strategy_Acceptor <Acceptor_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;

Acceptor_Handler::Acceptor_Handler (ACE_Thread_Manager *thr_mgr)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> (thr_mgr)
{
  // Make sure we use TP_Reactor with this class (that's the whole
  // point, right?)
  this->reactor (ACE_Reactor::instance ());
}

int
Acceptor_Handler::handle_input (ACE_HANDLE fd)
{
  ASYS_TCHAR buffer[BUFSIZ];
  int result = this->peer ().recv (buffer, BUFSIZ * sizeof (ASYS_TCHAR);

  if (result > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) Acceptor_Handler::handle_input (fd = %x)\n"),
                  fd));
      buffer[result] = '\0';
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%t) handle_input: input is %s\n"),
                  buffer));
      if (ACE_OS::strcmp (buffer, ASYS_TEXT ("shutdown")) == 0)
        {
          main_event_loop = 0;
          main_reactor->notify ();
          ACE_Reactor::end_event_loop ();
        }
      return 0;
    }
  else
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("(%t) Acceptor_Handler: end handle input (%x)\n"),
                fd));
  return -1;
}

int
Acceptor_Handler::handle_close (ACE_HANDLE fd, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%t) Acceptor_Handler::handle_close (fd = %x)\n"),
              fd));
  this->destroy ();
  return 0;
}

void *
svr_worker (void *)
{
  // Server thread function.

  while (!ACE_Reactor::event_loop_done ())
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) handling events ....\n"));

      if (ACE_Reactor::instance ()->handle_events () == -1)
        ACE_DEBUG ((LM_DEBUG,
                    "(%t) Error handling events\n"));
    }

  ACE_DEBUG ((LM_DEBUG,
              "(%t) I am done handling events. Bye, bye\n"));

  return 0;
}

void *
cli_worker (void *)
{
  // Client thread function.
  ACE_INET_Addr addr (rendezvous);
  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connect;
  ACE_Time_Value delay (0, req_delay);

  for (size_t i = 0 ; i < cli_conn_no; i++)
    {
      if (connect.connect (stream, addr) < 0)
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("(%t) %p\n"),
                      ASYS_TEXT ("connect")));
          continue;
        }

      ASYS_TCHAR *buf = ASYS_TEXT ("Message from Connection worker\n");

      for (size_t j = 0; j < cli_req_no; j++)
        {
          ACE_DEBUG ((LM_DEBUG, 
                      ASYS_TEXT ("(%t) conn_worker stream handle = %x\n"),
                      stream.get_handle ()));
          stream.send_n (buf,
                         (ACE_OS::strlen (buf) + 1) * sizeof (ASYS_TCHAR));
          ACE_OS::sleep (delay);
        }

      stream.close ();
    }

  return 0;
}

void *
worker (void *)
{
  ACE_OS::sleep (3);

  ACE_INET_Addr addr (rendezvous);

  int grp = ACE_Thread_Manager::instance ()->spawn_n (cli_thrno,
                                                      &cli_worker,
                                                      &addr);
  ACE_ASSERT (grp != -1);

  ACE_Thread_Manager::instance ()->wait_grp (grp);

  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("Shutting down...\n")));
  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connect;

  if (connect.connect (stream, addr) < 0)
    ACE_ERROR ((LM_ERROR,
                ASYS_TEXT ("%p Error while connecting\n"),
                ASYS_TEXT ("connect")));

  char *buf = "shutdown";
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("shutdown stream handle = %x\n"),
              stream.get_handle ()));

  stream.send_n (buf, ACE_OS::strlen (buf) + 1);
  stream.close ();

  return 0;
}

int
main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Thread_Pool_Reactor_Test"));
  parse_arg (argc, argv);

  // Changed the default
  ACE_TP_Reactor sr;
  ACE_Reactor new_reactor (&sr);
  ACE_Reactor::instance (&new_reactor);

  // Most platforms seem to have trouble accepting connections
  // simultaneously in multiple threads.  Therefore, we can't quite
  // use the Acceptor with the TP_Reactor.  Create a Select_Reactor
  // and run the event_loop in the main thread.
  ACE_Select_Reactor slr;
  ACE_Reactor mreactor (&slr);
  main_reactor = &mreactor;
  ACCEPTOR acceptor;
  ACE_INET_Addr accept_addr (rendezvous);
  if (acceptor.open (accept_addr,
                     main_reactor) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("open")),
                      1);

  ACE_Thread_Manager::instance ()->spawn_n (svr_thrno,
                                            svr_worker);
  ACE_Thread_Manager::instance ()->spawn (worker);

  int result = 0;

  while (result >= 0 && main_event_loop)
    result = slr.handle_events ();

  ACE_ASSERT (result != -1);

  ACE_Thread_Manager::instance ()->wait ();

  ACE_OS::sleep (1);

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Accept_Strategy<Acceptor_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Concurrency_Strategy<Acceptor_Handler>;
template class ACE_Creation_Strategy<Acceptor_Handler>;
template class ACE_Scheduling_Strategy<Acceptor_Handler>;
template class ACE_Acceptor<Acceptor_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Strategy_Acceptor<Acceptor_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate  ACE_Accept_Strategy<Acceptor_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate  ACE_Concurrency_Strategy<Acceptor_Handler>
#pragma instantiate  ACE_Creation_Strategy<Acceptor_Handler>
#pragma instantiate  ACE_Scheduling_Strategy<Acceptor_Handler>
#pragma instantiate  ACE_Acceptor<Acceptor_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate  ACE_Strategy_Acceptor<Acceptor_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate  ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, ASYS_TCHAR *[])
{
  ACE_START_TEST (ASYS_TEXT ("Thread_Pool_Reactor_Test"));

  ACE_ERROR ((LM_ERROR, ASYS_TEXT ("threads not supported on this platform\n")));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */
