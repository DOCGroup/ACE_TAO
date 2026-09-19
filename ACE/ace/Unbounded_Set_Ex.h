// -*- C++ -*-

//=============================================================================
/**
 *  @file Unbounded_Set_Ex.h
 *
 *  @author Douglas C. Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================

#ifndef ACE_UNBOUNDED_SET_EX_H
#define ACE_UNBOUNDED_SET_EX_H
#include /**/ "ace/pre.h"

#include "ace/Node.h"
#include "ace/os_include/os_stddef.h"
#include <iterator>

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

class ACE_Allocator;

template <class T, class C>
class ACE_Unbounded_Set_Ex_Iterator;

template <class T, class C>
class ACE_Unbounded_Set_Ex_Const_Iterator;

template <class T, class C>
class ACE_Unbounded_Set_Ex;

/**
 * @class ACE_Unbounded_Set_Ex_Iterator
 *
 * @brief Implement an iterator over an unbounded set.
 */
template <class T, class C>
class ACE_Unbounded_Set_Ex_Iterator
{
public:
  /// Type definition of the container type.
  using container_type = ACE_Unbounded_Set_Ex<T, C>;

  // = std::iterator_traits typedefs/traits.
  using iterator_category = std::forward_iterator_tag;
  using value_type = typename container_type::value_type;
  using reference = typename container_type::reference;
  using pointer = typename container_type::pointer;
  using difference_type = typename container_type::difference_type;

  ACE_Unbounded_Set_Ex_Iterator (ACE_Unbounded_Set_Ex<T, C> &s, bool end = false);

  // = Iteration methods.

  /// Pass back the @a next_item that hasn't been seen in the Set.
  /// Returns 0 when all items have been seen, else 1.
  int next (T *&next_item);

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance ();

  /// Move to the first element in the set.  Returns 0 if the
  /// set is empty, else 1.
  int first ();

  /// Returns 1 when all items have been seen, else 0.
  int done () const;

  /// Dump the state of an object.
  void dump () const;

  // = STL styled iteration, compare, and reference functions.

  /// Postfix advance.
  ACE_Unbounded_Set_Ex_Iterator<T, C> operator++ (int);

  /// Prefix advance.
  ACE_Unbounded_Set_Ex_Iterator<T, C>& operator++ ();

  /// Returns a reference to the internal element @c this is pointing to.
  T& operator* ();

  /// Check if two iterators point to the same position
  bool operator== (const ACE_Unbounded_Set_Ex_Iterator<T, C> &) const;
  bool operator!= (const ACE_Unbounded_Set_Ex_Iterator<T, C> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Pointer to the current node in the iteration.
  ACE_Node<T, C> *current_;

  /// Pointer to the set we're iterating over.
  ACE_Unbounded_Set_Ex<T, C> *set_;
};

/**
 * @class ACE_Unbounded_Set_Ex_Const_Iterator
 *
 * @brief Implement an const iterator over an unbounded set.
 */
template <class T, class C>
class ACE_Unbounded_Set_Ex_Const_Iterator
{
public:
  using container_type = ACE_Unbounded_Set_Ex<T, C>;

  // = std::iterator_traits typedefs/traits.
  using iterator_category = std::forward_iterator_tag;
  using value_type = typename container_type::const_value_type;
  using reference = typename container_type::const_reference;
  using pointer = typename container_type::const_pointer;
  using difference_type = typename container_type::difference_type;

  ACE_Unbounded_Set_Ex_Const_Iterator (const ACE_Unbounded_Set_Ex<T, C> &s,
                                       bool end = false);

  // = Iteration methods.

  /// Pass back the @a next_item that hasn't been seen in the Set.
  /// @return Returns 0 when all items have been seen, else 1.
  int next (T *&next_item);

  /// Move forward by one element in the set.  Returns 0 when all the
  /// items in the set have been seen, else 1.
  int advance ();

  /// Move to the first element in the set.  Returns 0 if the
  /// set is empty, else 1.
  int first ();

  /// Returns 1 when all items have been seen, else 0.
  int done () const;

  /// Dump the state of an object.
  void dump () const;

  // = STL styled iteration, compare, and reference functions.

  /// Postfix advance.
  ACE_Unbounded_Set_Ex_Const_Iterator<T, C> operator++ (int);

  /// Prefix advance.
  ACE_Unbounded_Set_Ex_Const_Iterator<T, C>& operator++ ();

  /// Returns a reference to the internal element @c this is pointing to.
  T& operator* ();

  /// Check if two iterators point to the same position
  bool operator== (const ACE_Unbounded_Set_Ex_Const_Iterator<T, C> &) const;
  bool operator!= (const ACE_Unbounded_Set_Ex_Const_Iterator<T, C> &) const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Pointer to the current node in the iteration.
  ACE_Node<T, C> *current_;

  /// Pointer to the set we're iterating over.
  const ACE_Unbounded_Set_Ex<T, C> *set_;
};

/**
 * @class ACE_Unbounded_Set_Ex
 *
 * @brief Implement a simple unordered set of <T> of unbounded size.
 *
 * This implementation of an unordered set uses a circular
 * linked list with a dummy node.  This implementation does not
 * allow duplicates, but it maintains FIFO ordering of insertions.
 *
 * This implementation may also be parameterized with a comparator
 * functor, which must implement bool operator () (const T&, const T&) const,
 * returning true if the given items are equivalent.  The default comparator
 * is sufficient for objects reliably compared with operator==.
 *
 * <b> Requirements and Performance Characteristics</b>
 *   - Internal Structure
 *       Circular linked list
 *   - Duplicates allowed?
 *       No
 *   - Random access allowed?
 *       No
 *   - Search speed
 *       Linear
 *   - Insert/replace speed
 *       Linear
 *   - Iterator still valid after change to container?
 *       Yes
 *   - Frees memory for removed elements?
 *       Yes
 *   - Items inserted by
 *       Value
 *   - Requirements for contained type
 *       -# Default constructor
 *       -# Copy constructor
 *       -# operator=
 *       -# operator== const
 */
template <class T, class C>
class ACE_Unbounded_Set_Ex
{
public:
  friend class ACE_Unbounded_Set_Ex_Iterator<T, C>;
  friend class ACE_Unbounded_Set_Ex_Const_Iterator<T, C>;

  // Trait definition.
  using ITERATOR = ACE_Unbounded_Set_Ex_Iterator<T, C>;
  using iterator = ACE_Unbounded_Set_Ex_Iterator<T, C>;
  using CONST_ITERATOR = ACE_Unbounded_Set_Ex_Const_Iterator<T, C>;
  using const_iterator = ACE_Unbounded_Set_Ex_Const_Iterator<T, C>;
  using COMP = C;
  using NODE = ACE_Node<T, C>;

  // = STL typedefs/traits.
  using value_type = T;
  using const_value_type = const T;
  using reference = value_type &;
  using const_reference = const_value_type &;
  using pointer = value_type *;
  using const_pointer = const_value_type *;
  using difference_type = ptrdiff_t;

  /// Constructor.  Use user specified allocation strategy
  /// if specified.
  /**
   * Initialize an empty set using the allocation strategy of the user if
   * provided.
   */
  ACE_Unbounded_Set_Ex (ACE_Allocator *alloc = nullptr);

  /**
   * Initialize an empty set using the allocation strategy of the user if
   * provided, and a given comparator functor.
   */
  ACE_Unbounded_Set_Ex (const C &comparator, ACE_Allocator *alloc = nullptr);

  /// Copy constructor.
  /**
   * Initialize this set to be an exact copy of the set provided.
   */
  ACE_Unbounded_Set_Ex (const ACE_Unbounded_Set_Ex<T, C> &);

  /// Assignment operator.
  /**
   * Perform a deep copy of the rhs into the lhs.
   */
  ACE_Unbounded_Set_Ex<T, C> & operator= (const ACE_Unbounded_Set_Ex<T, C> &);

  /// Destructor.
  /**
   * Destroy the nodes of the set.
   */
  ~ACE_Unbounded_Set_Ex ();

  // = Check boundary conditions.

  /// Returns @c true if the container is empty, otherwise returns @c false.
  /**
   * Constant time is_empty check.
   */
  bool is_empty () const;

  /// Returns @c false.
  /**
   * Always returns @c false since the set can never fill up.
   */
  bool is_full () const;

  // = Classic unordered set operations.

  /// Linear insertion of an item.
  /**
   * Insert @a new_item into the set (doesn't allow duplicates).
   * Returns -1 if failures occur, 1 if item is already present, else
   * 0.
   */
  int insert (const T &new_item);

  /// Insert @a item at the tail of the set (doesn't check for
  /// duplicates).
  /**
   * Constant time insert at the end of the set.
   */
  int insert_tail (const T &item);

  /// Linear remove operation.
  /**
   * Remove first occurrence of @a item from the set.  Returns 0 if
   * it removes the item, -1 if it can't find the item, and -1 if a
   * failure occurs.
   */
  int remove (const T &item);

  /// Finds if @a item occurs in the set.  Returns 0 if find succeeds,
  /// else -1.
  /**
   * Performs a linear find operation.
   */
  int find (const T &item) const;

  /// Size of the set.
  /**
   * Access the size of the set.
   */
  size_t size () const;

  /// Dump the state of an object.
  void dump () const;

  /// Reset the ACE_Unbounded_Set_Ex to be empty.
  /**
   * Delete the nodes of the set.
   */
  void reset ();

  // = STL-styled unidirectional iterator factory.
  iterator begin ();
  iterator end ();
  const_iterator begin () const;
  const_iterator end () const;

  /// Declare the dynamic allocation hooks.
  ACE_ALLOC_HOOK_DECLARE;

private:
  /// Delete all the nodes in the Set.
  void delete_nodes ();

  /// Copy nodes into this set.
  void copy_nodes (const ACE_Unbounded_Set_Ex<T, C> &);

  /// Head of the linked list of Nodes.
  NODE *head_;

  /// Current size of the set.
  size_t cur_size_;

  /// Allocation strategy of the set.
  ACE_Allocator *allocator_;

  /// Comparator to be used
  COMP comp_;
};

ACE_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "ace/Unbounded_Set_Ex.inl"
#endif /* __ACE_INLINE__ */

#include "ace/Unbounded_Set_Ex.cpp"

#include /**/ "ace/post.h"
#endif /* ACE_UNBOUNDED_SET_H */
