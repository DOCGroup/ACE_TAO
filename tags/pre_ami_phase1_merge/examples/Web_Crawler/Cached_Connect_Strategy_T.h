/* -*- C++ -*- */
// $Id$ 

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Cached_Connect_Strategy_T.h
//
// = AUTHOR
//    Kirthika Parameswaran
//
// ============================================================================

#ifndef CACHED_CONNECT_STRATEGY_T_H
#define CACHED_CONNECT_STRATEGY_T_H

#include "ace/Strategies_T.h"
#include "ace/Hash_Map_Manager_T.h"
#include "Optimal_Cache_Map_Manager_T.h"
#include "Caching_Strategies_T.h"
#include "ace/Functor_T.h"
#include "ace/Pair_T.h"
#include "ace/Synch.h"

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX>
class ACE_Cached_Connect_Strategy_Ex : public ACE_Connection_Recycling_Strategy, public ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>
{
  // = TITLE
  //     A connection strategy which caches connections to peers
  //     (represented by <SVC_HANDLER> instances), thereby allowing
  //     subsequent re-use of unused, but available, connections.
  //
  // = DESCRIPTION
  //     <Mem_Cached_Connect_Strategy> is intended to be used as a
  //     plug-in connection strategy for <ACE_Strategy_Connector>.
  //     It's added value is re-use of established connections.
  //
  // = USAGE
  //     In order to use this appropriately, the user must provide
  //     a template specialization for <ACE_Hash_Addr::compare_i()> and
  //     <ACE_Hash_Addr::hash_i()> based on the address type and the
  //     service handler type.  For example, a specialization using
  //     <ACE_INET_Addr> and <My_Service_Handler> might be:
  //     = BEGIN<NOFILL>
  //     = BEGIN<CODE>
  //     size_t
  //     ACE_Hash_Addr<ACE_INET_Addr, My_Service_Handler>::hash_i(const ACE_INET_Addr &a)
  //     {
  //       return ...;
  //     }
  //     = END<CODE>
  //     = END<NOFILL>
  //
  // = SEE ALSO
  //     <ACE_Hash_Addr>.
public:

  ACE_Cached_Connect_Strategy_Ex (ACE_Creation_Strategy<SVC_HANDLER> *cre_s = 0,
                                  ACE_Concurrency_Strategy<SVC_HANDLER> *con_s = 0,
                                  ACE_Recycling_Strategy<SVC_HANDLER> *rec_s = 0);
  // Constructor

  virtual ~ACE_Cached_Connect_Strategy_Ex (void);
  // Destructor

  virtual int open (ACE_Creation_Strategy<SVC_HANDLER> *cre_s,
                    ACE_Concurrency_Strategy<SVC_HANDLER> *con_s,
                    ACE_Recycling_Strategy<SVC_HANDLER> *rec_s);
  // This methods allow you to change the strategies used by the
  // cached connector.

  virtual int make_svc_handler (SVC_HANDLER *&sh);
  // Template method for making a new <svc_handler>

  virtual int activate_svc_handler (SVC_HANDLER *svc_handler);
  // Template method for activating a new <svc_handler>

  virtual int assign_recycler (SVC_HANDLER *svc_handler,
                               ACE_Connection_Recycling_Strategy *recycler,
                               const void *recycling_act);
  // Template method for setting the recycler information of the
  // svc_handler.

  virtual int prepare_for_recycling (SVC_HANDLER *svc_handler);
  // Template method for preparing the svc_handler for recycling.

  virtual int connect_svc_handler (SVC_HANDLER *&sh,
                                   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                   ACE_Time_Value *timeout,
                                   const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                   int reuse_addr,
                                   int flags,
                                   int perms);
  
  virtual int connect_svc_handler (SVC_HANDLER *&sh,
                                   SVC_HANDLER *&sh_copy,
                                   const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                   ACE_Time_Value *timeout,
                                   const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                   int reuse_addr,
                                   int flags,
                                   int perms);
  // Checks to see if there is already a <SVC_HANDLER> in the cache
  // connected to the <remote_addr>.  If so, we return this pointer.
  // Otherwise we establish the connection, put it into the cache, and
  // return the <SVC_HANDLER> pointer.  <[NOTE]>: the <{reuse_addr}>
  // argument does NOT control re-use of addresses in the cache.
  // Rather, if the underlying protocol requires a "dead time" prior
  // to re-use of its addresses (TCP is a classic example of this),
  // <{and}> the protocol provides a means by which to defeat the dead
  // time, setting this argument to non-zero will defeat the dead-time
  // requirement.  <{Dev. Note: We might want to consider enhancing
  // the interface at some point so that this also controls re-use of
  // the cache.}>

  virtual int purge (const void *recycling_act);
  // Remove from cache.

  virtual int cache (const void *recycling_act);
  // Add to cache.

  virtual int mark_as_closed (const void *recycling_act);
  // Mark as closed.

  virtual int cleanup_hint (const void *recycling_act);
  // Cleanup hint.

  // = Define some useful typedefs.
  typedef ACE_Creation_Strategy<SVC_HANDLER>
          CREATION_STRATEGY;
  typedef ACE_Concurrency_Strategy<SVC_HANDLER>
          CONCURRENCY_STRATEGY;
  typedef ACE_Recycling_Strategy<SVC_HANDLER>
          RECYCLING_STRATEGY;

  // = Super class
  typedef ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>
          CONNECT_STRATEGY;

  // = Typedefs for managing the map
  typedef ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>
          REFCOUNTED_HASH_RECYCLABLE_ADDRESS;

  typedef ACE_Optimal_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDRESS, SVC_HANDLER *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, CACHING_STRATEGY>
          CONNECTION_CACHE;
  typedef ACE_Optimal_Cache_Map_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDRESS, SVC_HANDLER *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, CACHING_STRATEGY>
          CONNECTION_CACHE_ITERATOR;
  typedef ACE_Optimal_Cache_Map_Reverse_Iterator<REFCOUNTED_HASH_RECYCLABLE_ADDRESS, SVC_HANDLER *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, CACHING_STRATEGY>
          CONNECTION_CACHE_REVERSE_ITERATOR;
  
  typedef ACE_TYPENAME CONNECTION_CACHE::CACHE_ENTRY 
          CONNECTION_CACHE_ENTRY;
  // = Strategy accessors
  virtual ACE_Creation_Strategy<SVC_HANDLER> *creation_strategy (void) const;
  virtual ACE_Recycling_Strategy<SVC_HANDLER> *recycling_strategy (void) const;
  virtual ACE_Concurrency_Strategy<SVC_HANDLER> *concurrency_strategy (void) const;

protected:

  virtual int purge_i (const void *recycling_act);
  // Remove from cache (non-locking version).

  virtual int cache_i (const void *recycling_act);
  // Add to cache (non-locking version).

  virtual int mark_as_closed_i (const void *recycling_act);
  // Mark as closed (non-locking version).

  virtual int cleanup_hint_i (const void *recycling_act);
  // Cleanup hint.

  // = Helpers
  int check_hint_i (SVC_HANDLER *&sh,
                    const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                    ACE_Time_Value *timeout,
                    const ACE_PEER_CONNECTOR_ADDR &local_addr,
                    int reuse_addr,
                    int flags,
                    int perms,
                    ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::CONNECTION_CACHE_ENTRY *&entry,
                    int &found);

  int connect_svc_handler_i (SVC_HANDLER *&sh,
                             const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                             ACE_Time_Value *timeout,
                             const ACE_PEER_CONNECTOR_ADDR &local_addr,
                             int reuse_addr,
                             int flags,
                             int perms,
                             int &found);

  int find_or_create_svc_handler_i (SVC_HANDLER *&sh,
                                    const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                    ACE_Time_Value *timeout,
                                    const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                    int reuse_addr,
                                    int flags,
                                    int perms,
                                    ACE_Cached_Connect_Strategy_Ex<SVC_HANDLER, ACE_PEER_CONNECTOR_2, CACHING_STRATEGY, MUTEX>::CONNECTION_CACHE_ENTRY *&entry,
                                    int &found);

  CONNECTION_CACHE connection_cache_;
  // Table that maintains the cache of connected <SVC_HANDLER>s.

  MUTEX lock_;
  // Mutual exclusion for this object.

  // = Strategy objects.

  CREATION_STRATEGY *creation_strategy_;
  // Creation strategy for an <Connector>.

  int delete_creation_strategy_;
  // 1 if <Connector> created the creation strategy and thus should
  // delete it, else 0.

  CONCURRENCY_STRATEGY *concurrency_strategy_;
  // Concurrency strategy for an <Connector>.

  int delete_concurrency_strategy_;
  // 1 if <Connector> created the concurrency strategy and thus should
  // delete it, else 0.

  RECYCLING_STRATEGY *recycling_strategy_;
  // Recycling strategy for an <Connector>.

  int delete_recycling_strategy_;
  // 1 if <Connector> created the recycling strategy and thus should
  // delete it, else 0.
};



#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "Cached_Connect_Strategy_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Cached_Connect_Strategy_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Cached_Connect_Strategy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CACHED_CONNECT_STRATEGY_T_H */
