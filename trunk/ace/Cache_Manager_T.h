/* -*- c++ -*- */
// $Id$

// @@ James, please add a standard "header" here like you see in all
// the other ACE headerfiles.

#ifndef ACE_CACHE_MANAGER_T_H
#define ACE_CACHE_MANAGER_T_H

#include "ace/Singleton.h"
#include "ace/Synch.h"

// @@ James, I think that this #include is wrong...
#include "ace/Cache_Object.h"

// @@ James, can you please update ALL of these classes and methods to
// use the standard ACE commenting style, i.e., add = TITLE and =
// DESCRIPTION headers and comments for each method.

template <class KEY, class HASH_FUNC, class EQ_FUNC> class ACE_Cache_Hash;
template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class ACE_Cache_Heap;

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class ACE_Cache_Manager
{
  friend class ACE_Cache_Hash<KEY, HASH_FUNC, EQ_FUNC>;
  friend class ACE_Cache_Heap<KEY, FACTORY, HASH_FUNC, EQ_FUNC>;

public:
  typedef ACE_Singleton<FACTORY, ACE_SYNCH_MUTEX> 
          Object_Factory;
  typedef ACE_Cache_Hash<KEY, HASH_FUNC, EQ_FUNC> 
          Cache_Hash;
  typedef ACE_Cache_Heap<KEY, FACTORY, HASH_FUNC, EQ_FUNC> 
          Cache_Heap;

  ACE_Cache_Manager (ACE_Allocator *alloc = 0,
                     ACE_Cache_Object_Factory *cof = 0,

                     // @@ James, can you please remove the use of
                     // explicit constants here and use macros instead
                     // that can be redefined, a la the ones in OS.h.

                     size_t hashsize = 1024,   // number of hash buckets
                     size_t maxsize = 4096,    // max number of in memory
                                               // objects

                     size_t maxobjsize = 512,  // max cached object size in kB
                     size_t minobjsize = 0,    // min cached object size in kB

                     size_t highwater = 50,    // max size of cache in MB
                     size_t lowwater = 40,     // min size of cache when
                                               // expiring after highwater
                                               // has been reached

                     int timetolive = -1,      // amt of time the lowest
                                               // priority item is allowed to
                                               // remain in the cache

                     int counted = 0           // flag for whether to use
                                               // counts
                     );

  int open (ACE_Allocator *alloc = 0,
            ACE_Cache_Object_Factory *cof = 0,

            size_t hashsize = 1024,   // number of hash buckets
            size_t maxsize = 4096,    // max number of in memory
                                      // objects

            size_t maxobjsize = 512,  // max cached object size in kB
            size_t minobjsize = 0,    // min cached object size in kB

            size_t highwater = 50,    // max size of cache in MB
            size_t lowwater = 30,     // min size of cache when
                                      // expiring after highwater
                                      // has been reached

            int timetolive = -1,      // amount of time the lowest
                                      // priority item is allowed to
                                      // remain in the cache

            int counted = 0           // flag for whether to use
                                      // counts
            );

  ~ACE_Cache_Manager (void);

  int close (void);

  // Search Methods

  int GET (const KEY &key, ACE_Cache_Object *&cobj);
  // Retrieve the object associated with key from cache.  Return 0 on
  // success, -1 on failure.

  int PUT (const KEY &key, const void *data, size_t size,
           ACE_Cache_Object *&obj);
  // Inserts or replaces object associated with key into cache.
  // Return 0 on success, -1 on failure.

  int MAKE (const void *data, size_t size, ACE_Cache_Object *&cobj);
  // Create a cached object, increment reference count.

  int TAKE (ACE_Cache_Object *const &cobj);
  // Increment reference count.

  int DROP (ACE_Cache_Object *const &cobj);
  // Decrement reference count on cached object, perhaps delete.
  // Returns 0 if only decremented, 1 if deleted, -1 if error.

  int FLUSH (void);
  // Removes lowest priority object from cache.

protected:

  int GET_i (const KEY &key, ACE_Cache_Object *&object);
  // Retrieve the object associated with key from cache.  Return 0 on
  // success, -1 on failure.

  int PUT_i (const KEY &key, const void *data, size_t size,
             ACE_Cache_Object *&object);
  // Inserts or replaces object associated with key into cache.
  // Return 0 on success, -1 on failure.

  int FLUSH_i (void);
  // Removes lowest priority object from cache.

  int FLUSH_i (const KEY &key);
  // Removes object associated with key from cache.

  int DROP_i (ACE_Cache_Object *const &cobj);
  // Decrement reference count on cached object, perhaps delete.

private:

  ACE_Allocator *allocator_;
  ACE_Cache_Object_Factory *factory_;

  size_t hashsize_;
  size_t maxsize_;
  size_t maxobjsize_;
  size_t minobjsize_;
  size_t highwater_;
  size_t lowwater_;
  size_t waterlevel_;
  int timetolive_;
  int counted_;

  Cache_Hash *hash_;
  Cache_Heap *heap_;

  ACE_SYNCH_RW_MUTEX lock_;
};

template <class KEY, class DATA, class CACHE_MANAGER>
class ACE_Cache_Proxy
{
public:
  typedef CACHE_MANAGER Cache_Manager;
  typedef ACE_Singleton<Cache_Manager, ACE_SYNCH_MUTEX>
          Cache_Manager_Singleton;

  ACE_Cache_Proxy (const KEY &, Cache_Manager * = 0);
  // Corresponds to a GET

  ACE_Cache_Proxy (const KEY &,
                   DATA *,
                   size_t,
                   Cache_Manager * = 0);
  // Corresponds to a U/PUT

  ~ACE_Cache_Proxy (void);

  DATA *data (void) const;
  operator DATA * (void) const;

private:
  ACE_Cache_Object *object_;
  Cache_Manager *manager_;
};

// @@ James, please also make sure that you add the 
// #if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
// stuff as well... (see other template files to see how this works).

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
// @@ James, this #include is incorrect.
#include "ace/Cache_Manager_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* ACE_CACHE_MANAGER_T_H */
