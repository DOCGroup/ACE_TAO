
//=============================================================================
/**
 *  @file    Cached_Accept_Conn_Test.h
 *
 *  $Id$
 *
 *  Define class needed for generating templates. IBM C++ requires this to
 *  be in its own file for auto template instantiation.
 *
 *
 *  @author Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACE_TESTS_CACHED_ACCEPT_CONN_TEST_H
#define ACE_TESTS_CACHED_ACCEPT_CONN_TEST_H

#include "ace/SOCK_Connector.h"
#include "ace/SOCK_Acceptor.h"
#include "ace/SOCK_Stream.h"
#include "ace/INET_Addr.h"
#include "ace/Acceptor.h"
#include "ace/Connector.h"
#include "ace/Svc_Handler.h"
#include "ace/Caching_Utility_T.h"
#include "ace/Cached_Connect_Strategy_T.h"
#include "ace/Handle_Gobbler.h"

class Client_Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:

  Client_Svc_Handler (ACE_Thread_Manager *t = 0);
  int open (void *v = 0);
  int close (u_long flags = 0);
};

class Server_Svc_Handler : public ACE_Svc_Handler<ACE_SOCK_STREAM, ACE_NULL_SYNCH>
{
public:

  Server_Svc_Handler (ACE_Thread_Manager *t = 0);
  int open (void *v = 0);
};

typedef size_t ATTRIBUTES;
typedef std::pair<Client_Svc_Handler *, ATTRIBUTES>
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


template <class SVC_HANDLER, ACE_PEER_ACCEPTOR_1>
class Accept_Strategy : public ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>
{
public:

  /// Constructor.
  Accept_Strategy (CACHED_CONNECT_STRATEGY &caching_connect_strategy);

  /**
   * Initialize the <peer_acceptor_> with <local_addr>.  If the
   * process runs out of descriptors, the unsed svc_handlers from the
   * CONNECTION CACHE are removed.
   */
  int open (const ACE_PEER_ACCEPTOR_ADDR &local_addr,
            bool restart = false);

  /**
   * The default behavior delegates to the <accept> method of the
   * PEER_ACCEPTOR. A check is made here for the process running out
   * of file descriptors. If so, the CONNECTION CACHE is purged of
   * some idle svc_handlers.
   */
  int accept_svc_handler (SVC_HANDLER *svc_handler);

protected:

  typedef ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2> ACCEPT_STRATEGY_BASE;

  /// Handler for removing cached connections.
  int out_of_sockets_handler (void);

  CACHED_CONNECT_STRATEGY &caching_connect_strategy_;
};

#endif /* ACE_TESTS_CACHED_ACCEPT_CONN_TEST_H */
