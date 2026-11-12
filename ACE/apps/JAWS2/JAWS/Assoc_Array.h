/* -*- c++ -*- */
#ifndef JAWS_ASSOC_ARRAY_H
#define JAWS_ASSOC_ARRAY_H

#include <memory>
#include <utility>
#include <vector>

template <class KEY, class DATA> class JAWS_Assoc_Array_Iterator;

template <class KEY, class DATA>
class JAWS_Assoc_Array
{
friend class JAWS_Assoc_Array_Iterator<KEY, DATA>;

public:
  explicit JAWS_Assoc_Array (int maxsize = 1024);
  ~JAWS_Assoc_Array ();
  JAWS_Assoc_Array (JAWS_Assoc_Array const &) = delete;
  JAWS_Assoc_Array (JAWS_Assoc_Array &&) = delete;
  JAWS_Assoc_Array &operator= (JAWS_Assoc_Array const &) = delete;
  JAWS_Assoc_Array &operator= (JAWS_Assoc_Array &&) = delete;

  int index (const KEY &k);
  // Returns the index into the array associated with key k
  // Returns -1 if not found.

  DATA * find (const KEY &k);
  // Returns the data associated with key k.  nullptr if not found.

  DATA * find_by_index (int i);
  // Returns the data associated with array index i.  Returns nullptr if the
  // index is invalid.

  DATA * insert (const KEY &k, const DATA &d);
  // Inserts a *copy* of the key and data into the associated array.
  // Both KEY and DATA must have well defined semantics for copy
  // construction.  This method returns a pointer to the inserted item
  // copy, or nullptr if an error occurred.  NOTE: if an identical key
  // already exists in the tree, no new item is created, and the
  // returned pointer addresses the existing item associated with the
  // existing key.

  int remove (const KEY &k);
  // Removes the item associated with the given key from the
  // tree and destroys it.  Returns 1 if it found the item
  // and successfully destroyed it, 0 if it did not find the
  // item, or -1 if an error occurred.

  void clear ();
  // Destroys all keys and associated data.

protected:
  int find_i (const KEY &k);
  // If k points to an associated data item, then this function
  // returns the index into the arrays that hold it.  Otherwise, it
  // returns an index suitable to insert the item.  If the item is not
  // found and the table is full, maxsize_ is returned.

private:
  std::vector<std::unique_ptr<KEY>> k_array_;
  std::vector<std::unique_ptr<DATA>> d_array_;
  int maxsize_;
};

template <class KEY, class DATA>
class JAWS_Assoc_Array_Iterator
{
public:
  explicit JAWS_Assoc_Array_Iterator (const JAWS_Assoc_Array<KEY, DATA> &aa);
  ~JAWS_Assoc_Array_Iterator () = default;
  JAWS_Assoc_Array_Iterator (JAWS_Assoc_Array_Iterator const &) = delete;
  JAWS_Assoc_Array_Iterator (JAWS_Assoc_Array_Iterator &&) = delete;
  JAWS_Assoc_Array_Iterator &operator= (JAWS_Assoc_Array_Iterator const &) = delete;
  JAWS_Assoc_Array_Iterator &operator= (JAWS_Assoc_Array_Iterator &&) = delete;

  KEY * key () const;
  DATA * data () const;

  int first ();
  int last ();
  int next ();
  int previous ();
  bool is_done () const;

private:
  const JAWS_Assoc_Array<KEY, DATA> &aa_;

  int i_;
  // The current item pointed by iterator.

  int j_;
  // The next item to be pointed to by iterator.
};

#include "JAWS/Assoc_Array.cpp"

#endif /* !defined (JAWS_ASSOC_ARRAY_H) */
