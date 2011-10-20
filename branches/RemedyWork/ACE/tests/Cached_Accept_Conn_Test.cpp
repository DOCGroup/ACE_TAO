
//=============================================================================
/**
 *  @file    Cached_Accept_Conn_Test.cpp
 *
 *  $Id$
 *
 *  The test illustrates how the <ACE_Strategy_Connector> works by
 *  showing how you can cache connections on the client using
 *  different caching strategies. Also how connections can be purged
 *  explicitly and implicitly if needed from the connection cache
 *  maintained by the connector. The <ACE_Strategy_Acceptor> can also
 *  explicitly purge connections from the process CONNECTION CACHE on
 *  demand.
 *
 *
 *  @author Kirthika Parameswaran <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef CACHED_ACCEPT_CONNECTION_TEST
#define CACHED_ACCEPT_CONNECTION_TEST

#include "test_config.h"

#include "Cached_Accept_Conn_Test.h"

#include "ace/OS_NS_string.h"
#include "ace/Get_Opt.h"

#if defined(_MSC_VER)
#pragma warning(disable:4503)
#endif /* _MSC_VER */



// Note: To keep both sunCC5.0 without debugging symbols and gcc2.7.3
// happy, it was necessary to have the definitions of the methods of
// the Accept_Strategy before the instantiations.

// HPUX doesn't accept these declaration after their usage.

// For some strange reason this must *not* be static since otherwise
// certain versions of SunC++ will not link properly.
int connection_accepted = 0;

// For some strange reason this must *not* be static since otherwise
// certain versions of SunC++ will not link properly.
int debug = 0;

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::Accept_Strategy (CACHED_CONNECT_STRATEGY &caching_connect_strategy)
  : caching_connect_strategy_ (caching_connect_strategy)
{
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                                                         bool restart)
{
  int result = ACCEPT_STRATEGY_BASE::open (local_addr,
                                           restart);

  if (result == 0)
    return result;

  // If the error occured due to the fact that the file descriptor
  // limit was exhausted, then purge the connection cache of some
  // entries.
  result = this->out_of_sockets_handler ();
  if (result == -1)
    return -1;

  // If we are able to purge, try again.
  return ACCEPT_STRATEGY_BASE::open (local_addr, restart);
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::accept_svc_handler (SVC_HANDLER *svc_handler)
{
  // Stop the event loop.
  connection_accepted = 1;

  // Try to find out if the implementation of the reactor that we are
  // using requires us to reset the event association for the newly
  // created handle. This is because the newly created handle will
  // inherit the properties of the listen handle, including its event
  // associations.
  int reset_new_handle = this->reactor_->uses_event_associations ();

  int result = this->acceptor ().accept (svc_handler->peer (), // stream
                                         0, // remote address
                                         0, // timeout
                                         1, // restart
                                         reset_new_handle  // reset new handler
                                         );
  if (result == 0)
    {
      if (debug)
        ACE_DEBUG ((LM_DEBUG,
                    ACE_TEXT ("Accept succeeded with handle %d\n"),
                    svc_handler->get_handle ()));
      return result;
    }

  // If the error occured due to the fact that the file descriptor
  // limit was exhausted, then purge the connection cache of some
  // entries.
  if (0 != this->out_of_sockets_handler ())
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("%p\n"),
                ACE_TEXT ("out_of_sockets_handler")));

  // Close down handler to avoid memory leaks.
  svc_handler->close (0);
  return -1;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::out_of_sockets_handler (void)
{
  if (ACE::out_of_handles (errno))
    {
      // Close connections which are cached by explicitly purging the
      // connection cache maintained by the connector.
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Purging connections from Connection Cache...\n")));

      return this->caching_connect_strategy_.purge_connections ();
    }

  return -1;
}

typedef Accept_Strategy<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>
        ACCEPT_STRATEGY;

Client_Svc_Handler::Client_Svc_Handler (ACE_Thread_Manager *t)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> (t)
{
}

int
Client_Svc_Handler::open (void *)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("opening Client_Svc_Handler %@ with handle %d\n"),
                this,
                this->peer ().get_handle ()));
  return 0;
}

int
Client_Svc_Handler::close (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Closing Client_Svc_Handler %@ with handle %d\n"),
              this,
              this->peer ().get_handle ()));
  return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::close (flags);
}


Server_Svc_Handler::Server_Svc_Handler (ACE_Thread_Manager *t)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> (t)
{
}

int
Server_Svc_Handler::open (void *)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("opening Server_Svc_Handler %@ with handle %d\n"),
                this,
                this->peer ().get_handle ()));

  return this->close ();
}

enum Caching_Strategy_Type
{
  ACE_LFU,
  ACE_FIFO,
  ACE_LRU,
  ACE_NULL,
  ACE_ALL
};

// Default number of clients/servers.
static int listen_once = 1;
static int user_has_specified_iterations = 0;
static size_t keep_handles_available = 100;
static double purge_percentage = 20;
static Caching_Strategy_Type caching_strategy_type = ACE_ALL;

// On Win32, the handle gobbling doesn't work.  Therefore, we need
// more iterations to get to the handle limit.
#if defined (ACE_WIN32)
static int iterations = 2000;
#else
static int iterations = 200;
#endif /* ACE_WIN32 */


static int
cached_connect (STRATEGY_CONNECTOR &con,
                const ACE_INET_Addr &server_addr)
{
  // This will make sure we get the host information correct.
  ACE_INET_Addr remote_addr (server_addr.get_port_number (),
                             ACE_LOCALHOST);

  // Perform a blocking connect to the server using the Strategy
  // Connector with a connection caching strategy.
  Client_Svc_Handler *svc_handler = 0;
  int result = con.connect (svc_handler,
                            remote_addr);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connection failed")),
                      -1);
  else
    if (debug)
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Connection successful to server at port %d!\n"),
                  remote_addr.get_port_number ()));

  // Reset Svc_Handler state.
  svc_handler->recycle_state (ACE_RECYCLABLE_PURGABLE_BUT_NOT_IDLE);

  return 0;
}

static void
server (void)
{
  int result = 1;

  while (connection_accepted == 0)
    result = ACE_Reactor::instance ()->handle_events ();

  connection_accepted = 0;

  ACE_UNUSED_ARG (result);
}

static void
test_connection_management (CACHING_STRATEGY &caching_strategy)
{
  // Configure the Strategy Connector with a strategy that caches
  // connection.
  CACHED_CONNECT_STRATEGY caching_connect_strategy (caching_strategy);

  NULL_CREATION_STRATEGY creation_strategy;
  NULL_ACTIVATION_STRATEGY activation_strategy;

  STRATEGY_CONNECTOR strategy_connector (0,
                                         &creation_strategy,
                                         &caching_connect_strategy,
                                         &activation_strategy);

  // Connect strategy is required by the <out_of_sockets_handler>.
  ACCEPT_STRATEGY listen_one_time_accept_strategy (caching_connect_strategy);

  // If <listen_once> is true, only one Acceptor is used for the test.
  ACCEPTOR listen_one_time_acceptor;
  ACE_INET_Addr server_addr;

  int result =
    listen_one_time_acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &),
                                   ACE_Reactor::instance (),
                                   0,
                                   &listen_one_time_accept_strategy);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("acceptor open")));
      return;
    }

  result = listen_one_time_acceptor.acceptor ().get_local_addr (server_addr);
  if (result != 0)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("acceptor getaddr")));
      listen_one_time_acceptor.close ();
      return;
    }

  for (int i = 1; i <= iterations; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%T iteration %d\n"),
                  i));

      // Connect strategy is required by the <out_of_sockets_handler>.
      ACCEPT_STRATEGY listen_multiple_times_accept_strategy (caching_connect_strategy);

      // If <listen_once> is false, one Acceptor is used for every
      // iteration.
      ACCEPTOR listen_multiple_times_acceptor;

      if (!listen_once)
        {
          // Bind acceptor to any port and then find out what the port
          // was.
          if (listen_multiple_times_acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &),
                                                   ACE_Reactor::instance (),
                                                   0,
                                                   &listen_multiple_times_accept_strategy) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("open")));
              break;
            }

          if (listen_multiple_times_acceptor.acceptor ().get_local_addr (server_addr) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("get_local_addr")));
              break;
            }

        }

      if (debug)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("starting server at port %d\n"),
                        server_addr.get_port_number ()));

      // Run the cached blocking test.
      if (-1 == cached_connect (strategy_connector, server_addr))
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("cached_connect")));
      else
        server ();
    }
}

void
test_caching_strategy_type (void)
{
  CACHING_STRATEGY *caching_strategy = 0;

  switch (caching_strategy_type)
    {
    case ACE_NULL:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nNull_Caching_Strategy\n\n")));
      ACE_NEW (caching_strategy,
               NULL_CACHING_STRATEGY_ADAPTER);
      break;

    case ACE_LRU:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nLRU_Caching_Strategy\n\n")));
      ACE_NEW (caching_strategy,
               LRU_CACHING_STRATEGY_ADAPTER);
      break;

    case ACE_LFU:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nLFU_Caching_Strategy\n\n")));
      ACE_NEW (caching_strategy,
               LFU_CACHING_STRATEGY_ADAPTER);
      break;

    case ACE_FIFO:
      ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("\nFIFO_Caching_Strategy\n\n")));
      ACE_NEW (caching_strategy,
               FIFO_CACHING_STRATEGY_ADAPTER);
      break;

    case ACE_ALL: // Just to remove warnings!
      break;
    }

  caching_strategy->purge_percent (purge_percentage);
  test_connection_management (*caching_strategy);
  delete caching_strategy;
}

int
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, ACE_TEXT ("l:i:p:c:a:d"));

  int cc;

  while ((cc = get_opt ()) != -1)
    switch (cc)
      {
      case 'd':
        debug = 1;
        break;
      case 'l':
        listen_once = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'i':
        iterations = ACE_OS::atoi (get_opt.opt_arg ());
        user_has_specified_iterations = 1;
        break;
      case 'p':
        purge_percentage = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case 'c':
        // Note that if null caching strategy is used then this test
        // will fail if the number of servers exceed number of open
        // files allowed for the process.
        if (ACE_OS::strcmp (get_opt.opt_arg (), ACE_TEXT ("null")) == 0)
          caching_strategy_type = ACE_NULL;
        if (ACE_OS::strcmp (get_opt.opt_arg (), ACE_TEXT ("lru")) == 0)
          caching_strategy_type = ACE_LRU;
        if (ACE_OS::strcmp (get_opt.opt_arg (), ACE_TEXT ("lfu")) == 0)
          caching_strategy_type = ACE_LFU;
        if (ACE_OS::strcmp (get_opt.opt_arg (), ACE_TEXT ("fifo")) == 0)
          caching_strategy_type = ACE_FIFO;
        break;
      case 'a':
        keep_handles_available = ACE_OS::atoi (get_opt.opt_arg ());
        break;
      case '?':
      case 'h':
      default:
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("usage: %s ")
                    ACE_TEXT ("[-t (timeout)] ")
                    ACE_TEXT ("[-c (caching strategy: lru / lfu / fifo / null [default = all])] ")
                    ACE_TEXT ("[-i (iterations)] ")
                    ACE_TEXT ("[-l (listen once)] ")
                    ACE_TEXT ("[-d (addition debugging output)] ")
                    ACE_TEXT ("[-p (purge percent)] ")
                    ACE_TEXT ("[-a (keep handles available)] "),
                    ACE_TEXT ("Cached_Accept_Conn_Test")));
        return -1;
      }

  return 0;
}

int
run_main (int argc, ACE_TCHAR *argv[])
{
  // Validate options.
  int result = parse_args (argc, argv);
  if (result != 0)
    return result;

#if defined (ACE_WIN32)
  // Somehow, on Win32, the <listen once> option allows us to create
  // more handles.
  if (!user_has_specified_iterations &&
      listen_once)
    iterations *= 2;
#endif /* ACE_WIN32 */

  // Start the test only if options are valid.
  ACE_START_TEST (ACE_TEXT ("Cached_Accept_Conn_Test"));

  // Remove the extra debugging attributes from Log_Msg output.
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  // The reactor's constructor changes the handle limit for the
  // process.
  ACE_Reactor::instance ();

  // Consume all handles in the process, leaving us
  // <keep_handles_available> to play with.
  ACE_Handle_Gobbler handle_gobbler;
  if (0 != handle_gobbler.consume_handles (keep_handles_available))
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("handle_gobbler")));

  // Do we need to test all the strategies.  Note, that the less
  // useful null strategy is ignored in this case.
  if (caching_strategy_type == ACE_ALL)
    {
      caching_strategy_type = ACE_LRU;
      test_caching_strategy_type ();

      // Default iterations are too many; if the user hasn't specified
      // otherwise, we'll shrink the iterations for LFU and FIFO.
      if (!user_has_specified_iterations)
        iterations /= 100;

      caching_strategy_type = ACE_LFU;
      test_caching_strategy_type ();

      caching_strategy_type = ACE_FIFO;
      test_caching_strategy_type ();
    }
  else
    {
      test_caching_strategy_type ();
    }

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE);

  ACE_END_TEST;
  return 0;
}


#endif /* CACHED_ACCEPT_CONNECTION_TEST */
