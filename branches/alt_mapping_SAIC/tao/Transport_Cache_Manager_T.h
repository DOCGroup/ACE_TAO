// -*- C++ -*-

//=============================================================================
/**
 *  @file    Transport_Cache_Manager_T.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONNECTION_CACHE_MANAGER_T_H
#define TAO_CONNECTION_CACHE_MANAGER_T_H

#include /**/ "ace/pre.h"
#include "ace/Null_Mutex.h"
#include "ace/Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Hash_Map_Manager_T.h"

#include "tao/Cache_Entries_T.h"
#include "tao/orbconf.h"

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
#include "ace/Monitor_Size.h"
#endif /* TAO_HAS_MONITOR_POINTS==1 */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Handle_Set;
template <class T> class ACE_Unbounded_Set;
template <class T> class ACE_Unbounded_Set_Iterator;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Connection_Handler;
class TAO_Resource_Factory;

template <class ACE_COND_MUTEX> class TAO_Condition;

namespace TAO
{
  typedef ACE_Unbounded_Set<TAO_Connection_Handler*> Connection_Handler_Set;
  /**
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
  template <typename TT, typename TRDT, typename PSTRAT>
  class Transport_Cache_Manager_T
  {
  public:
    typedef TT transport_type;
    typedef TRDT transport_descriptor_type;
    typedef PSTRAT purging_strategy;

    /// results of a find
    enum Find_Result
    {
      CACHE_FOUND_NONE,
      CACHE_FOUND_CONNECTING,
      CACHE_FOUND_BUSY,
      CACHE_FOUND_AVAILABLE
    };

    typedef Cache_ExtId_T <transport_descriptor_type> Cache_ExtId;
    typedef Cache_IntId_T <transport_type> Cache_IntId;

    // Some useful typedef's
    typedef ACE_Hash_Map_Manager_Ex <Cache_ExtId,
                                     Cache_IntId,
                                     ACE_Hash<Cache_ExtId>,
                                     ACE_Equal_To<Cache_ExtId>,
                                     ACE_Null_Mutex>
    HASH_MAP;

    typedef typename HASH_MAP::iterator HASH_MAP_ITER;

    typedef ACE_Hash_Map_Entry <Cache_ExtId,
                                Cache_IntId>
    HASH_MAP_ENTRY;

    typedef TAO_Condition<TAO_SYNCH_MUTEX> CONDITION;

    // == Public methods
    /// Constructor
    Transport_Cache_Manager_T (
      int percent,
      purging_strategy* purging_strategy,
      size_t cache_maximum,
      bool locked,
      const char *orbid);

    /// Destructor
    ~Transport_Cache_Manager_T (void);

    /// Add the transport to the cache.
    /**
     * The transport has the property definition based on which caching
     * can be done. This method sets the cache entry status.  By
     * default the status is set to <CODE>ENTRY_IDLE_AND_PURGABLE</CODE>
     */
    int cache_transport (transport_descriptor_type *prop,
                         transport_type *transport,
                         Cache_Entries_State state =
                           ENTRY_IDLE_AND_PURGABLE);

    /// Check the Transport Cache to check whether the connection exists
    /// in the Cache and return the connection
    Find_Result find_transport (
      transport_descriptor_type *prop,
      transport_type *&transport,
      size_t & busy_count);

    /// Remove entries from the cache depending upon the strategy.
    int purge (void);

    /// Purge the entry from the Cache Map
    int purge_entry (HASH_MAP_ENTRY *& entry);

    /// Mark the entry as connected.
    void mark_connected (HASH_MAP_ENTRY *& entry, bool state);

    /// Make the entry idle and ready for use.
    int make_idle (HASH_MAP_ENTRY *&entry);

    /// Modify the state setting on the provided entry.
    void set_entry_state (HASH_MAP_ENTRY *&entry,
                          TAO::Cache_Entries_State state);

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
     * waiting on these sockets for replies is to iterate over
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
    /// Lookup entry<key,value> in the cache. Grabs the lock and calls the
    /// implementation function find_i.
    Find_Result find (
      transport_descriptor_type *prop,
      transport_type *&transport,
      size_t & busy_count);

    /**
     * Non-Locking version and actual implementation of bind ()
     * call. Calls bind on the Hash_Map_Manager that it holds. If the
     * bind succeeds, it adds the Hash_Map_Entry in to the
     * Transport for its reference.
     */
    int bind_i (Cache_ExtId &ext_id, Cache_IntId &int_id);

    /**
     * Non-locking version and actual implementation of find ()
     * call. This calls the find () on the underlying
     * Hash_Map_Manager. If the find succeeds, it calls the
     * get_idle_transport ().
     */
    Find_Result find_i (
      transport_descriptor_type *prop,
      transport_type *&transport,
      size_t & busy_count);

    /// Non-locking version and actual implementation of make_idle ().
    int make_idle_i (HASH_MAP_ENTRY *entry);

    /// Non-locking version and actual implementation of close ()
    int close_i (Connection_Handler_Set &handlers);

    /// Purge the entry from the Cache Map
    int purge_entry_i (HASH_MAP_ENTRY *entry);

  private:
    /**
     * Tries to find if the @c int_id_ in @a entry is available for use.
     */
    bool is_entry_available_i (const HASH_MAP_ENTRY &entry);

    /**
     * Tries to find if the @c int_id_ in @a entry is connect pending
     */
    bool is_entry_connecting_i (const HASH_MAP_ENTRY &entry);

    /**
     * Tries to find if the @c int_id_ in @a entry is purgable
     */
    bool is_entry_purgable_i (HASH_MAP_ENTRY &entry);

#if !defined(ACE_LACKS_QSORT)
    /// Used by qsort
    static int cpscmp(const void* a, const void* b);
#endif

    typedef HASH_MAP_ENTRY** DESCRIPTOR_SET;

    /// Sort the list of entries
    void sort_set (DESCRIPTOR_SET& entries, int size);

    /// Fill sorted_set in with the transport_descriptor_type's in
    /// a sorted order.
    int fill_set_i (DESCRIPTOR_SET& sorted_set);

    /// Non-locking version of blockable_client_transports ().
    bool blockable_client_transports_i (Connection_Handler_Set &handlers);

  private:
    /// The percentage of the cache to purge at one time
    int percent_;

    /// The underlying connection purging strategy
    purging_strategy *purging_strategy_;

    /// The hash map that has the connections
    HASH_MAP cache_map_;

    TAO_SYNCH_MUTEX cache_map_mutex_;

    /// The lock that is used by the cache map
    ACE_Lock *cache_lock_;

    /// Maximum size of the cache
    size_t cache_maximum_;

#if defined (TAO_HAS_MONITOR_POINTS) && (TAO_HAS_MONITOR_POINTS == 1)
    /// Connection cache purge monitor.
    ACE::Monitor_Control::Size_Monitor *purge_monitor_;

    /// Connection cache size monitor.
    ACE::Monitor_Control::Size_Monitor *size_monitor_;
#endif /* TAO_HAS_MONITOR_POINTS==1 */
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/Transport_Cache_Manager_T.inl"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Transport_Cache_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("tao/Transport_Cache_Manager_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /*TAO_CONNECTION_CACHE_MANAGER_T_H*/
