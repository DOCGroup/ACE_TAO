/* -*- c++ -*- */
#ifndef JAWS_CACHE_HEAP_T_H
#define JAWS_CACHE_HEAP_T_H

#include "ace/Malloc.h"
#include "JAWS/Cache_Object.h"

// Forward declarations
template <class EXT_ID, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class JAWS_Cache_Manager;

template <class EXT_ID, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class JAWS_Cache_Heap_Item;

/// Priority heap used by the JAWS cache.
template <class EXT_ID, class FACT, class H_FN, class E_FN>
class JAWS_Cache_Heap
{
public:
  using Cache_Manager = JAWS_Cache_Manager<EXT_ID, FACT, H_FN, E_FN>;
  using Cache_Heap_Item = JAWS_Cache_Heap_Item<EXT_ID, FACT, H_FN, E_FN>;

  /// Construct a heap that manages at most maxsize objects.
  explicit JAWS_Cache_Heap (ACE_Allocator *alloc = nullptr, size_t maxsize = 8192);

  ~JAWS_Cache_Heap ();
  JAWS_Cache_Heap (JAWS_Cache_Heap const &) = delete;
  JAWS_Cache_Heap (JAWS_Cache_Heap &&) = delete;
  JAWS_Cache_Heap &operator= (JAWS_Cache_Heap const &) = delete;
  JAWS_Cache_Heap &operator= (JAWS_Cache_Heap &&) = delete;

  bool is_empty () const;
  bool is_full () const;

  size_t size () const;
  size_t maxsize () const;

  /// Grow or shrink the heap. Return 0 on success and -1 on error.
  int maxsize (Cache_Manager *cm, size_t new_maxsize);

  /// Insert an object into the heap.
  int insert (const EXT_ID &ext_id, JAWS_Cache_Object *const &int_id);

  /// Remove the highest-priority object from the heap.
  int remove (EXT_ID &ext_id, JAWS_Cache_Object *&int_id);

  /// Remove an item identified by its address.
  int remove (void *item);

  /// Adjust an item's position after its priority changes.
  int adjust (void *item);

protected:
  /// Insert an item into the heap.
  void insert_i (Cache_Heap_Item *item);

  /// Remove the item at pos without deleting it.
  void remove_i (size_t pos);

  /// Remove the top item without deleting it.
  void remove_i ();

private:
  ACE_Allocator *allocator_;

  size_t maxsize_;
  size_t size_;

  /// Allocator-owned array of heap item pointers.
  Cache_Heap_Item **heap_ {};
};

template <class EXT_ID, class FACT, class H_FN, class E_FN>
class JAWS_Cache_Heap_Item
{
  friend class JAWS_Cache_Heap<EXT_ID, FACT, H_FN, E_FN>;

public:
  JAWS_Cache_Heap_Item (const EXT_ID &ext_id, JAWS_Cache_Object *const &int_id);
  unsigned int priority () const;

private:
  EXT_ID ext_id_;
  JAWS_Cache_Object *int_id_;

  size_t heap_idx_;
};

#include "JAWS/Cache_Heap_T.cpp"

#endif /* JAWS_CACHE_HEAP_T_H */
