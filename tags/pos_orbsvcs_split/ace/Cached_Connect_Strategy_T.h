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
#include "ace/pre.h"
#define CACHED_CONNECT_STRATEGY_T_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Strategies_T.h"
#include "ace/Hash_Cache_Map_Manager_T.h"
#include "ace/Caching_Strategies_T.h"
#include "ace/Functor_T.h"
#include "ace/Pair_T.h"
#include "ace/Synch.h"

// For linkers which cant grok long names...
#define ACE_Cached_Connect_Strategy_Ex ACCSE

template <class SVC_HANDLER, ACE_PEER_CONNECTOR_1, class CACHING_STRATEGY, class ATTRIBUTES, class MUTEX>
class ACE_Cached_Connect_Strategy_Ex : public ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>
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

  ACE_Cached_Connect_Strategy_Ex (CACHING_STRATEGY &caching_s,
                                  ACE_Creation_Strategy<SVC_HANDLER> *cre_s = 0,
                                  ACE_Concurrency_Strategy<SVC_HANDLER> *con_s = 0,
                                  ACE_Recycling_Strategy<SVC_HANDLER> *rec_s = 0,
                                  MUTEX *lock = 0,
                                  int delete_lock = 0);
  // Constructor

  virtual ~ACE_Cached_Connect_Strategy_Ex (void);
  // Destructor

  virtual int purge_connections (void);
  // Explicit purging of connection entries from the connection cache.

  virtual int mark_as_closed_i (const void *recycling_act);
  // Mark as closed (non-locking version). This is used during the cleanup of the
  // connections purged.

  // = Typedefs for managing the map
  typedef ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>
          REFCOUNTED_HASH_RECYCLABLE_ADDRESS;
  typedef ACE_Hash_Cache_Map_Manager<REFCOUNTED_HASH_RECYCLABLE_ADDRESS,
                                     SVC_HANDLER *,
                                     ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>,
                                     ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>,
                                     CACHING_STRATEGY,
                                     ATTRIBUTES>
          CONNECTION_CACHE;
  typedef ACE_TYPENAME CONNECTION_CACHE::CACHE_ENTRY
          CONNECTION_CACHE_ENTRY;
  typedef ACE_TYPENAME CONNECTION_CACHE::key_type
          KEY;
  typedef ACE_TYPENAME CONNECTION_CACHE::mapped_type
          VALUE;

  // = Cleanup of the svc_handler.
  typedef ACE_Recyclable_Handler_Cleanup_Strategy<REFCOUNTED_HASH_RECYCLABLE_ADDRESS,
                                                  ACE_Pair<SVC_HANDLER *, ATTRIBUTES>,
                                                  ACE_Hash_Map_Manager_Ex<REFCOUNTED_HASH_RECYCLABLE_ADDRESS,
                                                                          ACE_Pair<SVC_HANDLER *, ATTRIBUTES>,
                                                                          ACE_Hash<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>,
                                                                          ACE_Equal_To<REFCOUNTED_HASH_RECYCLABLE_ADDRESS>,
                                                                          MUTEX> >
          CLEANUP_STRATEGY;

  typedef ACE_Cached_Connect_Strategy<SVC_HANDLER, ACE_PEER_CONNECTOR_2, MUTEX>
          CCSBASE;

  // = Accessor.
  CACHING_STRATEGY &caching_strategy (void);

protected:

  int find (ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR> &search_addr,
            ACE_Hash_Map_Entry<ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>, ACE_Pair<SVC_HANDLER *, ATTRIBUTES> > *&entry);
  // Find an idle handle.

  virtual int purge_i (const void *recycling_act);
  // Remove from cache (non-locking version).

  virtual int cache_i (const void *recycling_act);
  // Add to cache (non-locking version).

  virtual int recycle_state_i (const void *recycling_act,
                               ACE_Recyclable_State new_state);
  virtual ACE_Recyclable_State recycle_state_i (const void *recycling_act) const;
  // Get/Set <recycle_state> (non-locking version).

  virtual int cleanup_hint_i (const void *recycling_act,
                              void **act_holder);
  // Cleanup hint and reset <*act_holder> to zero if <act_holder != 0>.

  // = Helpers
  int check_hint_i (SVC_HANDLER *&sh,
                    const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                    ACE_Time_Value *timeout,
                    const ACE_PEER_CONNECTOR_ADDR &local_addr,
                    int reuse_addr,
                    int flags,
                    int perms,
                    ACE_Hash_Map_Entry<ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>, ACE_Pair<SVC_HANDLER *, ATTRIBUTES> > *&entry,
                    int &found);

  int find_or_create_svc_handler_i (SVC_HANDLER *&sh,
                                    const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                    ACE_Time_Value *timeout,
                                    const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                    int reuse_addr,
                                    int flags,
                                    int perms,
                                    ACE_Hash_Map_Entry<ACE_Refcounted_Hash_Recyclable<ACE_PEER_CONNECTOR_ADDR>, ACE_Pair<SVC_HANDLER *, ATTRIBUTES> > *&entry,
                                    int &found);

  virtual int connect_svc_handler_i (SVC_HANDLER *&sh,
                                     const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                                     ACE_Time_Value *timeout,
                                     const ACE_PEER_CONNECTOR_ADDR &local_addr,
                                     int reuse_addr,
                                     int flags,
                                     int perms,
                                     int &found);

  virtual int cached_connect (SVC_HANDLER *&sh,
                              const ACE_PEER_CONNECTOR_ADDR &remote_addr,
                              ACE_Time_Value *timeout,
                              const ACE_PEER_CONNECTOR_ADDR &local_addr,
                              int reuse_addr,
                              int flags,
                              int perms);
  // Connection of the svc_handler with the remote host.  This method
  // also encapsulates the connection done with auto_purging under the
  // hood. If the connect failed due to the process running out of
  // file descriptors then, auto_purging of some connections are done
  // from the CONNECTION_CACHE. This frees the descriptors which get
  // used in the connect process and hence the connect operation can
  // succeed.

#if defined (ACE_HAS_BROKEN_EXTENDED_TEMPLATES)
  void cleanup (void);
  // Since g++ version < 2.8 arent happy with templates, this special
  // method had to be devised to avoid memory leaks and perform
  // cleanup of the <connection_cache_>.
#endif /* ACE_HAS_BROKEN_EXTENDED_TEMPLATES */

  CONNECTION_CACHE connection_cache_;
  // Table that maintains the cache of connected <SVC_HANDLER>s.
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Cached_Connect_Strategy_T.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Cached_Connect_Strategy_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Cached_Connect_Strategy_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include "ace/post.h"
#endif /* CACHED_CONNECT_STRATEGY_T_H */
