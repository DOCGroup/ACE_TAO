/* -*- c++ -*- */
// Hey Emacs!  This is a C++ file!
#ifndef JAWS_HASH_BUCKET_T_H
#define JAWS_HASH_BUCKET_T_H

#include "ace/Containers.h"

#define JAWS_HASH_BUCKET_ITEM JAWS_Hash_Bucket_Item<EXT_ID, INT_ID>
#define JAWS_HASH_BUCKET_DLCSTACK JAWS_Hash_Bucket_DLCStack<EXT_ID, INT_ID>
#define JAWS_HASH_BUCKET_DLCSTACK_ITERATOR \
        JAWS_Hash_Bucket_DLCStack_Iterator<EXT_ID, INT_ID>

/**
 * Hash bucket entries for concurrent hash tables.
 * Reader-writer locks allow more concurrent access than mutual exclusion.
 */
template <class EXT_ID, class INT_ID>
class JAWS_Hash_Bucket_Item
{
public:
  JAWS_Hash_Bucket_Item (const EXT_ID &ext_id, const INT_ID &int_id,
                         JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *next = nullptr,
                         JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *prev = nullptr);
  explicit JAWS_Hash_Bucket_Item (JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *next = nullptr,
                         JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *prev = nullptr);

  ~JAWS_Hash_Bucket_Item () = default;
  JAWS_Hash_Bucket_Item (JAWS_Hash_Bucket_Item const &) = delete;
  JAWS_Hash_Bucket_Item (JAWS_Hash_Bucket_Item &&) = delete;
  JAWS_Hash_Bucket_Item &operator= (JAWS_Hash_Bucket_Item const &) = delete;
  JAWS_Hash_Bucket_Item &operator= (JAWS_Hash_Bucket_Item &&) = delete;

  /// Key used to look up an entry.
  EXT_ID ext_id_;

  /// Contents of the entry.
  INT_ID int_id_;

  /// Next item in the bucket's overflow chain.
  JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *next_;

  /// Previous item in the bucket's overflow chain.
  JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *prev_;
};

template <class EXT_ID, class INT_ID> class JAWS_Hash_Bucket_DLCStack_Iterator;

template <class EXT_ID, class INT_ID, class EQ_FUNC>
class JAWS_Hash_Bucket_Manager;

/// Doubly linked circular stack managed by JAWS_Hash_Bucket_Manager.
template <class EXT_ID, class INT_ID>
class JAWS_Hash_Bucket_DLCStack
{
  friend class JAWS_Hash_Bucket_DLCStack_Iterator<EXT_ID, INT_ID>;

public:
  explicit JAWS_Hash_Bucket_DLCStack (ACE_Allocator *alloc = nullptr);
  ~JAWS_Hash_Bucket_DLCStack ();
  JAWS_Hash_Bucket_DLCStack (JAWS_Hash_Bucket_DLCStack const &) = delete;
  JAWS_Hash_Bucket_DLCStack (JAWS_Hash_Bucket_DLCStack &&) = delete;
  JAWS_Hash_Bucket_DLCStack &operator= (JAWS_Hash_Bucket_DLCStack const &) = delete;
  JAWS_Hash_Bucket_DLCStack &operator= (JAWS_Hash_Bucket_DLCStack &&) = delete;

  bool is_empty () const;

  /// Add an item at the head of the list and return the inserted item.
  JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *push (const EXT_ID &ext_id,
                                              const INT_ID &int_id);

  /// Remove and return the first item, or nullptr if empty. The caller owns
  /// the returned node and must free it.
  JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *pop ();

  /// Delete all items and reset the stack to empty.
  void reset ();

  /// Remove and free item if it is still in the stack; return 0 on success or
  /// -1 on error.
  int remove (JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *item);

  ACE_Allocator *allocator_;

private:
  JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *head_;
  JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *tail_;
};

template <class EXT_ID, class INT_ID>
class JAWS_Hash_Bucket_DLCStack_Iterator
{
public:
  explicit JAWS_Hash_Bucket_DLCStack_Iterator (const JAWS_HASH_BUCKET_DLCSTACK &dlcstack);
  JAWS_Hash_Bucket_DLCStack_Iterator (JAWS_Hash_Bucket_DLCStack_Iterator const &) = delete;
  JAWS_Hash_Bucket_DLCStack_Iterator (JAWS_Hash_Bucket_DLCStack_Iterator &&) = delete;
  JAWS_Hash_Bucket_DLCStack_Iterator &operator= (JAWS_Hash_Bucket_DLCStack_Iterator const &) = delete;
  JAWS_Hash_Bucket_DLCStack_Iterator &operator= (JAWS_Hash_Bucket_DLCStack_Iterator &&) = delete;

  /// Move to the first element and clear the done flag; return 0 if empty.
  int first ();

  /// Move to the last element and clear the done flag; return 0 if empty.
  int last ();

  /// Move forward one element; return 0 if empty or positioned at the first
  /// element, and set done when wrapping to the first element.
  int advance ();

  /// Move backward one element; return 0 if empty or positioned at the last
  /// element, and set done when wrapping to the last element.
  int revert ();

  int next (JAWS_HASH_BUCKET_ITEM *&item);
  int next (JAWS_HASH_BUCKET_ITEM *&item) const;

  int prev (JAWS_HASH_BUCKET_ITEM *&item);
  int prev (JAWS_HASH_BUCKET_ITEM *&item) const;

  /// Return 1 when iteration is complete and 0 otherwise.
  int done () const;

private:
  const JAWS_HASH_BUCKET_DLCSTACK &dlcstack_;
  JAWS_HASH_BUCKET_ITEM *next_;
  JAWS_HASH_BUCKET_ITEM *prev_;
  int done_;
};

template <class EXT_ID, class INT_ID, class EQ_FUNC>
class JAWS_Hash_Bucket_Manager
{
public:
  explicit JAWS_Hash_Bucket_Manager (ACE_Allocator *alloc = nullptr);
  int open (ACE_Allocator *alloc = nullptr);

  ~JAWS_Hash_Bucket_Manager () = default;
  JAWS_Hash_Bucket_Manager (JAWS_Hash_Bucket_Manager const &) = delete;
  JAWS_Hash_Bucket_Manager (JAWS_Hash_Bucket_Manager &&) = delete;
  JAWS_Hash_Bucket_Manager &operator= (JAWS_Hash_Bucket_Manager const &) = delete;
  JAWS_Hash_Bucket_Manager &operator= (JAWS_Hash_Bucket_Manager &&) = delete;
  int close ();

  int find (const EXT_ID &ext_id) const;
  /// Find ext_id and write its value to int_id; return 0 if found, -1 if not.
  int find (const EXT_ID &ext_id, INT_ID &int_id) const;

  /// Bind ext_id only when absent; return 0 when inserted, 1 when already
  /// bound, or -1 on failure.
  int bind (const EXT_ID &ext_id, const INT_ID &int_id);
  int trybind (const EXT_ID &ext_id, INT_ID &int_id);

  /// Bind ext_id to int_id, returning the old values when replacing an
  /// existing entry. Return 0 when inserted, 1 when replaced, or -1 on error.
  int rebind (const EXT_ID &ext_id, const INT_ID &int_id,
              EXT_ID &old_ext_id, INT_ID &old_int_id);

  /// Remove ext_id; return 0 on success and -1 if it was not found.
  int unbind (const EXT_ID &ext_id);
  /// Remove ext_id and return the associated value in int_id.
  int unbind (const EXT_ID &ext_id, INT_ID &int_id);

protected:
  /// Return the item for ext_id, or nullptr if it is absent.
  JAWS_Hash_Bucket_Item<EXT_ID, INT_ID> *find_i (const EXT_ID &ext_id) const;

private:
  JAWS_Hash_Bucket_DLCStack<EXT_ID, INT_ID> dlcstack_;
};

#include "JAWS/Hash_Bucket_T.cpp"

#endif /* JAWS_HASH_BUCKET_T_H */
