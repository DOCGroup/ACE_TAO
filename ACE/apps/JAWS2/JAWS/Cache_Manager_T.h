/* -*- c++ -*- */
// Hey Emacs!  This is a C++ file!
#ifndef JAWS_CACHE_MANAGER_T_H
#define JAWS_CACHE_MANAGER_T_H

#include "ace/Singleton.h"

#include "JAWS/Cache_Object.h"

template <class KEY, class HASH_FUNC, class EQ_FUNC> class JAWS_Cache_Hash;
template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class JAWS_Cache_Heap;
template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class JAWS_Cache_List;

template <class KEY, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class JAWS_Cache_Manager
{
  friend class JAWS_Cache_Hash<KEY, HASH_FUNC, EQ_FUNC>;
  friend class JAWS_Cache_Heap<KEY, FACTORY, HASH_FUNC, EQ_FUNC>;
  friend class JAWS_Cache_List<KEY, FACTORY, HASH_FUNC, EQ_FUNC>;

public:
  using Object_Factory = ACE_Singleton<FACTORY, ACE_SYNCH_MUTEX>;
  using Cache_Hash = JAWS_Cache_Hash<KEY, HASH_FUNC, EQ_FUNC>;
  using Cache_Heap = JAWS_Cache_List<KEY, FACTORY, HASH_FUNC, EQ_FUNC>;

  explicit JAWS_Cache_Manager (ACE_Allocator *alloc = nullptr,
                     JAWS_Cache_Object_Factory *cof = nullptr,

                     size_t hashsize = 8192,   // number of hash buckets
                     size_t maxsize = 65535,   // max number of in memory
                                               // objects

                     size_t maxobjsize = 256,  // max cached object size in kB
                     size_t minobjsize = 0,    // min cached object size in kB

                     size_t highwater = 100,   // max size of cache in MB
                     size_t lowwater = 50,     // min size of cache when
                                               // expiring after highwater
                                               // has been reached

                     int timetolive = -1,      // amt of time the lowest
                                               // priority item is allowed to
                                               // remain in the cache

                     // flag for whether to use counts
                     int counted = 0);

  int open (ACE_Allocator *alloc = nullptr,
            JAWS_Cache_Object_Factory *cof = nullptr,

            size_t hashsize = 1024,   // number of hash buckets
            size_t maxsize = 4096,    // max number of in memory
                                      // objects

            size_t maxobjsize = 5120, // max cached object size in kB
            size_t minobjsize = 0,    // min cached object size in kB

            size_t highwater = 50,    // max size of cache in MB
            size_t lowwater = 30,     // min size of cache when
                                      // expiring after highwater
                                      // has been reached

            int timetolive = -1,      // amount of time the lowest
                                      // priority item is allowed to
                                      // remain in the cache

            // flag for whether to use counts
            int counted = 0);

  ~JAWS_Cache_Manager ();

  int close ();

  /// Retrieve the object associated with key. Return 0 on success, -1 on
  /// failure.
  int GET (const KEY &key, JAWS_Cache_Object *&cobj);

  /// Insert or replace the object associated with key. Return 0 on success,
  /// -1 on failure.
  int PUT (const KEY &key, const void *data, size_t size,
           JAWS_Cache_Object *&obj);

  /// Create a cached object and increment its reference count.
  int MAKE (const void *data, size_t size, JAWS_Cache_Object *&cobj);

  /// Increment the object's reference count.
  int TAKE (JAWS_Cache_Object *const &cobj);

  /// Decrement the object's reference count; return 0 if retained, 1 if
  /// deleted, or -1 on error.
  int DROP (JAWS_Cache_Object *&cobj);

  /// Remove the lowest-priority object from the cache.
  int FLUSH ();

protected:
  /// Retrieve the object associated with key. Return 0 on success, -1 on
  /// failure.
  int GET_i (const KEY &key, JAWS_Cache_Object *&object);

  /// Insert or replace the object associated with key. Return 0 on success,
  /// -1 on failure.
  int PUT_i (const KEY &key, const void *data, size_t size,
             JAWS_Cache_Object *&object);

  /// Remove the lowest-priority object from the cache.
  int FLUSH_i ();

  /// Remove the object associated with key.
  int FLUSH_i (const KEY &key);

  /// Decrement the object's reference count, possibly deleting it.
  int DROP_i (JAWS_Cache_Object *&cobj);

private:
  ACE_Allocator *allocator_;
  JAWS_Cache_Object_Factory *factory_;

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
class JAWS_Cache_Proxy
{
public:
  using Cache_Manager = CACHE_MANAGER;
  using Cache_Manager_Singleton = ACE_Singleton<Cache_Manager, ACE_SYNCH_MUTEX>;

  /// Construct a proxy for a cache lookup.
  JAWS_Cache_Proxy (const KEY &, Cache_Manager * = nullptr);

  /// Construct a proxy that inserts or replaces a cached object.
  JAWS_Cache_Proxy (const KEY &, DATA *, size_t, Cache_Manager * = nullptr);

  virtual ~JAWS_Cache_Proxy ();

  DATA *data () const;
  operator DATA * () const;

  virtual int close (DATA *);

private:
  JAWS_Cache_Object *object_;
  Cache_Manager *manager_;
};

#include "JAWS/Cache_Manager_T.cpp"

#endif /* JAWS_CACHE_MANAGER_T_H */
