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

// IBM C Set++ just can't grok the templates in here for auto template
// instantiation. It ends up overwriting a tempinc/*.C file and mashes
// its contents.
#if !defined (__xlC__) || (__xlC__ > 0x0301)

#include "Cached_Conn_Test.h"

#if defined(__GNUC__) && __GNUC__ == 2 && __GNUC_MINOR__ < 8
#define ACE_HAS_BROKEN_EXTENDED_TEMPLATES
#endif /* __GNUC__ */

#include "ace/INET_Addr.h"
#include "ace/Strategies.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Get_Opt.h"
#include "ace/Caching_Utility_T.h"
#include "ace/Cached_Connect_Strategy_T.h"

#if defined(_MSC_VER)
#pragma warning(disable:4503)
#endif /* _MSC_VER */

ACE_RCSID(tests, Cached_Conn_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

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
                ASYS_TEXT ("opening Svc_Handler %d with handle %d\n"),
                this,
                this->peer ().get_handle ()));

  return 0;
}

int
Svc_Handler::close (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("Closing Svc_Handler %d with handle %d\n"),
              this,
              this->peer ().get_handle ()));
  return ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>::close (flags);
}

typedef size_t ATTRIBUTES;
typedef ACE_Pair<Svc_Handler *, ATTRIBUTES>
        CACHED_HANDLER;
typedef ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
        ADDR;
typedef ACE_Hash<ADDR> H_KEY;
typedef ACE_Equal_To<ADDR> C_KEYS;

typedef ACE_Hash_Map_Manager_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP;
typedef ACE_Hash_Map_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP_ITERATOR;
typedef ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
        HASH_MAP_REVERSE_ITERATOR;

typedef ACE_Recyclable_Handler_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>
        CLEANUP_STRATEGY;
typedef ACE_Recyclable_Handler_Caching_Utility<ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTRIBUTES>
        CACHING_UTILITY;

typedef ACE_LRU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
        LRU_CACHING_STRATEGY;

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

typedef LRU_CACHING_STRATEGY
        CACHING_STRATEGY;

#else

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

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

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
static int iterations = 2000;
static int user_has_specified_iterations = 0;
static double purge_percentage = 20;
static Caching_Strategy_Type caching_strategy_type = ACE_ALL;
static CACHED_CONNECT_STRATEGY *connect_strategy = 0;

//====================================================================

static void
out_of_sockets_handler (void)
{
  // ENOBUFS had to be checked on NT while ENOENT check had to be added for
  // Solaris + Linux.
  if (ACE::out_of_file_descriptors (errno))
    {
      // Close connections which are cached by explicitly purging the
      // connection cache maintained by the connector.
      ACE_DEBUG ((LM_DEBUG, "Purging connections from Connection Cache...\n"));

      int retval = connect_strategy->purge_connections ();
      ACE_ASSERT (retval != -1);
    }
  else
    {
      ACE_ERROR ((LM_ERROR,
                  ASYS_TEXT ("%p\n"),
                  ASYS_TEXT ("out_of_sockets_handler  failed!")));
      // This shouldn't happen!
      ACE_ASSERT (0);
    }
}

static int
cached_connect (STRATEGY_CONNECTOR &con,
                const ACE_INET_Addr &server_addr)
{
  // This will make sure we get the host information correct.
  ACE_INET_Addr remote_addr (server_addr.get_port_number (),
                             ACE_DEFAULT_SERVER_HOST);

  // Perform a blocking connect to the server using the Strategy
  // Connector with a connection caching strategy.
  Svc_Handler *svc_handler = 0;
  int result = con.connect (svc_handler,
                            remote_addr);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("connection failed")),
                      -1);

  // Reset Svc_Handler state.
  svc_handler->state (ACE_RECYCLABLE_PURGABLE_BUT_NOT_IDLE);

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
                ASYS_TEXT ("client %s connected from %d\n"),
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

  int result = listen_one_time_acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &));
  ACE_ASSERT (result == 0);

  result = listen_one_time_acceptor.acceptor ().get_local_addr (server_addr);
  ACE_ASSERT (result == 0);

  for (int i = 1; i <= iterations; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("iteration %d\n"),
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
                          ASYS_TEXT ("%p\n"),
                          ASYS_TEXT ("get_local_addr")));
              ACE_ASSERT (0);
            }

          if (debug)
            ACE_DEBUG ((LM_DEBUG,
                        ASYS_TEXT ("starting server at port %d\n"),
                        server_addr.get_port_number ()));
        }

      // Run the cached blocking test.
      int result = cached_connect (strategy_connector,
                                   server_addr);
      ACE_ASSERT (result != -1);

      result = server (&acceptor);
      if (result == -1)
        out_of_sockets_handler ();
    }
}

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

void
test_caching_strategy_type (void)
{
  ACE_DEBUG ((LM_DEBUG, "\nLRU_Caching_Strategy\n\n"));
  CACHING_STRATEGY caching_strategy;
  caching_strategy.purge_percent (purge_percentage);
  test_connection_management (caching_strategy);
}

#else

void
test_caching_strategy_type (void)
{
  CACHING_STRATEGY *caching_strategy = 0;

  switch (caching_strategy_type)
    {
    case ACE_NULL:
      ACE_DEBUG ((LM_DEBUG, "\nNull_Caching_Strategy\n\n"));
      ACE_NEW (caching_strategy,
               NULL_CACHING_STRATEGY_ADAPTER);
      break;

    case ACE_LRU:
      ACE_DEBUG ((LM_DEBUG, "\nLRU_Caching_Strategy\n\n"));
      ACE_NEW (caching_strategy,
               LRU_CACHING_STRATEGY_ADAPTER);
      break;

    case ACE_LFU:
      ACE_DEBUG ((LM_DEBUG, "\nLFU_Caching_Strategy\n\n"));
      ACE_NEW (caching_strategy,
               LFU_CACHING_STRATEGY_ADAPTER);
      break;

    case ACE_FIFO:
      ACE_DEBUG ((LM_DEBUG, "\nFIFO_Caching_Strategy\n\n"));
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

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "l:i:p:c:d");

  int cc;

  while ((cc = get_opt ()) != -1)
    switch (cc)
      {
      case 'd':
        debug = 1;
        break;
      case 'l':
        listen_once = atoi (get_opt.optarg);
        break;
      case 'i':
        iterations = atoi (get_opt.optarg);
        user_has_specified_iterations = 1;
        break;
      case 'p':
        purge_percentage = atoi (get_opt.optarg);
        break;
      case 'c':
        // Note that if null caching strategy is used then this test
        // will fail if the number of servers exceed number of open
        // files allowed for the process.
        if (ACE_OS::strcmp (get_opt.optarg, "null") == 0)
          caching_strategy_type = ACE_NULL;
        if (ACE_OS::strcmp (get_opt.optarg, "lru") == 0)
          caching_strategy_type = ACE_LRU;
        if (ACE_OS::strcmp (get_opt.optarg, "lfu") == 0)
          caching_strategy_type = ACE_LFU;
        if (ACE_OS::strcmp (get_opt.optarg, "fifo") == 0)
          caching_strategy_type = ACE_FIFO;
        break;
      case '?':
      case 'h':
      default:
        ACE_ERROR ((LM_ERROR,
                    ASYS_TEXT ("usage: %s ")
                    ASYS_TEXT ("[-c (caching strategy: lru / lfu / fifo / null [default = all])] ")
                    ASYS_TEXT ("[-i (iterations)] ")
                    ASYS_TEXT ("[-l (listen once)] ")
                    ASYS_TEXT ("[-d (addition debugging output)] ")
                    ASYS_TEXT ("[-p (purge percent)] "),
                    argv[0]));
        return -1;
      }

  return 0;
}

int
main (int argc,
      ASYS_TCHAR *argv[])
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
  ACE_START_TEST (ASYS_TEXT ("Cached_Conn_Test"));

  // Remove the extra debugging attributes from Log_Msg output.
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

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

#else   /* Do this for C Set++ 3.1 */

int
main (int argc,
      ASYS_TCHAR *argv[])
{
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);

  ACE_START_TEST (ASYS_TEXT ("Cached_Conn_Test"));
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("C Set++ won't build this test correctly\n")));

#endif /* !__xlC__ || __xlC > 0x0301 */

  ACE_END_TEST;
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>;
template class ACE_NOOP_Creation_Strategy<Svc_Handler>;
template class ACE_Concurrency_Strategy<Svc_Handler>;
template class ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Creation_Strategy<Svc_Handler>;
template class ACE_Hash_Map_Entry<ADDR, Svc_Handler *>;
template class ACE_Hash<ADDR>;
template class ACE_Equal_To<ADDR>;
template class ACE_Hash_Map_Manager<ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator<ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_NOOP_Concurrency_Strategy<Svc_Handler>;
template class ACE_Recycling_Strategy<Svc_Handler>;
template class ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<Svc_Handler>;
template class ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>;

template class ACE_Pair<Svc_Handler *, ATTRIBUTES>;
template class ACE_Reference_Pair<ADDR, Svc_Handler *>;
template class ACE_Hash_Map_Entry<ADDR, CACHED_HANDLER>;

template class ACE_Hash_Map_Manager<ADDR, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ADDR, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ADDR, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ADDR, Svc_Handler *, H_KEY,  C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager<ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Bucket_Iterator<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Bucket_Iterator<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;

// = Caching_Strategy
template class ACE_Hash_Cache_Map_Manager<ADDR, Svc_Handler *, H_KEY, C_KEYS, CACHING_STRATEGY, ATTRIBUTES>;

template class ACE_LRU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

template class ACE_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;
template class ACE_LFU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;
template class ACE_FIFO_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;
template class ACE_Null_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;

template class ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, LRU_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, LFU_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, FIFO_CACHING_STRATEGY>;
template class ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, NULL_CACHING_STRATEGY>;

template class ACE_Cache_Map_Manager<ADDR, Svc_Handler *, HASH_MAP, HASH_MAP_ITERATOR, HASH_MAP_REVERSE_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>;
template class ACE_Cache_Map_Iterator<ADDR, Svc_Handler *, HASH_MAP_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>;
template class ACE_Cache_Map_Reverse_Iterator<ADDR, Svc_Handler *, HASH_MAP_REVERSE_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>;

#else

template class ACE_Cache_Map_Manager<ADDR, Svc_Handler *, HASH_MAP, CACHING_STRATEGY, ATTRIBUTES>;

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

template class ACE_Cached_Connect_Strategy_Ex<Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>;

template class ACE_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>;
template class ACE_Recyclable_Handler_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>;
template class ACE_Recyclable_Handler_Caching_Utility<ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTRIBUTES>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#pragma instantiate ACE_NOOP_Creation_Strategy<Svc_Handler>
#pragma instantiate ACE_Concurrency_Strategy<Svc_Handler>
#pragma instantiate ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Creation_Strategy<Svc_Handler>
#pragma instantiate ACE_Hash_Map_Entry<ADDR, Svc_Handler *>
#pragma instantiate ACE_Hash<ADDR>
#pragma instantiate ACE_Equal_To<ADDR>
#pragma instantiate ACE_Hash_Map_Manager<ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_NOOP_Concurrency_Strategy<Svc_Handler>
#pragma instantiate ACE_Recycling_Strategy<Svc_Handler>
#pragma instantiate ACE_Strategy_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<Svc_Handler>
#pragma instantiate ACE_Oneshot_Acceptor<Svc_Handler, ACE_SOCK_ACCEPTOR>

#pragma instantiate ACE_Pair<Svc_Handler *, ATTRIBUTES>
#pragma instantiate ACE_Reference_Pair<ADDR, Svc_Handler *>
#pragma instantiate ACE_Hash_Map_Entry<ADDR, CACHED_HANDLER>

#pragma instantiate ACE_Hash_Map_Manager<ADDR, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ADDR, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ADDR, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ADDR, Svc_Handler *, H_KEY,  C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Manager<ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>

// = Caching_Strategy
#pragma instantiate ACE_Hash_Cache_Map_Manager<ADDR, Svc_Handler *, H_KEY, C_KEYS, CACHING_STRATEGY, ATTRIBUTES>

#pragma instantiate ACE_LRU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>

#if !defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)

#pragma instantiate ACE_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
#pragma instantiate ACE_LFU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
#pragma instantiate ACE_FIFO_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>
#pragma instantiate ACE_Null_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>

#pragma instantiate ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, LRU_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, LFU_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, FIFO_CACHING_STRATEGY>
#pragma instantiate ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, NULL_CACHING_STRATEGY>

#pragma instantiate ACE_Cache_Map_Manager<ADDR, Svc_Handler *, HASH_MAP, HASH_MAP_ITERATOR, HASH_MAP_REVERSE_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Iterator<ADDR, Svc_Handler *, HASH_MAP_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<ADDR, Svc_Handler *, HASH_MAP_REVERSE_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>

#else

#pragma instantiate ACE_Cache_Map_Manager<ADDR, Svc_Handler *, HASH_MAP, CACHING_STRATEGY, ATTRIBUTES>

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

#pragma instantiate ACE_Cached_Connect_Strategy_Ex<Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>

#pragma instantiate ACE_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>
#pragma instantiate ACE_Recyclable_Handler_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>
#pragma instantiate ACE_Recyclable_Handler_Caching_Utility<ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTRIBUTES>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* CACHED_CONNECT_TEST */
