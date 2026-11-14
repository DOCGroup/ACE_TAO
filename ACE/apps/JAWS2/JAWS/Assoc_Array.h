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

  /// Return the index associated with key k, or -1 if not found.
  int index (const KEY &k);

  /// Return the data associated with key k, or nullptr if not found.
  DATA * find (const KEY &k);

  /// Return the data at index i, or nullptr if the index is invalid.
  DATA * find_by_index (int i);

  /// Insert copies of the key and data and return the inserted data, or
  /// nullptr on error. If the key already exists, return its existing data.
  DATA * insert (const KEY &k, const DATA &d);

  /// Remove and destroy the item for key k. Return 1 if removed, 0 if not
  /// found, or -1 on error.
  int remove (const KEY &k);

  /// Destroy all keys and associated data.
  void clear ();

protected:
  /// Return the matching index, an available insertion index, or maxsize_ if
  /// the table is full.
  int find_i (const KEY &k);

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

  /// Index of the current item.
  int i_;

  /// Index of the next item.
  int j_;
};

#include "JAWS/Assoc_Array.cpp"

#endif /* !defined (JAWS_ASSOC_ARRAY_H) */
