// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Thread_Pool_Reactor_Resume_Test.cpp
//
// = DESCRIPTION
//      This program is an additional torture test of thread pool
//      reactors.  This test  is based on Thread_Pool_Reactor_Test.cpp
//      in $ACE_ROOT/tests. This test differs from the other one
//      slightly. The TP reactor is instantiated with the
//      with a value of 1 for the <resume_flag> argument. The server
//      threads during the  handle_input call resumes the handle that
//      would have been suspended by the reactor.
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
//   Balachandran Natarajan <bala@cs.wustl.edu>
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

#if defined (ACE_HAS_THREADS)

#include "tests/Thread_Pool_Reactor_Resume_Test.h"
typedef ACE_Strategy_Acceptor <Request_Handler, ACE_SOCK_ACCEPTOR> ACCEPTOR;

// Accepting end point.  This is actually "localhost:10010", but some
// platform couldn't resolve the name so we use the IP address
// directly here.
static const ACE_TCHAR *rendezvous = ACE_TEXT ("127.0.0.1:10010");

// Total number of server threads.
static size_t svr_thrno = ACE_MAX_THREADS;


#if defined (CHORUS) // Add platforms that can't handle too many
                     // connection simultaneously here.
#define ACE_LOAD_FACTOR /2
#else
#define ACE_LOAD_FACTOR
#endif

// Total number of client threads.
static size_t cli_thrno = ACE_MAX_THREADS ACE_LOAD_FACTOR;

// Total connection attemps of a client thread.
static size_t cli_conn_no = ACE_MAX_ITERATIONS ACE_LOAD_FACTOR;

// Total requests a client thread sends.
static size_t cli_req_no = ACE_MAX_THREADS ACE_LOAD_FACTOR;

// Delay before a thread sending the next request (in msec.)
static int req_delay = 50;

static void
parse_arg (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT ("r:s:c:d:i:n:"));

  int c;

  while ((c = getopt ()) != -1)
    {
      switch (c)
        {
        case 'r':                   // hostname:port
          rendezvous = getopt.opt_arg ();
          break;
        case 's':
          svr_thrno = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'c':
          cli_thrno = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'd':
          req_delay = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'i':
          cli_conn_no = ACE_OS::atoi (getopt.opt_arg ());
          break;
        case 'n':
          cli_req_no = ACE_OS::atoi (getopt.opt_arg ());
          break;
        default:
          ACE_ERROR ((LM_ERROR,
                      "Usage: Thread_Pool_Reactor_Resume_Test [-r <hostname:port#>]"
                      "\t[-s <server thr#>] [-c <client thr#>] [-d <delay>]"
                      "\t[-i <client conn attempt#>]"
                      "[-n <client request# per conn>]\n"));
          break;
        }
    }
}

Request_Handler::Request_Handler (ACE_Thread_Manager *thr_mgr)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH> (thr_mgr),
    nr_msgs_rcvd_(0),
    ref_count_ (1),
    refcount_lock_ (0)
{
  // Make sure we use TP_Reactor with this class (that's the whole
  // point, right?)
  this->reactor (ACE_Reactor::instance ());

  // Create the lock
  ACE_NEW (refcount_lock_,
           ACE_Lock_Adapter<ACE_SYNCH_MUTEX>);
}

Request_Handler::~Request_Handler (void)
{
  delete this->refcount_lock_;
}


int
Request_Handler::resume_handler (void)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) resume_handler () called \n")));
  this->_decr_ref_count ();

  if (this->ref_count_ == 0)
    this->destroy ();
  return 1;
}


int
Request_Handler::handle_input (ACE_HANDLE fd)
{
  ACE_TCHAR buffer[BUFSIZ];
  ACE_TCHAR len = 0;
  ssize_t result = this->peer ().recv (&len, sizeof (ACE_TCHAR));


  if (result > 0
      && this->peer ().recv_n (buffer, len * sizeof (ACE_TCHAR))
          == ACE_static_cast (ssize_t, len * sizeof (ACE_TCHAR)))
    {
      ++this->nr_msgs_rcvd_;


      // Now the handle_input method has done what it can do, namely
      // read the data from the socket we can just resume the handler
      // at this point
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) svr input; fd: 0x%x; input: %s\n",
                  fd,
                  buffer));
      if (ACE_OS::strcmp (buffer, ACE_TEXT ("shutdown")) == 0)
          ACE_Reactor::end_event_loop ();

      this->_incr_ref_count ();
      this->reactor ()->resume_handler (fd);
      return 0;
    }
  else
    {

      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Errno is %d  and result is %d\n",
                  errno, result));
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) Request_Handler: 0x%x peer closed (0x%x)\n",
                  this, fd));
    }
  return -1;
}

int
Request_Handler::handle_close (ACE_HANDLE fd, ACE_Reactor_Mask)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) svr close; fd: 0x%x, rcvd %d msgs\n",
              fd,
              this->nr_msgs_rcvd_));
  if (this->nr_msgs_rcvd_ != cli_req_no)
    ACE_ERROR((LM_ERROR,
               "(%t) Handler 0x%x: Expected %d messages; got %d\n",
               this,
               cli_req_no,
               this->nr_msgs_rcvd_));
  this->_decr_ref_count ();

  if (this->ref_count_ == 0)
    this->destroy ();
  return 0;
}

void
Request_Handler::_incr_ref_count (void)
{
  ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->refcount_lock_));
  ++this->ref_count_;
}

void
Request_Handler::_decr_ref_count (void)
{
  ACE_MT (ACE_GUARD (ACE_Lock, guard, *this->refcount_lock_));
  --this->ref_count_;
}

static int
reactor_event_hook (ACE_Reactor *)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) handling events ....\n"));

  return 0;
}

static void *
svr_worker (void *)
{
  // Server thread function.
  int result =
    ACE_Reactor::instance ()->run_reactor_event_loop (&reactor_event_hook);

  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "(%t) %p\n",
                       "Error handling events"),
                      0);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) I am done handling events. Bye, bye\n"));

  return 0;
}

static void *
cli_worker (void *arg)
{
  // Client thread function.
  ACE_INET_Addr addr (rendezvous);
  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connect;
  ACE_Time_Value delay (0, req_delay);
  size_t len = * ACE_reinterpret_cast (ACE_TCHAR *, arg);

  for (size_t i = 0 ; i < cli_conn_no; i++)
    {
      if (connect.connect (stream, addr) < 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%t) %p\n",
                      "connect"));
          continue;
        }

      for (size_t j = 0; j < cli_req_no; j++)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) conn_worker handle = %x, req = %d\n",
                      stream.get_handle (),
                      j+1));
          if (stream.send_n (arg,
                             (len + 1) * sizeof (ACE_TCHAR)) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          "(%t) %p\n",
                          "send_n"));
              continue;
            }
          ACE_OS::sleep (delay);
        }

      stream.close ();
    }

  return 0;
}

static void *
worker (void *)
{
  ACE_OS::sleep (3);
  const ACE_TCHAR *msg = ACE_TEXT ("Message from Connection worker");
  ACE_TCHAR buf [BUFSIZ];
  buf[0] = ACE_OS::strlen (msg) + 1;
  ACE_OS::strcpy (&buf[1], msg);

  ACE_INET_Addr addr (rendezvous);

  ACE_DEBUG((LM_DEBUG,
             "(%t) Spawning %d client threads...\n",
             cli_thrno));
  int grp = ACE_Thread_Manager::instance ()->spawn_n (cli_thrno,
                                                      &cli_worker,
                                                      buf);
  ACE_ASSERT (grp != -1);

  ACE_Thread_Manager::instance ()->wait_grp (grp);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Client threads done; shutting down...\n"));
  ACE_SOCK_Stream stream;
  ACE_SOCK_Connector connect;

  if (connect.connect (stream, addr) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p Error while connecting\n",
                "connect"));

  const ACE_TCHAR *sbuf = ACE_TEXT ("\011shutdown");

  ACE_DEBUG ((LM_DEBUG,
              "shutdown stream handle = %x\n",
              stream.get_handle ()));

  if (stream.send_n (sbuf, (ACE_OS::strlen (sbuf) + 1) * sizeof (ACE_TCHAR)) == -1)
    ACE_ERROR ((LM_ERROR,
                "(%t) %p\n",
                "send_n"));

  ACE_DEBUG ((LM_DEBUG,
              "Sent message of length  = %d\n",
              ACE_OS::strlen (sbuf)));
  stream.close ();

  return 0;
}

int
main (int argc, ACE_TCHAR *argv[])
{
  ACE_START_TEST (ACE_TEXT ("Thread_Pool_Reactor_Resume_Test"));
  parse_arg (argc, argv);

  // Changed the default
  ACE_TP_Reactor sr;


  ACE_Reactor new_reactor (&sr);
  ACE_Reactor::instance (&new_reactor);

  ACCEPTOR acceptor;
  ACE_INET_Addr accept_addr (rendezvous);

  if (acceptor.open (accept_addr) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("open")),
                      1);

  ACE_DEBUG((LM_DEBUG,
             ACE_TEXT ("(%t) Spawning %d server threads...\n"),
             svr_thrno));
  ACE_Thread_Manager::instance ()->spawn_n (svr_thrno,
                                            svr_worker);
  ACE_Thread_Manager::instance ()->spawn (worker);

  ACE_Thread_Manager::instance ()->wait ();

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Accept_Strategy<Request_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Concurrency_Strategy<Request_Handler>;
template class ACE_Creation_Strategy<Request_Handler>;
template class ACE_Scheduling_Strategy<Request_Handler>;
template class ACE_Acceptor<Request_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Strategy_Acceptor<Request_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>;
template class ACE_Lock_Adapter<ACE_SYNCH_MUTEX>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate  ACE_Accept_Strategy<Request_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate  ACE_Concurrency_Strategy<Request_Handler>
#pragma instantiate  ACE_Creation_Strategy<Request_Handler>
#pragma instantiate  ACE_Scheduling_Strategy<Request_Handler>
#pragma instantiate  ACE_Acceptor<Request_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate  ACE_Strategy_Acceptor<Request_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate  ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_MT_SYNCH>
#pragma instantiate ACE_Lock_Adapter<ACE_SYNCH_MUTEX>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else
int
main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Thread_Pool_Reactor_Test"));

  ACE_ERROR ((LM_INFO,
              "threads not supported on this platform\n"));

  ACE_END_TEST;
  return 0;
}
#endif /* ACE_HAS_THREADS */
