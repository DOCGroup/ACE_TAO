/* -*- c++ -*- */
// $Id$

// @@ James, please add a standard "header" here like you see in all
// the other ACE headerfiles.

#ifndef ACE_CACHE_HEAP_T_H
#define ACE_CACHE_HEAP_T_H

#include "ace/Malloc.h"
// @@ James, this #include is wrong...
#include "ace/Cache_Object.h"

// Forward declarations
template <class EXT_ID, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class ACE_Cache_Manager;

template <class EXT_ID, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class ACE_Cache_Heap_Item;

// @@ James, can you please update ALL of these classes and methods to
// use the standard ACE commenting style, i.e., add = TITLE and =
// DESCRIPTION headers and comments for each method.

template <class EXT_ID, class FACT, class H_FN, class E_FN>
class ACE_Cache_Heap
// Roll my own heap here.  Eventually, a heap should be its own
// standalone data structure.
{
public:
  typedef ACE_Cache_Manager<EXT_ID, FACT, H_FN, E_FN> 
          Cache_Manager;
  typedef ACE_Cache_Heap_Item<EXT_ID, FACT, H_FN, E_FN> 
          Cache_Heap_Item;   

  ACE_Cache_Heap (ACE_Allocator *alloc = 0,
                  size_t maxsize = 8192);
  // maxsize is the total number of objects the in memory cache is
  // willing to manage

  ~ACE_Cache_Heap (void);

  int is_empty (void) const;
  int is_full (void) const;

  size_t size (void) const;
  size_t maxsize (void) const;

  int maxsize (Cache_Manager *cm, size_t new_maxsize);
  // attempt to grow (or shrink) the heap.  Return 0 on success, -1 on
  // error.

  int insert (const EXT_ID &ext_id, ACE_Cache_Object *const &int_id);
  // attempt to insert int_id into heap.

  int remove (EXT_ID &ext_id, ACE_Cache_Object *&int_id);
  // attempt to remove the top element of heap.

  int remove (void *item);
  // treat item as a Cache_Heap_Item, and remove it from the heap

  int adjust (void *item);
  // treat item as a Cache_Heap_Item, and alter its heap position

protected:

  void insert_i (Cache_Heap_Item *item);
  // insert item into heap.

  void remove_i (size_t pos);
  // remove the element residing at pos, but do not delete it.

  void remove_i (void);
  // remove the element residing at the top of heap, but do not delete it.

private:

  ACE_Allocator *allocator_;

  size_t maxsize_;
  size_t size_;

  Cache_Heap_Item **heap_;
};


template <class EXT_ID, class FACT, class H_FN, class E_FN>
class ACE_Cache_Heap_Item
{
  friend class ACE_Cache_Heap<EXT_ID, FACT, H_FN, E_FN>;

public:
  ACE_Cache_Heap_Item (const EXT_ID &ext_id, ACE_Cache_Object *const &int_id);
  u_int priority (void);

private:
  EXT_ID ext_id_;
  ACE_Cache_Object *int_id_;

  size_t heap_idx_;
};

// @@ James, please also make sure that you add the 
// #if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
// stuff as well... (see other template files to see how this works).

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Cache_Heap_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#endif /* ACE_CACHE_HEAP_T_H */
