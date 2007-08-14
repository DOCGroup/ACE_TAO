// -*- C++ -*-

//=============================================================================
/**
 *  @file    Transport_Cache_Manager.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONNECTION_CACHE_MANAGER_H
#define TAO_CONNECTION_CACHE_MANAGER_H

#include /**/ "ace/pre.h"
#include "ace/Null_Mutex.h"
#include "ace/Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"

#include "tao/Cache_Entries.h"
#include "tao/orbconf.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Handle_Set;
template <class T> class ACE_Unbounded_Set;
template <class T> class ACE_Unbounded_Set_Iterator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Connection_Handler;
class TAO_ORB_Core;
class TAO_Resource_Factory;
class TAO_Connection_Purging_Strategy;

template <class ACE_COND_MUTEX> class TAO_Condition;

namespace TAO
{
  typedef ACE_Unbounded_Set<TAO_Connection_Handler*> Connection_Handler_Set;
  /**
   * @class Transport_Cache_Manager
   *
   * @brief The Transport Cache Manager for TAO
   *
   * This class provides interfaces associating a TAO_Cache_ExtId
   * & TAO_Cache_IntId. This class is wrapper around the
   * ACE_Hash_Map_Manager  class which is used as a container to Cache
   * the connections. This class protects the entries with a lock. The
   * map is updated only by holding the lock. The more compelling reason
   * to have the lock in this class and not in the Hash_Map is that, we
   * do quite a bit of work in this class for which we need a lock.
   *
   */
  class TAO_Export Transport_Cache_Manager
  {
  public:
    // results of a find
    enum Find_Result
    {
      CACHE_FOUND_NONE,
      CACHE_FOUND_CONNECTING,
      CACHE_FOUND_BUSY,
      CACHE_FOUND_AVAILABLE
    };

    // Some useful typedef's
    typedef ACE_Hash_Map_Manager_Ex <Cache_ExtId,
                                     Cache_IntId,
                                     ACE_Hash<Cache_ExtId>,
                                     ACE_Equal_To<Cache_ExtId>,
                                     ACE_Null_Mutex>
    HASH_MAP;

    typedef HASH_MAP::iterator HASH_MAP_ITER;

    typedef ACE_Hash_Map_Entry <Cache_ExtId,
                                Cache_IntId>
    HASH_MAP_ENTRY;

    typedef TAO_Condition<TAO_SYNCH_MUTEX> CONDITION;

    // == Public methods
    /// Constructor
    Transport_Cache_Manager (TAO_ORB_Core &orb_core);

    /// Destructor
    ~Transport_Cache_Manager (void);

    /// Add the transport to the cache.
    /**
     * The transport has the property definition based on which caching
     * can be done. This method sets the cache entry status.  By
     * default the status is set to <CODE>ENTRY_IDLE_BUT_NOT_PURGABLE</CODE>
     */
    int cache_transport (TAO_Transport_Descriptor_Interface *prop,
                         TAO_Transport *transport,
                         Cache_Entries_State state = ENTRY_IDLE_BUT_NOT_PURGABLE);

    /// this is just a shortcut for cache_transport with a third argument of
    /// <CODE>ENTRY_IDLE_AND_PURGABLE</CODE>
    /// @@TODO: it should be replaced with a direct call to cache_transport,
    /// but that would require changes to all *_Connection_Handler so I'm
    /// deferring this for now. (wilsond@ociweb.com)
    int cache_idle_transport (TAO_Transport_Descriptor_Interface *prop,
                         TAO_Transport *transport);

    /// Check the Transport Cache to check whether the connection exists
    /// in the Cache and return the connection
    Find_Result find_transport (
      TAO_Transport_Descriptor_Interface *prop,
      TAO_Transport *&transport,
      size_t & busy_count);

    /// Remove entries from the cache depending upon the strategy.
    int purge (void);

    /// Purge the entry from the Cache Map
    int purge_entry (HASH_MAP_ENTRY *& entry);

    /// Mark the entry as invalid for use but keep it in cache.
    void mark_invalid (HASH_MAP_ENTRY * entry);

    /// Mark the entry as connected.
    void mark_connected (HASH_MAP_ENTRY * entry, bool state);

    /// Make the entry idle and ready for use.
    int make_idle (HASH_MAP_ENTRY *entry);

    void set_entry_state (HASH_MAP_ENTRY *entry, TAO::Cache_Entries_State state);

    /// Mark the entry as touched. This call updates the purging
    /// strategy policy information.
    int update_entry (HASH_MAP_ENTRY *&entry);

    /// Close the underlying hash map manager and return any handlers
    /// still registered
    int close (Connection_Handler_Set &handlers);

    /// Return a set of connection handlers that belong to transports
    /// that have a RW wait strategy.
    /**
     * This call is used for a specific use case by the ORB_Core
     * during shutdown. The only way the ORB can wake up threads
     * waiting on these sockstes for replies is to iterate over
     * these blockable transports and close the socket
     * handles. Without these the threads will continue to wait there
     * for ever.
     */
    bool blockable_client_transports (Connection_Handler_Set &handlers);

    /// Return the current size of the cache.
    size_t current_size (void) const;

    /// Return the total size of the cache.
    size_t total_size (void) const;

    /// Return the underlying cache map
    HASH_MAP &map (void);

  private:
    /// Associate @a ext_id with @a int_id. Grabs the lock and calls the
    /// implementation function bind_i.
    int bind (Cache_ExtId &ext_id,
              Cache_IntId &int_id);

    /// Lookup entry<key,value> in the cache. Grabs the lock and calls the
    /// implementation function find_i.
    Find_Result find (
      TAO_Transport_Descriptor_Interface *prop,
      TAO_Transport *&transport,
      size_t & busy_count);

    /**
     * Non-Locking version and actual implementation of bind ()
     * call. Calls bind on the Hash_Map_Manager that it holds. If the
     * bind succeeds, it adds the Hash_Map_Entry in to the
     * Transport for its reference.
     */
    int bind_i (Cache_ExtId &ext_id,
                Cache_IntId &int_id);

    /**
     * Non-locking version and actual implementation of find ()
     * call. This calls the find () on the underlying
     * Hash_Map_Manager. If the find succeeds, it calls the
     * get_idle_transport ().
     */
    Find_Result find_i (
      TAO_Transport_Descriptor_Interface *prop,
      TAO_Transport *&transport,
      size_t & busy_count);

    /// Non-locking version and actual implementation of make_idle ().
    int make_idle_i (HASH_MAP_ENTRY *entry);

    /// Non-locking version and actual implementation of close ()
    int close_i (Connection_Handler_Set &handlers);

    /// Purge the entry from the Cache Map
    int purge_entry_i (HASH_MAP_ENTRY *&entry);

    /// Mark the entry as invalid for use but keep it in cache.
    void mark_invalid_i (HASH_MAP_ENTRY *entry);

  private:
  /**
   * Tries to find if the <int_id_> in entry is available for use.
   */
    bool is_entry_available (const HASH_MAP_ENTRY &entry);

  /**
   * Tries to find if the <int_id_> in entry is connect pending
   */
    bool is_entry_connecting (const HASH_MAP_ENTRY &entry);

#if !defined(ACE_LACKS_QSORT)
    /// Used by qsort
    static int cpscmp(const void* a, const void* b);
#endif

    typedef HASH_MAP_ENTRY** DESCRIPTOR_SET;

    /// Sort the list of entries
    void sort_set (DESCRIPTOR_SET& entries, int size);

    /// Fill sorted_set in with the TAO_Transport_Descriptor_Interface's in
    /// a sorted order.
    int fill_set_i (DESCRIPTOR_SET& sorted_set);

    /// Non-locking version of blockable_client_transports ().
    bool blockable_client_transports_i (Connection_Handler_Set &handlers);

  private:
    /// The percentage of the cache to purge at one time
    int percent_;

    /// The underlying connection purging strategy
    TAO_Connection_Purging_Strategy *purging_strategy_;

    /// The hash map that has the connections
    HASH_MAP cache_map_;

    TAO_SYNCH_MUTEX cache_map_mutex_;

    /// The lock that is used by the cache map
    ACE_Lock *cache_lock_;
  };

}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Transport_Cache_Manager.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /*TAO_CONNECTION_CACHE_MANAGER_H*/
