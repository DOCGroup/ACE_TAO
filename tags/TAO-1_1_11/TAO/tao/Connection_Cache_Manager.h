/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO
//
// = FILENAME
//    Connection_Cache_Manager.h
//
// = AUTHOR
//    Bala Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_CONNECTION_CACHE_MANAGER_H
#define TAO_CONNECTION_CACHE_MANAGER_H
#include "ace/pre.h"

#include "ace/Hash_Map_Manager_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/TAO_Export.h"
#include "tao/Cache_Entries.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class TAO_ORB_Core;
class ACE_Handle_Set;

class TAO_Export TAO_Connection_Cache_Manager
{
  // = TITLE
  //     The Connection Cache Manager for TAO

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

  TAO_Connection_Cache_Manager (void);
  // Constructor

  virtual ~TAO_Connection_Cache_Manager (void);
  // Destructor

  int cache_handler (TAO_Connection_Descriptor_Interface *prop,
                     TAO_Connection_Handler *handler);
  // Add the handler to the cache. The handler has the property
  // definition based on which caching can be done


  int find_handler (TAO_Connection_Descriptor_Interface *prop,
                    TAO_Connection_Handler *&handler);
  // Check the Connection Cache to check whether the connection exists
  // in the Cache and return the connection

  int open (TAO_ORB_Core *orb_core,
            size_t size = ACE_DEFAULT_MAP_SIZE);
  // Initialize a <HASH_MAP> with <size> elements.

  int bind (TAO_Cache_ExtId &ext_id,
            TAO_Cache_IntId &int_id);
  // Associate <ext_id> with <int_id>. Grabs the lock and calls the
  // implementation function bind_i.

  int find (const TAO_Cache_ExtId &key,
            TAO_Cache_IntId &value);
  // Lookup entry<key,value> in the cache. Grabs the lock and calls the
  // implementation function find_i.

  int rebind (const TAO_Cache_ExtId &key,
              const TAO_Cache_IntId &value);
  // Reassociate the <key> with <value>. Grabs the lock and calls the
  // implementation function find_i.

  int trybind (const TAO_Cache_ExtId &key,
               TAO_Cache_IntId &value);
  // Associate <key> with <value> if and only if <key> is not in the
  // cache.  Grabs the lock and calls the implementation function
  // find_i.

  int unbind (const TAO_Cache_ExtId &key);
  // Remove <key> from the cache.

  int unbind (const TAO_Cache_ExtId &key,
              TAO_Cache_IntId &value);
  // Remove <key> from the cache, and return the <value> associated with
  // <key>.

  int purge (void);
  // Remove entries from the cache depending upon the strategy.

  int purge_entry (HASH_MAP_ENTRY *&);
  // Purge the entry from the Cache Map

  int make_idle (HASH_MAP_ENTRY *&entry);
  // Make the entry idle and ready for use.

  int close (ACE_Handle_Set &handle_set);
  // Close the underlying hash map manager and return the handle set
  // that have been registered with the reactor

  size_t current_size (void) const;
  // Return the current size of the cache.

  size_t total_size (void) const;
  // Return the total size of the cache.

private:

  int bind_i (TAO_Cache_ExtId &ext_id,
              TAO_Cache_IntId &int_id);
  // Non-Locking version and actual implementation of bind ()
  // call. Calls bind on the Hash_Map_Manager that it holds. If the
  // bind succeeds, it adds the Hash_Map_Entry in to the
  // Connection_Handler for its reference. If the bind fails because
  // of an exiting entry, this method calls the get_last_index_bind
  // ().

  int find_i (const TAO_Cache_ExtId &key,
              TAO_Cache_IntId &value);
  // Non-locking version and actual implementation of find ()
  // call. This calls the find () on the underlying
  // Hash_Map_Manager. If the find succeeds, it calls the
  // get_idle_handler ().

  int rebind_i (const TAO_Cache_ExtId &key,
              const TAO_Cache_IntId &value);
  // Non-locking version and actual implementation of rebind () call

  int trybind_i (const TAO_Cache_ExtId &key,
               TAO_Cache_IntId &value);
  // Non-locking version and actual implementation of trybind () call

  int unbind_i (const TAO_Cache_ExtId &key);
  // Non-locking version and actual implementation of unbind () call

  int unbind_i (const TAO_Cache_ExtId &key,
                TAO_Cache_IntId &value);
  // Non-locking version and actual implementation of unbind () call

  int make_idle_i (HASH_MAP_ENTRY *&entry);
  // Non-locking version and actual implementation of make_idle ().

  int close_i (ACE_Handle_Set &handle_set);
  // Non-locking version and actual implementation of close ()

  int purge_entry_i (HASH_MAP_ENTRY *&entry);
  // Purge the entry from the Cache Map

private:

  int get_last_index_bind (TAO_Cache_ExtId &key,
                           TAO_Cache_IntId &val,
                           HASH_MAP_ENTRY *&entry);
  // This is called by the bind () call when a bind fails with a
  // available entry. When a new connection is created in TAO with an
  // already existing endpoint, in addition to an exisitng one, we
  // mark the connections with an index. This method, finds out the
  // last highest index and binds the entry with an index = (last
  // highest index + 1).

  int is_entry_idle (HASH_MAP_ENTRY *&entry);
  // Tries to find if the <int_id_> in entry is idle for use. If it is
  // idle it is immediately markes as busy and returns a value of
  // 1, else it returns a value of 0


private:

  HASH_MAP cache_map_;
  // The hash map that has the connections

  ACE_Lock *cache_lock_;
  // Lock for the map
};

#if defined (__ACE_INLINE__)
# include "tao/Connection_Cache_Manager.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_CONNECTION_CACHE_MANAGER_H*/
