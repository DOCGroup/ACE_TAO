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
//     The test illustrates how the <ACE_Strategy_Connector> works by
//   showing how you can cache connections on the client using
//   different caching strategies. Also how connections can be purged
//   explicitly if needed from the connection cache maintained by the
//   connector.
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CACHED_CONNECT_TEST
#define CACHED_CONNECT_TEST 

#include "test_config.h"
#include "ace/OS.h"
#include "ace/inc_user_config.h"
#include "ace/Service_Config.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Addr.h"
#include "ace/INET_Addr.h"
#include "ace/Strategies.h"
#include "ace/Auto_Ptr.h"
#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/Svc_Handler.h"
#include "ace/SOCK_Stream.h"
#include "ace/Acceptor.h"
#include "ace/Handle_Set.h"
#include "ace/Connector.h"
#include "ace/Auto_Ptr.h"
#include "ace/Get_Opt.h"
#include "ace/Caching_Utility_T.h"
#include "ace/Cached_Connect_Strategy_T.h"

ACE_RCSID(tests, Cached_Conn_Test, "$Id$")

#if defined(__BORLANDC__) && __BORLANDC__ >= 0x0530
USELIB("..\ace\aced.lib");
//---------------------------------------------------------------------------
#endif /* defined(__BORLANDC__) && __BORLANDC__ >= 0x0530 */

// Default number of clients/servers.
static int n_servers = 1000;
static double purge_percentage = 20;

typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> 
        Svc_Handler;
typedef ACE_Oneshot_Acceptor<Svc_Handler,
                             ACE_SOCK_ACCEPTOR>
        ACCEPTOR;
typedef ACE_Connector<Svc_Handler,
                      ACE_SOCK_CONNECTOR>
        CONNECTOR;
typedef ACE_Strategy_Connector<Svc_Handler,
                               ACE_SOCK_CONNECTOR>
        STRAT_CONNECTOR;
typedef ACE_NOOP_Creation_Strategy<Svc_Handler>
        NULL_CREATION_STRATEGY;
typedef ACE_NOOP_Concurrency_Strategy<Svc_Handler>
        NULL_ACTIVATION_STRATEGY;

typedef size_t ATTR;
typedef ACE_Pair<Svc_Handler *, ATTR> CACHED_HANDLER;
typedef ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr> 
        REFCOUNTED_HASH_RECYCLABLE_ADDR;
typedef ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR> H_KEY;
typedef ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR> C_KEYS;

typedef ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Null_Mutex>
        HASH_MAP;
typedef ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Null_Mutex>
        HASH_MAP_ITERATOR;
typedef ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>, ACE_Null_Mutex>
        HASH_MAP_REVERSE_ITERATOR;
typedef ACE_Svc_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP>
        CLEANUP_STRATEGY;
typedef ACE_Pair_Caching_Utility<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTR>
        CACHING_UTILITY;
typedef ACE_LRU_Caching_Strategy <REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP, ATTR, CACHING_UTILITY>
        CACHING_STRATEGY;
typedef ACE_Cached_Connect_Strategy_Ex<Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTR, ACE_SYNCH_NULL_MUTEX>
        CACHED_CONNECT_STRATEGY;

//====================================================================

static int
cached_connect (STRAT_CONNECTOR &con,
                const ACE_INET_Addr &server_addr)
{
  Svc_Handler *svc_handler = 0;
 
  // Perform a blocking connect to the server using the Strategy
  // Connector with a connection caching strategy.  Since we are
  // connecting to the same <server_addr> these calls will return
  // the same dynamically allocated <Svc_Handler> for each
  // <connect>.
   int result = con.connect (svc_handler,
                             server_addr);
  if (result == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ASYS_TEXT ("(%P|%t) %p\n"),
                       ASYS_TEXT ("connection failed")),
                      -1);
  
  // Send the data to the server.
  for (char *c = ACE_ALPHABET; *c != '\0'; c++)
    if (svc_handler->peer ().send_n (c, 1) == -1)
      ACE_ERROR_RETURN ((LM_ERROR,
                         ASYS_TEXT ("(%P|%t) %p\n"),
                         ASYS_TEXT ("send_n")),
                        -1);
    else
      ACE_DEBUG ((LM_DEBUG,
                  "%c", c));

  // Svc_Handler is now idle, so mark it as such and let the cache
  // recycle it.
  svc_handler->idle (1);
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
    
  ACE_DEBUG ((LM_DEBUG,
              ASYS_TEXT ("(%P|%t) client %s connected from %d\n"),
              cli_addr.get_host_name (),
              cli_addr.get_port_number ()));
  
  char c; 
  char *t = ACE_ALPHABET;
  ssize_t r_bytes = 0;
 
  // Read data from client (terminate on error).
  while ((r_bytes ==  svc_handler.peer ().recv_n (&c, 1)) > 0)
    {
      ACE_ASSERT (*t == c);
      
      // We need to guard against cached connections, which
      // will send multiple sequences of letters from 'a' ->
      // 'z' through the same connection.
      if (*t == 'z')
        t = ACE_ALPHABET;
      else
        t++;
    }
  
  if (r_bytes == 0)
    ACE_DEBUG ((LM_DEBUG,
                ASYS_TEXT ("(%P|%t) reached end of input, connection closed by client\n")));
  else if (r_bytes == -1)
    {
      if (errno == EWOULDBLOCK)
        ACE_DEBUG ((LM_DEBUG,
                    ASYS_TEXT ("(%P|%t) no input available, going back to reading\n")));
      else
        ACE_ERROR_RETURN ((LM_ERROR,
                           ASYS_TEXT ("(%P|%t) %p\n"),
                           ASYS_TEXT ("recv_n")),
                          -1);
    }
  
  return 0;
}

static int
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "s:p:");

  int c;

  while ((c = get_opt ()) != -1)
    switch (c)
      {
      case 's':
        n_servers = atoi (get_opt.optarg);
        break;
      case 'p':
        purge_percentage = atoi (get_opt.optarg);
        break;
      case '?':
      case 'h':
      default:
        ACE_ERROR ((LM_ERROR,
                    "usage: %s "
                    "[-s (number of servers to connect to)] "
                    "[-p (purge percent)] \n",
                    argv[0]));
        return -1;
      }

  return 0;
}

static void
test_connection_management (void)
{
  NULL_CREATION_STRATEGY creation_strategy;
  NULL_ACTIVATION_STRATEGY activation_strategy;
  // Configure the Strategy Connector with a strategy that caches
  // connection.
  CACHED_CONNECT_STRATEGY caching_connect_strategy; 

  STRAT_CONNECTOR strat_connector (0,
                                   &creation_strategy,
                                   &caching_connect_strategy,
                                   &activation_strategy);

  // Set the purging percentage explicitly. By default it is 10%
  CACHED_CONNECT_STRATEGY *connect_strategy = 
    ACE_dynamic_cast (CACHED_CONNECT_STRATEGY *,
                      strat_connector.connect_strategy ());

  connect_strategy->caching_strategy ().purge_percent (purge_percentage);
  
  for (int i = 0; i < n_servers; ++i)
    {
       // Acceptor
      ACCEPTOR acceptor;
      ACE_INET_Addr server_addr;

      // Bind acceptor to any port and then find out what the port
      // was.
      if (acceptor.open (ACE_sap_any_cast (const ACE_INET_Addr &)) == -1
          || acceptor.acceptor ().get_local_addr (server_addr) == -1)
        {
          ACE_ERROR ((LM_ERROR,
                      ASYS_TEXT ("(%P|%t) %p\n"),
                      ASYS_TEXT ("open")));
        }
      else
        ACE_DEBUG ((LM_DEBUG,
                      ASYS_TEXT ("(%P|%t) starting server at port %d\n"),
                      server_addr.get_port_number ()));
      
      int result = 0;
      // Run the cached blocking test.
      ACE_DEBUG ((LM_DEBUG,
                  ASYS_TEXT ("(%P|%t) *starting cached blocking connect\n")));

      result = cached_connect (strat_connector,
                               server_addr);
      ACE_ASSERT (result != -1);     
      
      result = server (&acceptor);
      if (result == -1)
        {
          // Close connections which are cached by forcebully purging
          // the connection cache maintained by the connector.
          ACE_DEBUG ((LM_DEBUG, "Purging connections from Connection Cache...\n"));

          int retval = connect_strategy->purge_connections (purge_percentage);
          ACE_ASSERT (retval != -1);
            
        }
    } 
}

int main (int argc, ASYS_TCHAR *argv[])
{
  ACE_START_TEST (ASYS_TEXT ("Cached_Conn_Test"));
  ACE_LOG_MSG->clr_flags (ACE_Log_Msg::VERBOSE_LITE);

  
  // Obtain the <number of servers> to connect to. Also the purge
  // percentage using which the entries in the connection cache of
  // the connector will be removed.
  int result = parse_args (argc, argv);
  ACE_ASSERT (result == 0);
  
  test_connection_management ();

  ACE_LOG_MSG->set_flags (ACE_Log_Msg::VERBOSE_LITE); 
  ACE_END_TEST;
  return 0;
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>;
template class ACE_Cached_Connect_Strategy_Ex<Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTR, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>;
template class ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>;
template class ACE_NOOP_Creation_Strategy<Svc_Handler>;
template class ACE_Concurrency_Strategy<Svc_Handler>;
template class ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>;
template class ACE_Creation_Strategy<Svc_Handler>;
template class ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *>;
template class ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>;
template class ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>;
template class ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, C_KEYS, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>, C_KEYS, ACE_SYNCH_RW_MUTEX>;
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

template class ACE_Pair<Svc_Handler *, ATTR>;
template class ACE_Reference_Pair<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *>;
template class ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER>;

template class ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY,  C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>;

template class ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
template class ACE_Hash_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, CACHING_STRATEGY, ATTR>;
template class ACE_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, HASH_MAP, HASH_MAP::iterator, HASH_MAP::reverse_iterator, CACHING_STRATEGY, ATTR>;
template class ACE_Cache_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, HASH_MAP::iterator, CACHING_STRATEGY, ATTR>;
template class ACE_Cache_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, HASH_MAP::reverse_iterator, CACHING_STRATEGY, ATTR>;
template class ACE_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP>;
template class ACE_Default_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP>;
template class ACE_Pair_Caching_Utility<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP::iterator, ATTR>;
template class ACE_Svc_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP>;
template class ACE_LRU_Caching_Strategy <REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP, ATTR, CACHING_UTILITY>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
#pragma instantiate ACE_Cached_Connect_Strategy_Ex<Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTR, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Cached_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR, ACE_SYNCH_NULL_MUTEX>
#pragma instantiate ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>
#pragma instantiate ACE_NOOP_Creation_Strategy<Svc_Handler>
#pragma instantiate ACE_Concurrency_Strategy<Svc_Handler>
#pragma instantiate ACE_Connect_Strategy<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Connector<Svc_Handler, ACE_SOCK_CONNECTOR>
#pragma instantiate ACE_Creation_Strategy<Svc_Handler>
#pragma instantiate ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *>
#pragma instantiate ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDR>
#pragma instantiate ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDR>
#pragma instantiate ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_SYNCH_RW_MUTEX>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_SYNCH_RW_MUTEX>
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

#pragma instantiate ACE_Pair<Svc_Handler *, ATTR>
#pragma instantiate ACE_Reference_Pair<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *>
#pragma instantiate ACE_Hash_Map_Entry<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER>

#pragma instantiate ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, H_KEY, C_KEYS, CACHING_STRATEGY, ATTR>
#pragma instantiate ACE_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, HASH_MAP, HASH_MAP::iterator, HASH_MAP::reverse_iterator, CACHING_STRATEGY, ATTR>
#pragma instantiate ACE_Cache_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, HASH_MAP::iterator, CACHING_STRATEGY, ATTR>
#pragma instantiate ACE_Cache_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDR, Svc_Handler *, HASH_MAP::reverse_iterator, CACHING_STRATEGY, ATTR>
#pragma instantiate ACE_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP>
#pragma instantiate ACE_Default_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP>
#pragma instantiate ACE_Pair_Caching_Utility<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP::iterator, ATTR>
#pragma instantiate ACE_Svc_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP>
#pragma instantiate ACE_LRU_Caching_Strategy <REFCOUNTED_HASH_RECYCLABLE_ADDR, CACHED_HANDLER, HASH_MAP, ATTR, CACHING_UTILITY>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#endif /* CACHED_CONNECT_TEST */
