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

#include "tao/Hash_Map_Manager_T.h"



#if !defined (ACE_LACKS_PRAGMA_ONCE)
#define  ACE_LACKS_PRAGMA_ONCE
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Cache_ExtId;
class TAO_Cache_IntId;

class TAO_Export TAO_Connection_Cache_Manager
{
  // = TITLE


  // = DESCRIPTION

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

  typedef ACE_Hash_Map_Reverse_Iterator <TAO_Cache_ExtId,
                                         TAO_Cache_IntId,
                                         ACE_Null_Mutex>
  HASH_MAP_REV_ITER;

  // == Public methods

  TAO_Connection_Cache_Manager (void);
  // Constructor

  TAO_Connection_Cache_Manager (
      ACE_Caching_Strategy<ATTRBIBUTES, CACHING_UTILITY> *strat);
  // Constructor

  virtual ~TAO_Connection_Cache_Manager (void);
  // Destructor

  int open (size_t size = ACE_DEFAULT_MAP_SIZE,
            ACE_Allocator *alloc = 0);
  // Initialize a <HASH_MAP> with <size> elements.

  int bind (const TAO_Cache_ExtId &ext_id,
            const TAO_Cache_IntId &int_id);
  // Associate <ext_id> with <int_id>. If <ext_id> is already in the
  // map then the <Map_Entry> is not changed.  Returns 0 if a new
  // entry is bound successfully, returns 1 if an attempt is made to
  // bind an existing entry, and returns -1 if failures occur.

  int find (const TAO_Cache_ExtId &key,
            TAO_Cache_IntId &value);
  // Lookup entry<key,value> in the cache. If it is not found, returns -1.
  // If the <key> is located in the MAP object, the CACHING_STRATEGY is
  // notified of it via notify_find (int result, ATTRIBUTES &attribute).
  // If notify_find also returns 0 (success), then this function returns
  // 0 (success) and sets the cached value in <value>.

  int rebind (const TAO_Cache_ExtId &key,
              const TAO_Cache_IntId &value);
  // Reassociate the <key> with <value>. If the <key> already exists
  // in the cache then returns 1, on a new bind returns 0 and returns
  // -1 in case of any failures.

  int trybind (const TAO_Cache_ExtId &key,
               TAO_Cache_IntId &value);
  // Associate <key> with <value> if and only if <key> is not in the
  // cache.  If <key> is already in the cache, then the <value>
  // parameter is overwritten with the existing value in the
  // cache. Returns 0 if a new <key>/<value> association is created.
  // Returns 1 if an attempt is made to bind an existing entry.  This
  // function fails for maps that do not allow user specified keys.

  int unbind (const TAO_Cache_ExtId &key);
  // Remove <key> from the cache.

  int unbind (const TAO_Cache_ExtId &key,
              TAO_Cache_IntId &value);
  // Remove <key> from the cache, and return the <value> associated with
  // <key>.

  int purge (void);
  // Remove entries from the cache depending upon the strategy.

  size_t current_size (void) const;
  // Return the current size of the cache.

  size_t total_size (void) const;
  // Return the total size of the cache.

protected:









};

#if defined (__ACE_INLINE__)
# include "tao/Connection_Cache_Manager.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /*TAO_CONNECTION_CACHE_MANAGER_H*/
