// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Cached_Conn_Test.cpp
//
// = DESCRIPTION
//    The test illustrates how the <ACE_Strategy_Connector> works by
//    showing how you can cache connections on the client using
//    different caching strategies. Also how connections can be purged
//    explicitly and implicitly if needed from the connection cache
//    maintained by the connector.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CACHED_CONNECT_TEST
#define CACHED_CONNECT_TEST

#include "test_config.h"

#include "Cached_Conn_Test.h"

#include "ace/OS_NS_string.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Get_Opt.h"
#include "ace/Caching_Utility_T.h"
#include "ace/Cached_Connect_Strategy_T.h"
#include "ace/Handle_Gobbler.h"

#if defined(_MSC_VER)
#pragma warning(disable:4503)
#endif /* _MSC_VER */



typedef size_t ATTRIBUTES;
typedef ACE_Pair<Svc_Handler *, ATTRIBUTES>
        CACHED_HANDLER;
typedef ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
        ACE_ADDR;
typedef ACE_Hash<ACE_ADDR> H_KEY;
typedef ACE_Equal_To<ACE_ADDR> C_KEYS;

typedef ACE_Hash_Map_Manager_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP;
typedef ACE_Hash_Map_Iterator_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP_ITERATOR;
typedef ACE_Hash_Map_Reverse_Iterator_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP_REVERSE_ITERATOR;

typedef ACE_Recyclable_Handler_Cleanup_Strategy<ACE_ADDR, CACHED_HANDLER, HASH_MAP>
        CLEANUP_STRATEGY;
typedef ACE_Recyclable_Handler_Caching_Utility<ACE_ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTRIBUTES>
        CACHING_UTILITY;

typedef ACE_LRU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
        LRU_CACHING_STRATEGY;

typedef ACE_LFU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
        LFU_CACHING_STRATEGY;
typedef ACE_FIFO_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
        FIFO_CACHING_STRATEGY;
typedef ACE_Null_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
        NULL_CACHING_STRATEGY;
typedef ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, LRU_CACHING_STRATEGY>
        LRU_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, LFU_CACHING_STRATEGY>
        LFU_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, FIFO_CACHING_STRATEGY>
        FIFO_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, NULL_CACHING_STRATEGY>
        NULL_CACHING_STRATEGY_ADAPTER;
typedef ACE_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
        CACHING_STRATEGY;

typedef ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>
        ACCEPTOR;

typedef ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
        STRATEGY_CONNECTOR;

typedef ACE_NOOP_Creation_Strategy<Svc_Handler>
        NULL_CREATION_STRATEGY;

typedef ACE_NOOP_Concurrency_Strategy<Svc_Handler>
        NULL_ACTIVATION_STRATEGY;

typedef ACE_Cached_Connect_Strategy_Ex<Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES, ACE_SYNCH_NULL_MUTEX>
        CACHED_CONNECT_STRATEGY;

#endif /* CACHED_CONNECT_TEST */

static int debug = 0;

Svc_Handler::Svc_Handler (ACE_Thread_Manager *t)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> (t)
{
}

int
Svc_Handler::open (void *)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("opening Svc_Handler %@ with handle %d\n"),
                this,
                this->peer ().get_handle ()));

  return 0;
}

int
Svc_Handler::close (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("Closing Svc_Handler %@ with handle %d\n"),
              this,
              this->peer ().get_handle ()));
  return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::close (flags);
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
static CACHED_CONNECT_STRATEGY *connect_strategy = 0;

// On Win32, the handle gobbling doesn't work.  Therefore, we need
// more iterations to get to the handle limit.
#if defined (ACE_WIN32)
static int iterations = 2000;
#else
static int iterations = 200;
#endif /* ACE_WIN32 */

//====================================================================

static void
out_of_sockets_handler (void)
{
  if (ACE::out_of_handles (errno))
    {
      // Close connections which are cached by explicitly purging the
      // connection cache maintained by the connector.
      ACE_DEBUG
        ((LM_DEBUG,
          ACE_TEXT ("Purging connections from Connection Cache...\n")));

      if (-1 == connect_strategy->purge_connections ())
        ACE_ERROR ((LM_ERROR,
                    ACE_TEXT ("%p\n"),
                    ACE_TEXT ("purge_connections")));
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("%p\n"),
                  ACE_TEXT ("in out_of_sockets_handler, ")
                  ACE_TEXT ("but out_of_handles said no")));
    }
}

static int
cached_connect (STRATEGY_CONNECTOR &con,
                const ACE_INET_Addr &server_addr)
{
  // This will make sure we get the host information correct.
  ACE_INET_Addr remote_addr (server_addr.get_port_number (),
                             ACE_LOCALHOST);

  // Perform a blocking connect to the server using the Strategy
  // Connector with a connection caching strategy.
  Svc_Handler *svc_handler = 0;
  int result = con.connect (svc_handler,
                            remote_addr);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("connection failed")),
                      -1);

  // Reset Svc_Handler state.
  svc_handler->recycle_state (ACE_RECYCLABLE_PURGABLE_BUT_NOT_IDLE);

  return 0;
}

static int
server (ACCEPTOR *acceptor)
{
  ACE_INET_Addr cli_addr;

  // Create a new <Svc_Handler> to consume the data.
  Svc_Handler svc_handler;

  int result = acceptor->accept (&svc_handler,
                                 &cli_addr);
  if (result == -1)
    return -1;

  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ACE_TEXT ("client %s connected from %d\n"),
                cli_addr.get_host_name (),
                cli_addr.get_port_number ()));

  //
  // Svc_Handler dies here, closing the server side socket.
  //
  return 0;
}

static void
test_connection_management (CACHING_STRATEGY &caching_strategy)
{
  // Configure the Strategy Connector with a strategy that caches
  // connection.
  CACHED_CONNECT_STRATEGY caching_connect_strategy (caching_strategy);

  // This is required by the <out_of_sockets_handler>.
  connect_strategy = &caching_connect_strategy;

  NULL_CREATION_STRATEGY creation_strategy;
  NULL_ACTIVATION_STRATEGY activation_strategy;

  STRATEGY_CONNECTOR strategy_connector (0,
                                         &creation_strategy,
                                         &caching_connect_strategy,
                                         &activation_strategy);

  // If <listen_once> is true, only one Acceptor is used for the test.
  ACCEPTOR listen_one_time_acceptor;
  ACE_INET_Addr server_addr;

  if (0 != listen_one_time_acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &)))
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("acceptor open")));
      return;
    }

  if (0 != listen_one_time_acceptor.acceptor ().get_local_addr (server_addr))
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

      // If <listen_once> is false, one Acceptor is used for every
      // iteration.
      ACCEPTOR listen_multiple_times_acceptor;

      ACCEPTOR &acceptor = listen_once ?
        listen_one_time_acceptor :
        listen_multiple_times_acceptor;

      if (!listen_once)
        {
          // Bind acceptor to any port and then find out what the port
          // was.
          if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &)) == -1)
            {
              out_of_sockets_handler ();
              continue;
            }

          if (acceptor.acceptor ().get_local_addr (server_addr) == -1)
            {
              ACE_ERROR ((LM_ERROR,
                          ACE_TEXT ("%p\n"),
                          ACE_TEXT ("get_local_addr")));
              break;
            }

          if (debug)
            ACE_DEBUG ((LM_DEBUG,
                        ACE_TEXT ("starting server at port %d\n"),
                        server_addr.get_port_number ()));
        }

      // Run the cached blocking test.
      if (-1 == cached_connect (strategy_connector, server_addr))
        ACE_ERROR ((LM_ERROR, ACE_TEXT ("%p\n"), ACE_TEXT ("cached_connect")));

      if (-1 == server (&acceptor))
        out_of_sockets_handler ();
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
                    ACE_TEXT ("[-c (caching strategy: lru / lfu / fifo / null [default = all])] ")
                    ACE_TEXT ("[-i (iterations)] ")
                    ACE_TEXT ("[-l (listen once)] ")
                    ACE_TEXT ("[-d (addition debugging output)] ")
                    ACE_TEXT ("[-p (purge percent)] ")
                    ACE_TEXT ("[-a (keep handles available)] "),
                    ACE_TEXT ("Cached_Conn_Test")));
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
  if (!user_has_specified_iterations && listen_once)
    iterations *= 2;
#endif /* ACE_WIN32 */

  // Start the test only if options are valid.
  ACE_START_TEST (ACE_TEXT ("Cached_Conn_Test"));

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
