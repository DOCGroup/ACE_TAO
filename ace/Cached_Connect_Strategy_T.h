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
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//
// ============================================================================

#ifndef CACHED_CONNECT_STRATEGY_T_H
#define CACHED_CONNECT_STRATEGY_T_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Strategies_T.h"
#include "ace/Hash_Cache_Map_Manager_T.h"
#include "ace/Caching_Strategies_T.h"
#include "ace/Functor_T.h"
#include "ace/Pair_T.h"
#include "ace/Synch.h"

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class MUTEX>
class ACE_Cached_Connect_Strategy_Ex : public ACE_Connection_Recycling_Strategy, 
                                       public ACE_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2>
{
  // = TITLE
  //     A connection strategy which caches connections to peers
  //     (represented by <SVC_HANDLER> instances), thereby allowing
  //     subsequent re-use of unused, but available, connections.
  //
  // = DESCRIPTION
  //     <Cached_Connect_Strategy> is intended to be used as a
  //     plug-in connection strategy for <ACE_Strategy_Connector>.
  //     It's added value is re-use of established connections and
  //     tweaking the role of the cache as per the caching strategy.
 
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

  virtual int connect (SVC_HANDLER *&sh,
                       const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                       ACE_Time_Value *timeout,
                       const ACE_PEER_CONNECTOR_ADDR &local_addr,
                       int reuse_addr,
                       int flags,
                       int perms);
  // Connection of the svc_handler with the remote host.
  // This method also encapsulates the connection done with
  // auto_purging under the hood. If the connect failed due to the
  // process running out of file descriptors then, auto_purging of
  // some connections are done from the CONNECTION_CACHE. This frees
  // the descriptors which get used in the connect process and hence
  // the connect operation can succeed.

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
  // requirement. 

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
  typedef ACE_TYPENAME CACHING_STRATEGY::ATTRIBUTES ATTRIBUTES;
  typedef ACE_Hash_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDRESS, SVC_HANDLER *, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>,ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, CACHING_STRATEGY, ATTRIBUTES>
          CONNECTION_CACHE;
  typedef ACE_TYPENAME CONNECTION_CACHE::CACHE_ENTRY 
          CONNECTION_CACHE_ENTRY;
  typedef ACE_TYPENAME CONNECTION_CACHE::key_type
          KEY;
  typedef ACE_TYPENAME CONNECTION_CACHE::mapped_type 
          VALUE;
  typedef ACE_TYPENAME CONNECTION_CACHE::ITERATOR 
          CONNECTION_CACHE_ITERATOR;

  // = Cleanup of the svc_handler.
  typedef ACE_Svc_Cleanup_Strategy<ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDRESS, ACE_Pair<SVC_HANDLER *, ATTRIBUTES>, ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>, MUTEX> >
          SVC_CLEANUP_STRATEGY; 

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

  SVC_CLEANUP_STRATEGY *svc_cleanup_strategy_;
  // The strategy which controls the destruction and closing of the
  // svc_handler.

  CACHING_STRATEGY *caching_strategy_;
  // The strategy which decides how and when entries are to be removed
  // from the connection_cache.

};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Cached_Connect_Strategy_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Cached_Connect_Strategy_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ace/Cached_Connect_Strategy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* CACHED_CONNECT_STRATEGY_T_H */
