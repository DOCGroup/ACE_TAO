/* -*- C++ -*- */

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
#include "ace/pre.h"

#include "tao/Cache_Entries.h"


#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Synch_T.h"


#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORB_Core;
class ACE_Handle_Set;
class TAO_Resource_Factory;
class TAO_Connection_Purging_Strategy;

template <class ACE_COND_MUTEX> class TAO_Condition;

typedef ACE_Unbounded_Set<ACE_Event_Handler*> TAO_EventHandlerSet;
typedef ACE_Unbounded_Set_Iterator<ACE_Event_Handler*>
        TAO_EventHandlerSetIterator;

/**
 * @class TAO_Transport_Cache_Manager
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
class TAO_Export TAO_Transport_Cache_Manager
{
public:

  // Some useful typedef's
  typedef ACE_Hash_Map_Manager <TAO_Cache_ExtId,
                                TAO_Cache_IntId,
                                ACE_Null_Mutex>
  HASH_MAP;

  typedef ACE_Hash_Map_Iterator <TAO_Cache_ExtId,
                                 TAO_Cache_IntId,
                                 ACE_Null_Mutex>
  HASH_MAP_ITER;

  typedef ACE_Hash_Map_Entry <TAO_Cache_ExtId,
                              TAO_Cache_IntId>
  HASH_MAP_ENTRY;

  typedef TAO_Condition<TAO_SYNCH_MUTEX> CONDITION;

  // == Public methods

  /// Constructor
  TAO_Transport_Cache_Manager (TAO_ORB_Core &orb_core);

  /// Destructor
  virtual ~TAO_Transport_Cache_Manager (void);

  /// Add the transport to the cache. The transport has the property
  /// definition based on which caching can be done
  int cache_transport (TAO_Transport_Descriptor_Interface *prop,
                       TAO_Transport *transport);


  /// Check the Transport Cache to check whether the connection exists
  /// in the Cache and return the connection
  int find_transport (TAO_Transport_Descriptor_Interface *prop,
                      TAO_Transport *&transport);

  /// Remove entries from the cache depending upon the strategy.
  int purge (void);

  /// Purge the entry from the Cache Map
  int purge_entry (HASH_MAP_ENTRY *&);

  /// Mark the entry as invalid for use but keep it in cache.
  void mark_invalid (HASH_MAP_ENTRY *&);

  /// Make the entry idle and ready for use.
  int make_idle (HASH_MAP_ENTRY *&entry);

  /// Close the underlying hash map manager and return the handle set
  /// that have been registered with the reactor
  int close (ACE_Handle_Set &reactor_registered,
             TAO_EventHandlerSet &unregistered);

  /// Return the current size of the cache.
  size_t current_size (void) const;

  /// Return the total size of the cache.
  size_t total_size (void) const;

private:

  /// Associate <ext_id> with <int_id>. Grabs the lock and calls the
  /// implementation function bind_i.
  int bind (TAO_Cache_ExtId &ext_id,
            TAO_Cache_IntId &int_id);

  /// Lookup entry<key,value> in the cache. Grabs the lock and calls the
  /// implementation function find_i.
  int find (const TAO_Cache_ExtId &key,
            TAO_Cache_IntId &value);

  /**
   * Non-Locking version and actual implementation of bind ()
   * call. Calls bind on the Hash_Map_Manager that it holds. If the
   * bind succeeds, it adds the Hash_Map_Entry in to the
   * Transport for its reference. If the bind fails because
   * of an exiting entry, this method calls the get_last_index_bind
   * ().
   */
  int bind_i (TAO_Cache_ExtId &ext_id,
              TAO_Cache_IntId &int_id);

  /**
   * Non-locking version and actual implementation of find ()
   * call. This calls the find () on the underlying
   * Hash_Map_Manager. If the find succeeds, it calls the
   * get_idle_transport ().
   */
  int find_i (const TAO_Cache_ExtId &key,
              TAO_Cache_IntId &value);

  /// Non-locking version and actual implementation of make_idle ().
  int make_idle_i (HASH_MAP_ENTRY *&entry);

  /// Non-locking version and actual implementation of close ()
  int close_i (ACE_Handle_Set &reactor_registered,
               TAO_EventHandlerSet &unregistered);

  /// Purge the entry from the Cache Map
  int purge_entry_i (HASH_MAP_ENTRY *&entry);

  /// Mark the entry as invalid for use but keep it in cache.
  void mark_invalid_i (HASH_MAP_ENTRY *&);

private:

  /**
   * This is called by the bind () call when a bind fails with a
   * available entry. When a new connection is created in TAO with an
   * already existing endpoint, in addition to an exisitng one, we
   * mark the connections with an index. This method, finds out the
   * last highest index and binds the entry with an index = (last
   * highest index + 1).
   */
  int get_last_index_bind (TAO_Cache_ExtId &key,
                           TAO_Cache_IntId &val,
                           HASH_MAP_ENTRY *&entry);

  /**
   * Tries to find if the <int_id_> in entry is idle for use. If it is
   * idle it is immediately markes as busy and returns a value of
   * 1, else it returns a value of 0
   */
  int is_entry_idle (HASH_MAP_ENTRY *&entry);

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

  /// Look through the sorted set and close the connection on
  /// the required number of items in the set.
  void close_entries (DESCRIPTOR_SET& sorted_set, int size);

  /// Wait for connections if we have reached the limit on the number
  /// of muxed connections. If not (ie. if we dont use a muxed
  /// connection or if we have not reached the limit) this just
  /// behaves as a no-op. <extid> has all the information about the
  /// connection that is being searched.
  int wait_for_connection (TAO_Cache_ExtId &extid);

  /// Is the wakeup useful todo some work?
  int is_wakeup_useful (TAO_Cache_ExtId &extid);
private:

  /// The percentage of the cache to purge at one time
  int percent_;

  /// The underlying connection purging strategy
  TAO_Connection_Purging_Strategy *purging_strategy_;

  /// The hash map that has the connections
  HASH_MAP cache_map_;

  /// The condition variable
  CONDITION *condition_;

  /// The lock that is used by the cache map
  ACE_Lock *cache_lock_;

  /// Number of allowed muxed connections
  CORBA::ULong muxed_number_;

  /// Number of threads waiting for connections
  int no_waiting_threads_;

  /// This is for optimization purposes. In a situation where number
  /// of threads are waiting for connections, the last connection that
  /// is put back is cached here. This should prevent all th threads
  /// trying to search for their required entry.
  TAO_Cache_ExtId *last_entry_returned_;
};

#if defined (__ACE_INLINE__)
# include "tao/Transport_Cache_Manager.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_CONNECTION_CACHE_MANAGER_H*/
