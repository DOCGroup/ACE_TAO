/* -*- c++ -*- */
#ifndef JAWS_CACHE_LIST_T_H
#define JAWS_CACHE_LIST_T_H

#include "ace/Malloc.h"
#include "JAWS/Cache_Object.h"

// Forward declarations
template <class EXT_ID, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class JAWS_Cache_Manager;

template <class EXT_ID, class FACTORY, class HASH_FUNC, class EQ_FUNC>
class JAWS_Cache_List_Item;

/// Doubly linked list used to keep cache operations O(1).
template <class EXT_ID, class FACT, class H_FN, class E_FN>
class JAWS_Cache_List
{
public:
  using Cache_Manager = JAWS_Cache_Manager<EXT_ID, FACT, H_FN, E_FN>;
  using Cache_List_Item = JAWS_Cache_List_Item<EXT_ID, FACT, H_FN, E_FN>;

  /// Construct a list that manages at most maxsize objects.
  explicit JAWS_Cache_List (ACE_Allocator *alloc = nullptr, size_t maxsize = 8192);

  ~JAWS_Cache_List ();
  JAWS_Cache_List (JAWS_Cache_List const &) = delete;
  JAWS_Cache_List (JAWS_Cache_List &&) = delete;
  JAWS_Cache_List &operator= (JAWS_Cache_List const &) = delete;
  JAWS_Cache_List &operator= (JAWS_Cache_List &&) = delete;

  bool is_empty () const;
  bool is_full () const;

  size_t size () const;
  size_t maxsize () const;

  /// Grow or shrink the list. Return 0 on success and -1 on error.
  int maxsize (Cache_Manager *cm, size_t new_maxsize);

  /// Insert an object into the list.
  int insert (const EXT_ID &ext_id, JAWS_Cache_Object *const &int_id);

  /// Remove the highest-priority object from the list.
  int remove (EXT_ID &ext_id, JAWS_Cache_Object *&int_id);

  /// Remove an item identified by its address.
  int remove (void *item);

  /// Adjust an item's position after its priority changes.
  int adjust (void *item);

protected:
  /// Insert an item into the list.
  void insert_i (Cache_List_Item *item);

  /// Remove an item without deleting it.
  void remove_i (Cache_List_Item *item);

  /// Remove the head item without deleting it.
  void remove_i ();

private:
  ACE_Allocator *allocator_;

  size_t maxsize_;
  size_t size_;

  Cache_List_Item *item_;

  Cache_List_Item *head_;
  Cache_List_Item *tail_;
};

template <class EXT_ID, class FACT, class H_FN, class E_FN>
class JAWS_Cache_List_Item
{
  friend class JAWS_Cache_List<EXT_ID, FACT, H_FN, E_FN>;

public:
  using Cache_List = JAWS_Cache_List<EXT_ID, FACT, H_FN, E_FN>;

  JAWS_Cache_List_Item (const EXT_ID &ext_id, JAWS_Cache_Object *const &int_id);
  unsigned int priority () const;

private:
  EXT_ID ext_id_;
  JAWS_Cache_Object *int_id_;

  JAWS_Cache_List_Item *next_;
  JAWS_Cache_List_Item *prev_;
};

#include "JAWS/Cache_List_T.cpp"

#endif /* JAWS_CACHE_LIST_T_H */
