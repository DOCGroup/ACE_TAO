/**
 * @file Bug_2740_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 2740
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=2740
 *
 * @author paolo.volpi@tvblob.com
 */

#include "test_config.h"

#if defined (ACE_HAS_DEV_POLL) || defined (ACE_HAS_EVENT_POLL)

#include "ace/Atomic_Op.h"
#include "ace/Reactor.h"
#include "ace/Dev_Poll_Reactor.h"
#include "ace/Connector.h"
#include "ace/SOCK_Connector.h"
#include "ace/Acceptor.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/Signal.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_string.h"

const ACE_Time_Value MAX_CLIENT_TIMEOUT(30);  // 30 secs

// The number of clients we'll run. When the ClientsLeft gets back to 0 (it's
// decremented when a ClientSvcHandler is deleted) the event loop can stop.
const long NrClients = 10;
static ACE_Atomic_Op<ACE_Thread_Mutex, long> ClientsLeft (10);


class ClientSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  // Default ctor needed to satisfy ACE_Connector template, but not used.
  ClientSvcHandler ()
  { ACE_ERROR((LM_ERROR, ACE_TEXT ("%t: Wrong ClientSvcHandler ctor!!\n"))); }

  // Each client will send/recv 'echo_cnt' times, close/reopen the socket,
  // then echo, etc. for ACE_MAX_ITERATIONS times.
  ClientSvcHandler (const ACE_INET_Addr &addr, int echo_cnt);
  ~ClientSvcHandler ();

  int open (void* factory);
  int handle_input (ACE_HANDLE handle = ACE_INVALID_HANDLE);
  int handle_timeout (const ACE_Time_Value &now, const void *act = 0);
  int handle_close (ACE_HANDLE handle = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask mask = 0);

protected:
  static const char *send_str;

  ACE_INET_Addr server_addr_;
  // Know when we've received 'echo_cnt' echoes of the data. Don't want to
  // get into record-marking, short-reads/writes, etc. so just count the number
  // of bytes it takes to echo the data. Don't care about verifying the
  // content - this is mostly a stress test for multithreaded reactor.
  size_t echo_cnt_;
  size_t iter_;
  unsigned long timer_;
};

const char *ClientSvcHandler::send_str =
  "1234567890123456789012345678901234567890";

class ServerSvcHandler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:
  int open (void* factory);
  int handle_input (ACE_HANDLE handle = ACE_INVALID_HANDLE);
  int handle_close (ACE_HANDLE handle = ACE_INVALID_HANDLE,
                    ACE_Reactor_Mask mask = 0);
};


ClientSvcHandler::ClientSvcHandler (const ACE_INET_Addr &addr, int echo_cnt)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>(),
    server_addr_ (addr),
    echo_cnt_ (ACE_OS::strlen (ClientSvcHandler::send_str) * echo_cnt),
    iter_ (0)
{
}

ClientSvcHandler::~ClientSvcHandler ()
{
  if (--ClientsLeft == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%t: No more clients; ending reactor loop\n")));
      this->reactor ()->end_reactor_event_loop ();
    }
}

int
ClientSvcHandler::open (void* factory)
{
  ++iter_;
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: client h %d, open for iteration %B\n"),
              this->peer ().get_handle (), this->iter_));

  if (ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open (factory) == 0)
    {
      this->timer_ = this->reactor ()->schedule_timer (this,
                                                       0,
                                                       MAX_CLIENT_TIMEOUT);

      size_t send_len = ACE_OS::strlen (ClientSvcHandler::send_str);
      if (-1 == this->peer().send_n (ClientSvcHandler::send_str, send_len))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%t: client h %d, %p\n"),
                           this->peer ().get_handle(),
                           ACE_TEXT ("send")),
                          -1);
      return 0;
    }

  ACE_ERROR_RETURN ((LM_ERROR,
                     ACE_TEXT ("%t: %p\n"),
                     ACE_TEXT ("Client open")),
                    -1);
}

int
ClientSvcHandler::handle_input (ACE_HANDLE handle)
{
    // Get socket data
    char buffer[ACE_DEFAULT_MAX_SOCKET_BUFSIZ];
    ssize_t bc;
    bc = this->peer ().recv (buffer, sizeof (buffer));
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("%t: client h %d recv %b bytes\n"),
                handle,
                bc));
    if (bc > 0)
      {
        // Reset Client timeout timer
        this->reactor ()->cancel_timer (this->timer_);

        // Reduce the echo count by what was received; if not there, send
        // more data and await more echo.
        size_t ubc = (size_t)bc;
        if (ubc <= this->echo_cnt_)
          this->echo_cnt_ -= ubc;
        if (this->echo_cnt_ == 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("%t: client h %d end of echo\n"),
                        handle));
            return -1;
          }

        // Not done yet; send more data and resched timer.
        size_t send_len = ACE_OS::strlen (ClientSvcHandler::send_str);
        if (-1 == this->peer().send_n (ClientSvcHandler::send_str, send_len))
          ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%t: client h %d: %p\n"),
                             ACE_TEXT ("resending")),
                            -1);
        this->timer_ = reactor ()->schedule_timer (this, 0, MAX_CLIENT_TIMEOUT);
      }
    else if (bc == 0) // Socket was closed by server
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%t: client h %d: closed by server\n"),
                           handle),
                          -1);
      }
    else if (errno == EWOULDBLOCK) // no data ready on socket
      {
        ACE_ERROR_RETURN ((LM_DEBUG,
                           ACE_TEXT ("%t: client h %d: recv no data\n"),
                           handle),
                          0);
      }
    else
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%t: client h %d: %p\n"),
                         handle,
                         ACE_TEXT ("send")),
                        -1);

    return 0;
}

int
ClientSvcHandler::handle_timeout (const ACE_Time_Value &, const void*)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%t: client h %d: timeout\n"),
              this->peer ().get_handle ()));
  reactor()->remove_handler (this, ACE_Event_Handler::ALL_EVENTS_MASK);
  return 0;
}

int
ClientSvcHandler::handle_close (ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%t: client h %d handle_close\n"),
              handle));

  // If not done iterating, just close the socket and reopen the connection.
  // Else shut down and delete.
  if (this->iter_ >= ACE_MAX_ITERATIONS)
    return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::handle_close(handle,
                                                                          mask);
  this->shutdown ();
  ClientSvcHandler *p = const_cast<ClientSvcHandler *>(this);
  ACE_Connector<ClientSvcHandler, ACE_SOCK_CONNECTOR>
    connector (this->reactor (), ACE_NONBLOCK);
  connector.connect (p, this->server_addr_);
  return 0;
}

int
ServerSvcHandler::open (void* factory)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: server h %d open\n"),
              this->peer ().get_handle ()));

  return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::open (factory);
}

int
ServerSvcHandler::handle_input (ACE_HANDLE handle)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: server h %d input\n"), handle));

  // Receive whatever is here and send it back.
  char buffer[ACE_DEFAULT_MAX_SOCKET_BUFSIZ];
  ssize_t bc;
  bc = this->peer ().recv (buffer, sizeof (buffer));
  if (bc > 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%t: server h %d recv %b bytes\n"),
                  handle,
                  bc));
      if (-1 == this->peer ().send_n (buffer, bc))
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%t: server h %d: %p\n"),
                           handle, ACE_TEXT ("send")),
                          -1);
    }
  else if (bc == 0 || errno == ECONNRESET) // Socket was closed by client
    {
      ACE_ERROR_RETURN ((LM_DEBUG,
                         ACE_TEXT ("%t: server h %d: closed by client\n"),
                         handle),
                        -1);
    }
  else if (errno == EWOULDBLOCK) // no data ready on socket
    {
      // The reactor shouldn't call back for input if there's no data, so
      // log an error, but keep running.
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%t: server h %d: recv no data\n"),
                         handle),
                        0);
    }
  else
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%t: server h %d: %p\n"),
                       handle,
                       ACE_TEXT ("recv")),
                      -1);

  return 0;
}

int
ServerSvcHandler::handle_close(ACE_HANDLE handle, ACE_Reactor_Mask mask)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: server h %d: handle_close\n"), handle));
  return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::handle_close (handle,
                                                                         mask);
}

static int
disable_signal (int sigmin, int sigmax)
{
#if !defined (ACE_LACKS_UNIX_SIGNALS)
  sigset_t signal_set;
  if (ACE_OS::sigemptyset (&signal_set) == - 1)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Error: (%P|%t):%p\n"),
                ACE_TEXT ("sigemptyset failed")));

  for (int i = sigmin; i <= sigmax; i++)
    ACE_OS::sigaddset (&signal_set, i);

  // Put the <signal_set>.
# if defined (ACE_LACKS_PTHREAD_THR_SIGSETMASK)
  // In multi-threaded application this is not POSIX compliant
  // but let's leave it just in case.
  if (ACE_OS::sigprocmask (SIG_BLOCK, &signal_set, 0) != 0)
# else
  if (ACE_OS::thr_sigsetmask (SIG_BLOCK, &signal_set, 0) != 0)
# endif /* ACE_LACKS_PTHREAD_THR_SIGSETMASK */
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error: (%P|%t): %p\n"),
                       ACE_TEXT ("SIG_BLOCK failed")),
                      -1);
#else
  ACE_UNUSED_ARG (sigmin);
  ACE_UNUSED_ARG (sigmax);
#endif /* ACE_LACKS_UNIX_SIGNALS */

  return 0;
}

ACE_THR_FUNC_RETURN
event_loop(void *arg)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: Starting reactor event loop\n")));

  disable_signal (SIGPIPE, SIGPIPE);

  ACE_Reactor *reactor = static_cast<ACE_Reactor*>(arg);
  int s = reactor->run_reactor_event_loop();

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%t: reactor loop done; status %d\n"), s));
  return 0;
}


int run_main(int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2740_Regression_Test"));

  // Make sure we ignore SIGPIPE
  disable_signal (SIGPIPE, SIGPIPE);

  ACE_Dev_Poll_Reactor dp_reactor;
  dp_reactor.restart (1);          // Restart on EINTR
  ACE_Reactor reactor (&dp_reactor);

  // Bind acceptor to any port and then find out what the port was.
  ACE_Acceptor<ServerSvcHandler, ACE_SOCK_Acceptor> acceptor (&reactor,
                                                              ACE_NONBLOCK);
  ACE_SOCK_Acceptor::PEER_ADDR server_addr;
  if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &), &reactor) == -1
      || acceptor.acceptor ().get_local_addr (server_addr) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%P|%t) %p\n"),
                         ACE_TEXT ("acceptor open")),
                        1);
    }

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%P|%t) starting server at port %d\n"),
              server_addr.get_port_number ()));

  ACE_Thread_Manager::instance ()->spawn_n (ACE_MAX_THREADS,
                                            event_loop,
                                            &reactor);

  ACE_Connector<ClientSvcHandler, ACE_SOCK_CONNECTOR> connector (&reactor,
                                                                 ACE_NONBLOCK);
  // Spin up clients to connect and iterate
  long i;
  for (i = 0; i < NrClients; ++i)
    {
      ClientSvcHandler *c = new ClientSvcHandler (server_addr, i % 4);
      connector.connect(c, server_addr);
    }

  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%d clients connected\n"), (int)i));
  ACE_Thread_Manager::instance ()->wait ();
  ACE_END_TEST;
  return 0;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2740_Regression_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Dev Poll and Event Poll are not supported ")
              ACE_TEXT ("on this platform\n")));
  ACE_END_TEST;
  return 0;
}

#endif  /* ACE_HAS_DEV_POLL || ACE_HAS_EVENT_POLL */
