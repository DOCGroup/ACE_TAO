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

class TAO_Cache_ExtId;
class TAO_Cache_IntId;

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

  int open (size_t size = ACE_DEFAULT_MAP_SIZE,
            ACE_Allocator *alloc = 0);
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

  int make_idle (HASH_MAP_ENTRY *&entry);
  // Make the entry idle and ready for use.

  size_t current_size (void) const;
  // Return the current size of the cache.

  size_t total_size (void) const;
  // Return the total size of the cache.

private:

  int bind_i (TAO_Cache_ExtId &ext_id,
              TAO_Cache_IntId &int_id);
  // Non-Locking version of bind () call. Calls bind on the
  // Hash_Map_Manager that it holds. If the bind succeeds, it adds the
  // Hash_Map_Entry in to the Connection_Handler for its reference. If
  // the bind fails because of an exiting entry, this method calls the
  // get_last_index_bind ().

  int find_i (const TAO_Cache_ExtId &key,
              TAO_Cache_IntId &value);
  // Non-locking version of find () call. This calls the find () on
  // the underlying Hash_Map_Manager. If the find succeeds, it calls
  // the get_idle_handler ().

  int rebind_i (const TAO_Cache_ExtId &key,
              const TAO_Cache_IntId &value);
  // Non-locking version of rebind () call

  int trybind_i (const TAO_Cache_ExtId &key,
               TAO_Cache_IntId &value);
  // Non-locking version of trybind () call

  int unbind_i (const TAO_Cache_ExtId &key);
  // Non-locking version of unbind () call

  int unbind_i (const TAO_Cache_ExtId &key,
                TAO_Cache_IntId &value);
  // Non-locking version of unbind () call

  int make_idle_i (HASH_MAP_ENTRY *&entry);
  // Non-locking version of make_idle ().

  int get_last_index_bind (TAO_Cache_ExtId &key,
                           TAO_Cache_IntId &val,
                           HASH_MAP_ENTRY *&entry);
  // This is called by the bind () call when a bind fails with a
  // available entry. When a new connection is created in TAO with an
  // already existing endpoint, in addition to an exisitng one, we
  // mark the connections with an index. This method, finds out the
  // last highest index and binds the entry with an index = (last
  // highest index + 1).

  int get_idle_handler (const TAO_Cache_ExtId &ext_id,
                        HASH_MAP_ENTRY *&entry);
  // This is called by the find (). Get an idle handler if one is
  // available in Cache. If an idle one does not exist, we look at the
  // next entry to see whether we  have an entry of the same ext_id
  // type. If so, we check whether they are are idle for use. If we
  // find one we mark it busy, and set the Hash_Map_Entry in the
  // Connection_Handler object.


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
