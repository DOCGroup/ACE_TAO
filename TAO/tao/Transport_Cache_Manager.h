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

#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"
#include "tao/Cache_Entries.h"

#include "tao/Connection_Purging_Strategy.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORB_Core;
class ACE_Handle_Set;
class TAO_Resource_Factory;


typedef ACE_Unbounded_Set<ACE_Event_Handler*> TAO_EventHandlerSet;
typedef ACE_Unbounded_Set_Iterator<ACE_Event_Handler*>
        TAO_EventHandlerSetIterator;

/**
 * @class TAO_Transport_Cache_Manager
 *
 * @brief The Transport Cache Manager for TAO
 */
class TAO_Export TAO_Transport_Cache_Manager
{

  // = DESCRIPTION
  //     This class provides interfaces associating a TAO_Cache_ExtId
  //     & TAO_Cache_IntId. This class manages a ACE_Hash_Map_Manager
  //     class which is used as a container to Cache the
  //     connections. This class protects the entries with a lock. The
  //     map can be updated only by holding the lock.

  //     General Note: This class at present has an interface that may
  //     not be needed. But, the interface has just been copied from
  //     the ACE Hash Map Manager classes. The interface wold be
  //     pruned once I get the purging stuff also in. Till then let
  //     the interface be there as it is.
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

  /// Reassociate the <key> with <value>. Grabs the lock and calls the
  /// implementation function find_i.
  int rebind (const TAO_Cache_ExtId &key,
              const TAO_Cache_IntId &value);

  /// Remove <key> from the cache.
  int unbind (const TAO_Cache_ExtId &key);

  /// Remove <key> from the cache, and return the <value> associated with
  /// <key>.
  int unbind (const TAO_Cache_ExtId &key,
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

  /// Non-locking version and actual implementation of rebind () call
  int rebind_i (const TAO_Cache_ExtId &key,
                const TAO_Cache_IntId &value);

  /// Non-locking version and actual implementation of unbind () call
  int unbind_i (const TAO_Cache_ExtId &key);

  /// Non-locking version and actual implementation of unbind () call
  int unbind_i (const TAO_Cache_ExtId &key,
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

private:
  /// The percentage of the cache to purge at one time
  int percent_;

  /// The underlying connection purging strategy
  TAO_Connection_Purging_Strategy *purging_strategy_;

  /// The hash map that has the connections
  HASH_MAP cache_map_;

  /// Lock for the map
  ACE_Lock *cache_lock_;
};

#if defined (__ACE_INLINE__)
# include "tao/Transport_Cache_Manager.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_CONNECTION_CACHE_MANAGER_H*/
