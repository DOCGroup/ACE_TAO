// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    Cached_Accept_Conn_Test.cpp
//
// = DESCRIPTION
//    The test illustrates how the <ACE_Strategy_Connector> works by
//    showing how you can cache connections on the client using
//    different caching strategies. Also how connections can be purged
//    explicitly and implicitly if needed from the connection cache
//    maintained by the connector. The <ACE_Strategy_Acceptor> can also
//    explicitly purge connections from the process CONNECTION CACHE on
//    demand.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CACHED_ACCEPT_CONNECTION_TEST
#define CACHED_ACCEPT_CONNECTION_TEST

#include "test_config.h"

// IBM C Set++ just can't grok the templates in here for auto template
// instantiation. It ends up overwriting a tempinc/*.C file and mashes
// its contents.
#if !defined (__xlC__) || (__xlC__ > 0x0301)

#include "Cached_Accept_Conn_Test.h"

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

ACE_RCSID(tests, Cached_Accept_Conn_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

static int debug = 0;

Client_Svc_Handler::Client_Svc_Handler (ACE_Thread_Manager *t)
  : ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> (t)
{
}

int
Client_Svc_Handler::open (void *)
{
  if (debug)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("opening Client_Svc_Handler %d with handle %d\n"),
                this,
                this->peer ().get_handle ()));
  return 0;
}

int
Client_Svc_Handler::close (u_long flags)
{
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("Closing Client_Svc_Handler %d with handle %d\n"),
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
                ASYS_TEXT ("opening Server_Svc_Handler %d with handle %d\n"),
                this,
                this->peer ().get_handle ()));

  return this->close ();
}

typedef size_t ATTRIBUTES;
typedef ACE_Pair<Client_Svc_Handler *, ATTRIBUTES>
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

typedef ACE_Strategy_Acceptor<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>
        ACCEPTOR;

typedef ACE_Strategy_Connector<Client_Svc_Handler, ACE_SOCK_CONNECTOR>
        STRATEGY_CONNECTOR;

typedef ACE_NOOP_Creation_Strategy<Client_Svc_Handler>
        NULL_CREATION_STRATEGY;

typedef ACE_NOOP_Concurrency_Strategy<Client_Svc_Handler>
        NULL_ACTIVATION_STRATEGY;

typedef ACE_Cached_Connect_Strategy_Ex<Client_Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES, ACE_SYNCH_NULL_MUTEX>
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
static int default_iterations = 3000;
static int iterations = default_iterations;
static double purge_percentage = 20;
static Caching_Strategy_Type caching_strategy_type = ACE_ALL;

//====================================================================

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class Accept_Strategy : public ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
public:

  Accept_Strategy (CACHED_CONNECT_STRATEGY &caching_connect_strategy);
  // Constructor.

  int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
            int restart = 0);
  // Initialize the <peer_acceptor_> with <local_addr>.  If the
  // process runs out of descriptors, the unsed svc_handlers from the
  // CONNECTION CACHE are removed.

  int accept_svc_handler (SVC_HANDLER *svc_handler);
  // The default behavior delegates to the <accept> method of the
  // PEER_ACCEPTOR. A check is made here for the process running out
  // of file descriptors. If so, the CONNECTION CACHE is purged of
  // some idle svc_handlers.

protected:

  typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY_BASE;

  int out_of_sockets_handler (void);
  // Handler for removing cached connections.

  CACHED_CONNECT_STRATEGY &caching_connect_strategy_;
};

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::Accept_Strategy (CACHED_CONNECT_STRATEGY &caching_connect_strategy)
  : caching_connect_strategy_ (caching_connect_strategy)
{
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
                                                                    int restart)
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
  int result = ACE_Reactor::instance ()->end_event_loop ();
  ACE_ASSERT (result != 1);

  // Note: The base class method isnt called since it closes the
  // svc_handler on error which we want to avoid as we are trying to
  // accept after purging.

  // Try to find out if the implementation of the reactor that we are
  // using requires us to reset the event association for the newly
  // created handle. This is because the newly created handle will
  // inherit the properties of the listen handle, including its event
  // associations.
  int reset_new_handle = this->reactor_->uses_event_associations ();

  result = this->acceptor_.accept (svc_handler->peer (), // stream
                                   0, // remote address
                                   0, // timeout
                                   1, // restart
                                   reset_new_handle  // reset new handler
                                   );
  if (result == 0)
    return result;

  // If the error occured due to teh fact that the file descriptor
  // limit was exhausted, then purge the connection cache of some
  // entries.
  result = this->out_of_sockets_handler ();
  ACE_ASSERT (result == 0);

  // Close down handler to avoid memory leaks.
  svc_handler->close (0);
  return -1;
}

template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1> int
Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>::out_of_sockets_handler (void)
{
  // ENOBUFS had to be checked on NT while ENOENT check had to be
  // added for Solaris + Linux.
  if (errno == EMFILE || errno == ENOBUFS || errno == ENOENT)
    {
      // Close connections which are cached by explicitly purging the
      // connection cache maintained by the connector.
      ACE_DEBUG ((LM_DEBUG, "Purging connections from Connection Cache...\n"));

      return this->caching_connect_strategy_.purge_connections ();
    }

  return -1;
}

typedef Accept_Strategy<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>
        ACCEPT_STRATEGY;

static int
cached_connect (STRATEGY_CONNECTOR &con,
                const ACE_INET_Addr &server_addr)
{
  // This will make sure we get the host information correct.
  ACE_INET_Addr remote_addr (server_addr.get_port_number (),
                             ACE_DEFAULT_SERVER_HOST);

  // Perform a blocking connect to the server using the Strategy
  // Connector with a connection caching strategy.
  Client_Svc_Handler *svc_handler = 0;
  int result = con.connect (svc_handler,
                            remote_addr);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("%p\n"),
                       ASYS_TEXT ("connection failed")),
                      -1);

  // Svc_Handler is now idle, so mark it as such and let the cache
  // recycle it.
  svc_handler->idle (1);

  return 0;
}

static void
server (void)
{
  ACE_Reactor::instance ()->reset_event_loop ();

  int result = ACE_Reactor::instance ()->run_event_loop ();
  ACE_ASSERT (result != 1);
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

  for (int i = 1; i <= iterations; ++i)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("iteration %d\n"),
                  i));

      // Connect strategy is required by the <out_of_sockets_handler>.
      ACCEPT_STRATEGY accept_strategy (caching_connect_strategy);

      // Acceptor
      ACCEPTOR acceptor;
      ACE_INET_Addr server_addr;

      // Bind acceptor to any port and then find out what the port
      // was.
      if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &),
                         ACE_Reactor::instance (),
                         0,
                         &accept_strategy) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("%p\n"),
                      ASYS_TEXT ("open")));
          ACE_ASSERT (0);
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

      // Run the cached blocking test.
      int result = cached_connect (strategy_connector,
                                   server_addr);
      ACE_ASSERT (result != -1);

      server ();
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
  ACE_Get_Opt get_opt (argc, argv, "i:p:c:d");

  int cc;

  while ((cc = get_opt ()) != -1)
    switch (cc)
      {
      case 'd':
        debug = 1;
        break;
      case 'i':
        iterations = atoi (get_opt.optarg);
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
                    ASYS_TEXT ("[-t (timeout)] ")
                    ASYS_TEXT ("[-c (caching strategy: lru / lfu / fifo / null [default = all])] ")
                    ASYS_TEXT ("[-i (iterations)] ")
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

  // Start the test only if options are valid.
  ACE_START_TEST (ASYS_TEXT ("Cached_Accept_Conn_Test"));

  // Remove the extra debugging attributes from Log_Msg output.
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  // Do we need to test all the strategies.  Note, that the less
  // useful null strategy is ignored in this case.
  if (caching_strategy_type == ACE_ALL)
    {
      caching_strategy_type = ACE_LRU;
      test_caching_strategy_type ();

      // Default iterations are too many; we the user hasn't specified
      // otherwise, we'll shrink the iterations for LFU and FIFO.
      if (iterations == default_iterations)
        iterations = default_iterations / 100;

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

  ACE_START_TEST (ASYS_TEXT ("Cached_Accept_Conn_Test"));
  ACE_ERROR ((LM_INFO,
              ASYS_TEXT ("C Set++ won't build this test correctly\n")));

#endif /* !__xlC__ || __xlC > 0x0301 */

  ACE_END_TEST;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>;
template class ACE_NOOP_Creation_Strategy<Client_Svc_Handler>;
template class ACE_Concurrency_Strategy<Client_Svc_Handler>;
template class ACE_Connect_Strategy<Client_Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<Client_Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Creation_Strategy<Client_Svc_Handler>;
template class ACE_Hash_Map_Entry<ADDR, Client_Svc_Handler *>;
template class ACE_Hash<ADDR>;
template class ACE_Equal_To<ADDR>;
template class ACE_Hash_Map_Manager<ADDR, Client_Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator<ADDR, Client_Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<ADDR, Client_Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *>;
template class ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *, ACE_SYNCH_RW_MUTEX>;
template class ACE_NOOP_Concurrency_Strategy<Client_Svc_Handler>;
template class ACE_Recycling_Strategy<Client_Svc_Handler>;
template class ACE_Strategy_Connector<Client_Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Svc_Tuple<Client_Svc_Handler>;

template class ACE_Strategy_Acceptor<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>;
template class Accept_Strategy<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Acceptor<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Accept_Strategy<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>;
template class ACE_Creation_Strategy<Server_Svc_Handler>;
template class ACE_Concurrency_Strategy<Server_Svc_Handler>;
template class ACE_Scheduling_Strategy<Server_Svc_Handler>;

template class ACE_Pair<Client_Svc_Handler *, ATTRIBUTES>;
template class ACE_Reference_Pair<ADDR, Client_Svc_Handler *>;
template class ACE_Hash_Map_Entry<ADDR, CACHED_HANDLER>;

template class ACE_Hash_Map_Manager<ADDR, Client_Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ADDR, Client_Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ADDR, Client_Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ADDR, Client_Svc_Handler *, H_KEY,  C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager<ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Bucket_Iterator<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;

// = Caching_Strategy
template class ACE_Hash_Cache_Map_Manager<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, CACHING_STRATEGY, ATTRIBUTES>;

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

template class ACE_Cache_Map_Manager<ADDR, Client_Svc_Handler *, HASH_MAP, HASH_MAP_ITERATOR, HASH_MAP_REVERSE_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>;
template class ACE_Cache_Map_Iterator<ADDR, Client_Svc_Handler *, HASH_MAP_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>;
template class ACE_Cache_Map_Reverse_Iterator<ADDR, Client_Svc_Handler *, HASH_MAP_REVERSE_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>;

#else

template class ACE_Cache_Map_Manager<ADDR, Client_Svc_Handler *, HASH_MAP, CACHING_STRATEGY, ATTRIBUTES>;

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

template class ACE_Cached_Connect_Strategy_Ex<Client_Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Cached_Connect_Strategy<Client_Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>;

template class ACE_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>;
template class ACE_Recyclable_Handler_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>;
template class ACE_Recyclable_Handler_Caching_Utility<ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTRIBUTES>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#pragma instantiate ACE_NOOP_Creation_Strategy<Client_Svc_Handler>
#pragma instantiate ACE_Concurrency_Strategy<Client_Svc_Handler>
#pragma instantiate ACE_Connect_Strategy<Client_Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<Client_Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Creation_Strategy<Client_Svc_Handler>
#pragma instantiate ACE_Hash_Map_Entry<ADDR, Client_Svc_Handler *>
#pragma instantiate ACE_Hash<ADDR>
#pragma instantiate ACE_Equal_To<ADDR>
#pragma instantiate ACE_Hash_Map_Manager<ADDR, Client_Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<ADDR, Client_Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ADDR, Client_Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Entry<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *>
#pragma instantiate ACE_Map_Manager<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator_Base<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Map_Reverse_Iterator<ACE_HANDLE, ACE_Svc_Tuple<Client_Svc_Handler> *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_NOOP_Concurrency_Strategy<Client_Svc_Handler>
#pragma instantiate ACE_Recycling_Strategy<Client_Svc_Handler>
#pragma instantiate ACE_Strategy_Connector<Client_Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Svc_Tuple<Client_Svc_Handler>

#pragma instantiate ACE_Strategy_Acceptor<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate Accept_Strategy<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Acceptor<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Accept_Strategy<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>
#pragma instantiate ACE_Creation_Strategy<Server_Svc_Handler>
#pragma instantiate ACE_Concurrency_Strategy<Server_Svc_Handler>
#pragma instantiate ACE_Scheduling_Strategy<Server_Svc_Handler>

#pragma instantiate ACE_Pair<Client_Svc_Handler *, ATTRIBUTES>
#pragma instantiate ACE_Reference_Pair<ADDR, Client_Svc_Handler *>
#pragma instantiate ACE_Hash_Map_Entry<ADDR, CACHED_HANDLER>

#pragma instantiate ACE_Hash_Map_Manager<ADDR, Client_Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ADDR, Client_Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ADDR, Client_Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ADDR, Client_Svc_Handler *, H_KEY,  C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Manager<ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Bucket_Iterator<ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>

// = Caching_Strategy
#pragma instantiate ACE_Hash_Cache_Map_Manager<ADDR, Client_Svc_Handler *, H_KEY, C_KEYS, CACHING_STRATEGY, ATTRIBUTES>

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

#pragma instantiate ACE_Cache_Map_Manager<ADDR, Client_Svc_Handler *, HASH_MAP, HASH_MAP_ITERATOR, HASH_MAP_REVERSE_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Iterator<ADDR, Client_Svc_Handler *, HASH_MAP_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<ADDR, Client_Svc_Handler *, HASH_MAP_REVERSE_ITERATOR, CACHING_STRATEGY, ATTRIBUTES>

#else

#pragma instantiate ACE_Cache_Map_Manager<ADDR, Client_Svc_Handler *, HASH_MAP, CACHING_STRATEGY, ATTRIBUTES>

#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

#pragma instantiate ACE_Cached_Connect_Strategy_Ex<Client_Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Cached_Connect_Strategy<Client_Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>

#pragma instantiate ACE_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>
#pragma instantiate ACE_Recyclable_Handler_Cleanup_Strategy<ADDR, CACHED_HANDLER, HASH_MAP>
#pragma instantiate ACE_Recyclable_Handler_Caching_Utility<ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTRIBUTES>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

#endif /* CACHED_CONNECT_TEST */
