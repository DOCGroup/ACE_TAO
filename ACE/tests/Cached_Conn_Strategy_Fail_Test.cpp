//=============================================================================
/**
 *  @file    Cached_Conn_Strategy_Fail_Test.cpp
 *
 *  This test verifies the return code when ACE_Cached_Connect_Strategy_T
 *  fails to connect.
 *
 *  @author Like Ma <likemartinma@gmail.com>
 */
//=============================================================================


#include "test_config.h"

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



typedef ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH> SVC_HANDLER;

typedef size_t ATTRIBUTES;
typedef std::pair<SVC_HANDLER *, ATTRIBUTES>
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

typedef ACE_Oneshot_Acceptor<SVC_HANDLER, ACE_SOCK_ACCEPTOR>
        ACCEPTOR;

typedef ACE_Strategy_Connector<SVC_HANDLER, ACE_SOCK_CONNECTOR>
        STRATEGY_CONNECTOR;

typedef ACE_NOOP_Creation_Strategy<SVC_HANDLER>
        NULL_CREATION_STRATEGY;

typedef ACE_NOOP_Concurrency_Strategy<SVC_HANDLER>
        NULL_ACTIVATION_STRATEGY;

typedef ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES, ACE_SYNCH_NULL_MUTEX>
        CACHED_CONNECT_STRATEGY;

int run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Cached_Conn_Strategy_Fail_Test"));

  CACHING_STRATEGY *caching_strategy = 0;
  ACE_NEW_NORETURN (caching_strategy, NULL_CACHING_STRATEGY_ADAPTER);
  ACE_TEST_ASSERT (caching_strategy);

  CACHED_CONNECT_STRATEGY caching_connect_strategy (*caching_strategy);
  NULL_CREATION_STRATEGY creation_strategy;
  NULL_ACTIVATION_STRATEGY activation_strategy;

  STRATEGY_CONNECTOR strategy_connector (0,
                                         &creation_strategy,
                                         &caching_connect_strategy,
                                         &activation_strategy);

  ACE_INET_Addr remote_addr (80, "www.google.com");

  ACE_Synch_Options opts(ACE_Synch_Options::USE_TIMEOUT, ACE_Time_Value(0, 1));
  // Perform a blocking connect to the server using the Strategy
  // Connector with a connection caching strategy.
  SVC_HANDLER *svc_handler = 0;
  int result = strategy_connector.connect (svc_handler, remote_addr, opts);
  ACE_TEST_ASSERT (result == -1);
  ACE_TEST_ASSERT (ENOTSUP == errno);

  ACE_END_TEST;
  return 0;
}


// vim: set ts=2 sw=2 sts=2 et:
