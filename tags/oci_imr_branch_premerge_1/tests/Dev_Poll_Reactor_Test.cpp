//=============================================================================
/**
 *  @file    Dev_Poll_Reactor_Test.cpp
 *
 *  $Id$
 *
 *  This test verifies that the Dev_Poll_Reactor is functioning
 *  properly, and demonstrates how "speculative reads" can be
 *  performed.  "Speculative reads" are necessary when using event
 *  demultiplexing mechanisms that use a "state change" interface.
 *  Similarly, "speculative writes" may be necessary, i.e. keep
 *  writing until the connection is flow controlled.  An example of a
 *  demuxing mechanism with such an interface is Linux's `/dev/epoll'
 *  character device.  Mechansims with state change interfaces are
 *  also said to be "edge triggered," versus "level triggered"
 *  mechanisms such as select().
 *
 *  @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#include "test_config.h"

ACE_RCSID (tests,
           Dev_Poll_Reactor_Test,
           "$Id$")


#if defined (ACE_HAS_DEV_POLL) || defined (ACE_HAS_EVENT_POLL)

#include "ace/Reactor.h"
#include "ace/Dev_Poll_Reactor.h"

#include "ace/Acceptor.h"
#include "ace/Connector.h"

#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Stream.h"


typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> SVC_HANDLER;

// ----------------------------------------------------

class Client : public SVC_HANDLER
{
public:

  Client (void);

  virtual int open (void * = 0);

  virtual int handle_output (ACE_HANDLE handle);

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask mask);

private:

  unsigned int call_count_;

};


class Server : public SVC_HANDLER
{
public:

  Server (void);

  virtual int handle_input (ACE_HANDLE handle);

  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act);

  virtual int handle_close (ACE_HANDLE handle,
                            ACE_Reactor_Mask mask);

private:

  unsigned int call_count_;

};

// ----------------------------------------------------

Client::Client (void)
  : call_count_ (0)
{
}

int
Client::open (void *)
{
  //  ACE_ASSERT (this->reactor () != 0);

  if (this->reactor ()
      && this->reactor ()->register_handler (
           this,
           ACE_Event_Handler::WRITE_MASK) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("unable to register client handler")),
                      -1);

  return 0;
}

int
Client::handle_output (ACE_HANDLE)
{
  for (int i = 1; i <= 5; ++i)
    {
      char buffer[BUFSIZ] = { 0 };

      ACE_OS::sprintf (buffer, "test message %d.\n", i);

      ssize_t bytes_sent =
        this->peer ().send (buffer, ACE_OS::strlen (buffer));

      if (bytes_sent == -1)
        {
          if (errno == EWOULDBLOCK)
            return 0;  // Flow control kicked in.
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("Client::handle_output")),
                              -1);
        }
      else if (bytes_sent == 0)
        return -1;
      else
        ACE_DEBUG ((LM_INFO,
                    "Sent %s",
                    buffer));
    }

  return 0;
}

int
Client::handle_timeout (const ACE_Time_Value &, const void *)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) Expected client timeout occured at: %T\n")));

  int status = this->handle_output (this->get_handle ());
  if (status != 0)
    return status;

  this->call_count_++;

  if (this->call_count_ > 10)
    {
      if (this->reactor ()->end_reactor_event_loop () == 0)
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("(%P|%t) Successful client ")
                    ACE_TEXT ("reactor shutdown.\n")));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Failed client reactor shutdown.\n")));

      // Force this service handler to be closed in either case.
      return -1;
    }

  return 0;
}

int
Client::handle_close (ACE_HANDLE handle,
                      ACE_Reactor_Mask mask)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) Client Svc_Handler closed ")
              ACE_TEXT ("handle <%d> with reactor mask <%d>.\n"),
              handle,
              mask));

  return 0;
}

// ----------------------------------------------------

Server::Server (void)
  : call_count_ (0)
{
}

int
Server::handle_input (ACE_HANDLE /* handle */)
{
  char buffer[BUFSIZ] = { 0 };

  ssize_t bytes_read = 0;

  for (char * buf = buffer; buf < buffer + BUFSIZ; buf += bytes_read)
    {
      // Keep reading until it is no longer possible to do so.
      //
      // This is done since the underlying event demultiplexing
      // mechanism may be "state change" interface (as opposed to
      // "state monitoring"), in which case a "speculative" read is
      // done.
      ssize_t bytes_read =
        this->peer ().recv (buf, BUFSIZ);

      ACE_DEBUG ((LM_DEBUG,
                  "****** bytes_read = %d\n",
                  bytes_read));

      if (bytes_read == -1)
        {
          if (errno == EWOULDBLOCK)
            {

//               ACE_HEX_DUMP ((LM_DEBUG,
//                              buf,
//                              80,
//                              "BUFFER CONTENTS"));
              if (buf == buffer)
                return 0;
              else
                break;
            }
          else
            ACE_ERROR_RETURN ((LM_ERROR,
                               ACE_TEXT ("%p\n"),
                               ACE_TEXT ("Server::handle_input")),
                              -1);
        }
      else if (bytes_read == 0)
        return -1;
    }

  ACE_DEBUG ((LM_INFO,
              "(%P|%t) Message received: %s\n",
              buffer));

  return 0;
}

int
Server::handle_timeout (const ACE_Time_Value &,
                        const void *)
{
  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) Expected server timeout occured at: %T\n")));

//   if (this->call_count_ == 0
//       && this->handle_input (this->get_handle ()) != 0
//       && errno != EWOULDBLOCK)
//     return -1;

//   ACE_DEBUG ((LM_INFO,
//               "SERVER HANDLE = %d\n",
//               this->get_handle ()));


  this->call_count_++;

  if (this->call_count_ > 10)
    {
      if (this->reactor ()->end_reactor_event_loop () == 0)
        ACE_DEBUG ((LM_INFO,
                    ACE_TEXT ("(%P|%t) Successful server ")
                    ACE_TEXT ("reactor shutdown.\n")));
      else
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("(%P|%t) Failed server reactor shutdown.\n")));

      // Force this service handler to be closed in either case.
      return -1;
    }

  return 0;
}

int
Server::handle_close (ACE_HANDLE handle,
                      ACE_Reactor_Mask mask)
{
  if (this->call_count_ > 4)
    {
      ACE_DEBUG ((LM_INFO,
                  ACE_TEXT ("(%P|%t) Server Svc_Handler closed ")
                  ACE_TEXT ("handle <%d,%d> with reactor mask <%d>.\n"),
                  handle,
                  this->get_handle (),
                  mask));

      return this->peer ().close ();
    }

  return 0;
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
                      ACE_TEXT ("(%P|%t) %p\n"),
                      ACE_TEXT ("Unable to accept connection")));

        return result;
      }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Accepted connection.  ")
                ACE_TEXT ("Stream handle: <%d>\n"),
                handler->get_handle ()));

//     if (handler->handle_input (handler->get_handle ()) == -1
//         && errno != EWOULDBLOCK)
//       return -1;

// #if 0
    ACE_Time_Value delay (2, 0);
    ACE_Time_Value restart (2, 0);
    if (handler->reactor ()->schedule_timer (handler,
                                             0,
                                             delay,
                                             restart) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Unable to schedule server side ")
                           ACE_TEXT ("timer in ACE_Dev_Poll_Reactor")),
                          -1);
      }
// #endif  /* 0 */

    return result;
  }

};

// ----------------------------------------------------

class TestConnector : public CONNECTOR
{
public:

  virtual int connect_svc_handler (
    Client *& handler,
    const CONNECTOR::ACE_TYPENAME_ACE_PEER_CONNECTOR_PEER_ADDR &remote_addr,
    ACE_Time_Value *timeout,
    const CONNECTOR::ACE_TYPENAME_ACE_PEER_CONNECTOR_PEER_ADDR &local_addr,
    int reuse_addr,
    int flags,
    int perms)
  {
    int result = this->CONNECTOR::connect_svc_handler (handler,
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
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Unable to retrieve hostname")),
                          -1);
      }

    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("Connected to <%s:%d>.\n"),
                hostname,
                (int) remote_addr.get_port_number ()));

// #if 0
    ACE_Time_Value delay (4, 0);
    ACE_Time_Value restart (3, 0);
    if (handler->reactor ()->schedule_timer (handler,
                                             0,
                                             delay,
                                             restart) == -1)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           ACE_TEXT ("%p\n"),
                           ACE_TEXT ("Unable to schedule client side ")
                           ACE_TEXT ("timer in ACE_Dev_Poll_Reactor")),
                          -1);
      }
// #endif  /* 0 */

    return result;
  }

};

// ----------------------------------------------------

extern "C" void *
server_worker (void *p)
{
  unsigned short port = *(ACE_static_cast (unsigned short *, p));

  ACE_INET_Addr addr;

  if (addr.set (port, INADDR_LOOPBACK) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("server_worker - ACE_INET_Addr::set")));

      return (void *) -1;
    }

  ACE_Dev_Poll_Reactor dp_reactor;
  ACE_Reactor reactor (&dp_reactor);

  TestAcceptor server;

  int flags = 0;
  ACE_SET_BITS (flags, ACE_NONBLOCK);  // Enable non-blocking in the
                                       // Svc_Handlers.

  if (server.open (addr, &reactor, flags) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("Unable to open server service handler")));

      return (void *) -1;
    }

  if (reactor.run_reactor_event_loop () != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("Error when running server ")
                  ACE_TEXT ("reactor event loop\n")));

      return (void *) -1;
    }

  ACE_DEBUG ((LM_INFO,
              ACE_TEXT ("(%P|%t) Reactor event loop finished ")
              ACE_TEXT ("successfully.\n")));

  return 0;
}

// ----------------------------------------------------

// struct server_arg
// {
//   unsigned short port;

//   ACE_Condition<ACE_SYNCH_MUTEX> * cv;
// };

// ----------------------------------------------------

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Dev_Poll_Reactor_Test"));

  ACE_Dev_Poll_Reactor dp_reactor;
  ACE_Reactor reactor (&dp_reactor);

  TestConnector client;

  int flags = 0;
  ACE_SET_BITS (flags, ACE_NONBLOCK);  // Enable non-blocking in the
                                       // Svc_Handlers.

  if (client.open (&reactor, flags) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Unable to open client service handler")),
                      -1);

//   ACE_SYNCH_MUTEX mutex;
//   ACE_Condition<ACE_SYNCH_MUTEX> cv (mutex);

//   server_arg arg;
//   arg.port = 54678;  // Port the server will listen on.
//   arg.cv = &cv;

  unsigned short port = 54678;

  if (ACE_Thread_Manager::instance ()->spawn (server_worker, &port) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Unable to spawn server thread")),
                      -1);

  ACE_OS::sleep (5);  // Wait for the listening endpoint to be set up.

  ACE_INET_Addr addr;
  if (addr.set (port, INADDR_LOOPBACK) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("ACE_INET_Addr::set")),
                      -1);

  Client *client_handler = 0;

  if (client.connect (client_handler, addr) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Unable to connect to server")),
                      -1);

  if (reactor.run_reactor_event_loop () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("Error when running client ")
                       ACE_TEXT ("reactor event loop\n")),
                      -1);

  if (ACE_Thread_Manager::instance ()->wait () != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Error waiting for threads to complete")),
                      -1);

  ACE_END_TEST;

  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Acceptor<Server, ACE_SOCK_ACCEPTOR>;
template class ACE_Connector_Base<Client>;
template class ACE_Connector<Client, ACE_SOCK_CONNECTOR>;
template class ACE_NonBlocking_Connect_Handler<Client>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Acceptor<Server, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Connector_Base<Client>
#pragma instantiate ACE_Connector<Client, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_NonBlocking_Connect_Handler<Client>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#else

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Dev_Poll_Reactor_Test"));
  ACE_ERROR ((LM_INFO,
              ACE_TEXT ("Dev Poll and Event Poll are not supported ")
              ACE_TEXT ("on this platform\n")));
  ACE_END_TEST;
  return 0;
}

#endif  /* ACE_HAS_DEV_POLL || ACE_HAS_EVENT_POLL */
