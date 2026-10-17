
//=============================================================================
/**
 *  @file    Cached_Accept_Conn_Test.h
 *
 *  Define class needed for generating templates.
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

using ATTRIBUTES = size_t;
using CACHED_HANDLER = std::pair<Client_Svc_Handler *, ATTRIBUTES>;
using ACE_ADDR = ACE_Refcounted_Hash_Recyclable<ACE_INET_Addr>;
using H_KEY = ACE_Hash<ACE_ADDR>;
using C_KEYS = ACE_Equal_To<ACE_ADDR>;

using HASH_MAP = ACE_Hash_Map_Manager_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
using HASH_MAP_ITERATOR = ACE_Hash_Map_Iterator_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;
using HASH_MAP_REVERSE_ITERATOR =
  ACE_Hash_Map_Reverse_Iterator_Ex<ACE_ADDR, CACHED_HANDLER, H_KEY, C_KEYS, ACE_Null_Mutex>;

using CLEANUP_STRATEGY = ACE_Recyclable_Handler_Cleanup_Strategy<ACE_ADDR, CACHED_HANDLER, HASH_MAP>;
using CACHING_UTILITY =
  ACE_Recyclable_Handler_Caching_Utility<ACE_ADDR, CACHED_HANDLER, HASH_MAP, HASH_MAP_ITERATOR, ATTRIBUTES>;

using LRU_CACHING_STRATEGY = ACE_LRU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;

using LFU_CACHING_STRATEGY = ACE_LFU_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;
using FIFO_CACHING_STRATEGY = ACE_FIFO_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;
using NULL_CACHING_STRATEGY = ACE_Null_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;
using LRU_CACHING_STRATEGY_ADAPTER =
  ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, LRU_CACHING_STRATEGY>;
using LFU_CACHING_STRATEGY_ADAPTER =
  ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, LFU_CACHING_STRATEGY>;
using FIFO_CACHING_STRATEGY_ADAPTER =
  ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, FIFO_CACHING_STRATEGY>;
using NULL_CACHING_STRATEGY_ADAPTER =
  ACE_Caching_Strategy_Adapter<ATTRIBUTES, CACHING_UTILITY, NULL_CACHING_STRATEGY>;
using CACHING_STRATEGY = ACE_Caching_Strategy<ATTRIBUTES, CACHING_UTILITY>;

using ACCEPTOR = ACE_Strategy_Acceptor<Server_Svc_Handler, ACE_SOCK_ACCEPTOR>;

using STRATEGY_CONNECTOR = ACE_Strategy_Connector<Client_Svc_Handler, ACE_SOCK_CONNECTOR>;

using NULL_CREATION_STRATEGY = ACE_NOOP_Creation_Strategy<Client_Svc_Handler>;

using NULL_ACTIVATION_STRATEGY = ACE_NOOP_Concurrency_Strategy<Client_Svc_Handler>;

using CACHED_CONNECT_STRATEGY =
  ACE_Cached_Connect_Strategy_Ex<Client_Svc_Handler, ACE_SOCK_CONNECTOR, CACHING_STRATEGY, ATTRIBUTES,
                                 ACE_SYNCH_NULL_MUTEX>;

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
  using ACCEPT_STRATEGY_BASE = ACE_Accept_Strategy<SVC_HANDLER, ACE_PEER_ACCEPTOR_2>;

  /// Handler for removing cached connections.
  int out_of_sockets_handler ();

  CACHED_CONNECT_STRATEGY &caching_connect_strategy_;
};

#endif /* ACE_TESTS_CACHED_ACCEPT_CONN_TEST_H */
