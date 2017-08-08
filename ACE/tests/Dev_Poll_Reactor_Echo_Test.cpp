//=============================================================================
/**
 *  @file    Dev_Poll_Reactor_Echo_Test.cpp
 *
 *  This test implements a simple echo server using the
 *  Dev_Poll_Reactor.  This forces the reactor to behave like a
 *  reactor would in a typical client/server application, i.e.,
 *  receive a message then send a messages.
 *  @author Justin Wilson <wilsonj@ociweb.com>
 */
//=============================================================================

#include "test_config.h"

#if defined (ACE_HAS_DEV_POLL) || defined (ACE_HAS_EVENT_POLL)

#include "ace/OS_NS_signal.h"
#include "ace/Reactor.h"
#include "ace/Dev_Poll_Reactor.h"

#include "ace/Acceptor.h"
#include "ace/Connector.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"

#include "ace/OS_NS_unistd.h"
#include "ace/OS_NS_netdb.h"

#include <queue>

typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> SVC_HANDLER;

// ----------------------------------------------------

class Client : public SVC_HANDLER
{
public:

  Client (void);

  //FUZZ: disable check_for_lack_ACE_OS
  virtual int open (void * = 0);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int handle_output (ACE_HANDLE handle);

  virtual int handle_input (ACE_HANDLE handle);

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask mask);

  std::string sent;
  std::string received;

private:
  unsigned int call_count_;
};


class Server : public SVC_HANDLER
{
public:

  Server (void);

  virtual int handle_input (ACE_HANDLE handle);

  virtual int handle_output (ACE_HANDLE handle);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask mask);

private:
  int send_i (const char* buffer,
              size_t size);

  std::queue<std::string*> buffer_list_;
  size_t offset_;
};

// ----------------------------------------------------

Client::Client (void)
  : call_count_ (0)
{
}

int
Client::open (void *)
{
  // Trigger writes on a timer.
  ACE_Time_Value delay (1, 0);
  ACE_Time_Value restart (1, 0);
  if (this->reactor ()->schedule_timer (this,
                                        0,
                                        delay,
                                        restart) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) %p\n"),
                         ACE_TEXT ("Unable to schedule client side ")
                         ACE_TEXT ("timer in ACE_Dev_Poll_Reactor")),
                        -1);
    }

  if (this->reactor ()->register_handler (this, ACE_Event_Handler::READ_MASK) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("(%t) %p\n"),
                         ACE_TEXT ("Unable to register for reading ")
                         ACE_TEXT ("in ACE_Dev_Poll_Reactor")),
                        -1);
    }

  return 0;
}

int
Client::handle_output (ACE_HANDLE handle)
{
  std::string buffer = "Hello, world!";
  ssize_t bytes_sent = this->peer ().send (buffer.data (), buffer.size ());

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Client::handle_output; handle = %d")
              ACE_TEXT (" bytes sent %d\n"),
              handle,
              bytes_sent));

  if (bytes_sent == -1)
    {
      if (errno == EWOULDBLOCK)
        return 0;  // Flow control kicked in.
      else if (errno == EPIPE || errno == ECONNRESET)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Client::handle_output; server ")
                      ACE_TEXT ("closed handle %d\n"),
                      this->peer ().get_handle ()));
          return -1;
        }
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%t) %p\n"),
                           ACE_TEXT ("Client::handle_output")),
                          -1);
    }
  else if (bytes_sent == 0)
    return -1;
  else
    this->sent.append (buffer.substr (0, bytes_sent));

  return -1;
}

int
Client::handle_input (ACE_HANDLE handle)
{
  for (;;)
    {
      char buffer[BUFSIZ];
      ssize_t bytes_read = this->peer ().recv (buffer, BUFSIZ);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Client::handle_input handle = %d bytes_read = %d\n"),
                  handle, bytes_read));

      if (bytes_read == -1 && errno == EWOULDBLOCK)
        {
          return 0;
        }
      else if (bytes_read <= 0)
        {
          // Closed.
          return -1;
        }
      else
        {
          this->received.append (buffer, bytes_read);
        }
    }
}

int
Client::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%t) Expected client timeout occurred at: %T\n")));

  if (this->call_count_ != 10)
    {
      // Register for write.
      if (this->reactor ()->register_handler (this, ACE_Event_Handler::WRITE_MASK) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%t) %p\n"),
                             ACE_TEXT ("Unable to register for writing ")
                             ACE_TEXT ("in ACE_Dev_Poll_Reactor")),
                            -1);
        }
      this->call_count_++;
      return 0;
    }
  else
    {
      // Shutdown.
      if (this->reactor ()->end_reactor_event_loop () == 0)
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("(%t) Successful client reactor shutdown.\n")));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%t) %p\n"),
                    ACE_TEXT ("Failed client reactor shutdown")));

      // Force this service handler to be closed in either case.
      return -1;
    }
}

int
Client::handle_close (ACE_HANDLE handle,
                      ACE_Reactor_Mask mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Client::handle_close handle = %d mask = %xd\n"), handle, mask));
  return 0;
  //return SVC_HANDLER::handle_close (handle, mask);
}

// ----------------------------------------------------

Server::Server (void)
  : offset_ (0)
{
}

int
Server::handle_input (ACE_HANDLE handle)
{
  for (;;)
    {
      char buffer[BUFSIZ];
      ssize_t bytes_read = this->peer ().recv (buffer, BUFSIZ);
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Server::handle_input handle = %d bytes_read = %d\n"),
                  handle, bytes_read));

      if (bytes_read == -1 && errno == EWOULDBLOCK)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Server::handle_input handle = %d EWOULDBLOCK\n"),
                      handle));
          return 0;
        }
      else if (bytes_read == 0)
        {
          // Closed.
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("(%t) Server::handle_input handle = %d CLOSED\n"),
                      handle));
          return -1;
        }
      else
        {
          if (send_i (buffer, bytes_read) == -1)
            return -1;
        }
    }
}

int
Server::send_i (const char* buffer,
                size_t size)
{
  if (size == 0)
    {
      return 0;
    }

  if (buffer_list_.empty ())
    {
      // Register for write.
      if (this->reactor ()->register_handler (this, ACE_Event_Handler::WRITE_MASK) == -1)
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             ACE_TEXT ("(%t) %p\n"),
                             ACE_TEXT ("Unable to register for writing ")
                             ACE_TEXT ("in ACE_Dev_Poll_Reactor")),
                            -1);
        }
    }

  buffer_list_.push (new std::string (buffer, size));
  return 0;
}

int
Server::handle_output (ACE_HANDLE handle)
{
  while (!buffer_list_.empty ())
    {
      size_t bytes_to_send = buffer_list_.front ()->size () - offset_;
      ssize_t bytes_sent = this->peer ().send (buffer_list_.front ()->data () + offset_, bytes_to_send);

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("(%t) Server::handle_output; handle = %d")
                  ACE_TEXT (" bytes sent %d\n"),
                  handle, bytes_sent));

      if (bytes_sent == -1)
        {
          if (errno == EWOULDBLOCK)
            return 0;
          else if (errno == EPIPE || errno == ECONNRESET)
            {
              ACE_DEBUG ((LM_DEBUG,
                          ACE_TEXT ("(%t) Client::handle_output; server ")
                          ACE_TEXT ("closed handle %d\n"),
                          this->peer ().get_handle ()));
              return -1;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("(%t) %p\n"),
                               ACE_TEXT ("Client::handle_output")),
                              -1);
        }
      else if (bytes_sent == 0)
        return -1;
      else
        {
          if (bytes_sent == static_cast<ssize_t> (bytes_to_send))
            {
              delete buffer_list_.front ();
              buffer_list_.pop ();
              offset_ = 0;
            }
          else
            {
              offset_ += bytes_sent;
            }
        }
    }

  return -1;
}

int
Server::handle_close (ACE_HANDLE handle,
                      ACE_Reactor_Mask mask)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("(%t) Server::handle_close handle = %d mask = %xd\n"), handle, mask));
  return 0;
  //return SVC_HANDLER::handle_close (handle, mask);
}

// ----------------------------------------------------

typedef ACE_Acceptor<Server, ACE_SOCK_ACCEPTOR>   ACCEPTOR;
typedef ACE_Connector<Client, ACE_SOCK_CONNECTOR> CONNECTOR;

// ----------------------------------------------------

class TestAcceptor : public ACCEPTOR
{
public:

  virtual int accept_svc_handler (Server * handler)
  {
    int result = this->ACCEPTOR::accept_svc_handler (handler);

    if (result != 0)
      {
        if (errno != EWOULDBLOCK)
          ACE_ERROR ((LM_ERROR,
                      ACE_TEXT ("(%t) %p\n"),
                      ACE_TEXT ("Unable to accept connection")));

        return result;
      }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Accepted connection.  ")
                ACE_TEXT ("Stream handle: <%d>\n"),
                handler->get_handle ()));

    return result;
  }

};

// ----------------------------------------------------

class TestConnector : public CONNECTOR
{
public:

  virtual int connect_svc_handler (
    CONNECTOR::handler_type *& handler,
    const CONNECTOR::addr_type &remote_addr,
    ACE_Time_Value *timeout,
    const CONNECTOR::addr_type &local_addr,
    int reuse_addr,
    int flags,
    int perms)
  {
    const int result = this->CONNECTOR::connect_svc_handler (handler,
                                                             remote_addr,
                                                             timeout,
                                                             local_addr,
                                                             reuse_addr,
                                                             flags,
                                                             perms);

    if (result != 0)
      return result;

    ACE_TCHAR hostname[MAXHOSTNAMELEN];
    if (remote_addr.get_host_name (hostname,
                                   sizeof (hostname)) != 0)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("(%t) %p\n"),
                           ACE_TEXT ("Unable to retrieve hostname")),
                          -1);
      }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("(%t) Connected to <%s:%d>.\n"),
                hostname,
                (int) remote_addr.get_port_number ()));

    return result;
  }

  virtual int connect_svc_handler (
    CONNECTOR::handler_type *& handler,
    CONNECTOR::handler_type *& sh_copy,
    const CONNECTOR::addr_type &remote_addr,
    ACE_Time_Value *timeout,
    const CONNECTOR::addr_type &local_addr,
    int reuse_addr,
    int flags,
    int perms) {
    sh_copy = handler;
    return this->connect_svc_handler (handler, remote_addr, timeout,
                                      local_addr, reuse_addr, flags,
                                      perms);
  }
};

// ----------------------------------------------------

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

// ----------------------------------------------------

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Dev_Poll_Reactor_Echo_Test"));

  // Make sure we ignore SIGPIPE
  disable_signal (SIGPIPE, SIGPIPE);

  ACE_Dev_Poll_Reactor dp_reactor;
  dp_reactor.restart (1);          // Restart on EINTR
  ACE_Reactor reactor (&dp_reactor);

  TestConnector client;

  int flags = 0;
  ACE_SET_BITS (flags, ACE_NONBLOCK);  // Enable non-blocking in the
                                       // Svc_Handlers.

  if (client.open (&reactor, flags) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("Unable to open client service handler")),
                      -1);

  unsigned short port = 54678;

  ACE_INET_Addr addr;

  if (addr.set (port, INADDR_LOOPBACK) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("server_worker - ACE_INET_Addr::set")),
                      -1);

  TestAcceptor server;

  if (server.open (addr, &reactor, flags) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("Unable to open server service handler")),
                      -1);

  Client *client_handler = 0;

  if (client.connect (client_handler, addr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("Unable to connect to server")),
                      -1);

  if (reactor.run_reactor_event_loop () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("(%t) %p\n"),
                       ACE_TEXT ("Error when running client ")
                       ACE_TEXT ("reactor event loop")),
                      -1);

  ACE_DEBUG((LM_DEBUG, "sent: %C\n", client_handler->sent.c_str ()));
  ACE_DEBUG((LM_DEBUG, "received: %C\n", client_handler->received.c_str ()));

  ACE_TEST_ASSERT (client_handler->sent == client_handler->received);

  ACE_END_TEST;

  return 0;
}

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Dev_Poll_Reactor_Echo_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Dev Poll and Event Poll are not supported ")
              ACE_TEXT ("on this platform\n")));
  ACE_END_TEST;
  return 0;
}

#endif  /* ACE_HAS_DEV_POLL || ACE_HAS_EVENT_POLL */
