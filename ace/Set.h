/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Set.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_SET)
#define ACE_SET

#include "ace/ACE.h"

// Forward declarations.
template <class T> class ACE_Unbounded_Set;

// "Cheshire cat" forward decl.
template <class T> class ACE_Set_Node;

template <class T>
class ACE_Unbounded_Set_Iterator
  // = TITLE
  //     Implement an iterator over an unbounded set.
{
public:
  // = Initialization method.
  ACE_Unbounded_Set_Iterator (ACE_Unbounded_Set<T> &s);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Set_Node<T> *current_;
};

// Forward declaration (use the "Cheshire Cat" approach to information
// hiding). 
template <class T>
class ACE_Set_Node;

template <class T>
class ACE_Unbounded_Set
  // = TITLE
  //     Implement a simple unordered set of <T> of unbounded size.
  //     
  // = DESCRIPTION
  //     This implementation of an unordered set uses a linked list.
  //     This implementation does not allow duplicates...
{
friend class ACE_Unbounded_Set_Iterator<T>;
public:
  // = Initialization and termination methods.
  ACE_Unbounded_Set (void);
  ~ACE_Unbounded_Set (void);
  
  // = Classic unordered set operations.
  int insert (const T &new_item);
  // Insert <new_item> into the set (doesn't allow duplicates).
  // Returns -1 if failures occur, 1 if item is already present, else
  // 0.
  
  int remove (const T &item);
  // Remove first occurrence of <item> from the set.  Returns 1 if
  // it removes the item, 0 if it can't find the item, and -1 if a
  // failure occurs.

  int find (const T &item) const;
  // Return first occurrence of <item> from the set.
  // Returns 0 if can't find, else 1.
  
  size_t size (void) const;
  // Size of the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Set_Node<T> *head_;
   // Head of the linked list of Nodes.

  size_t cur_size_;
  // Current size of the set.
};

// Forward declaration.
template <class T, size_t SIZE> 
class ACE_Fixed_Set;

template <class T, size_t SIZE>
class ACE_Fixed_Set_Iterator
  // = TITLE
  //     Interates through an unordered set.
  //     
  // = DESCRIPTION
  //     This implementation of an unordered set uses a fixed array.
  //     Allows deletions while iteration is occurring.
{
public:
  // = Initialization method.
  ACE_Fixed_Set_Iterator (ACE_Fixed_Set<T, SIZE> &s);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Fixed_Set<T, SIZE> &s_; 
  // Set we are iterating over.

  ssize_t next_;
  // How far we've advanced over the set.
};

template <class T, size_t SIZE>
class ACE_Fixed_Set
  // = TITLE
  //     Implement a simple unordered set of <T> with maximum <SIZE>.
  //     
  // = DESCRIPTION
  //     This implementation of an unordered set uses a fixed array.
  //     This implementation does not allow duplicates...
{
friend class ACE_Fixed_Set_Iterator<T, SIZE>;
public:
  // = Initialization and termination methods.
  ACE_Fixed_Set (void);
  ~ACE_Fixed_Set (void);
  
  // = Classic unordered set operations.
  int insert (const T &new_item);
  // Insert <new_item> into the set (doesn't allow duplicates).
  // Returns -1 if failures occur, 1 if item is already present, else
  // 0.
  
  int remove (const T &item);
  // Remove first occurrence of <item> from the set.  Returns 1 if
  // it removes the item, 0 if it can't find the item, and -1 if a
  // failure occurs.
  
  int find (const T &item) const;
  // Return first occurrence of <item> from the set.
  // Returns 0 if can't find, else 1.

  size_t size (void) const;
  // Size of the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  struct
  {
    T item_;
    // Item in the set.
    int is_free_;
    // Keeps track of whether this item is in use or not.
  } search_structure_[SIZE];
  // Holds the contents of the set.

  size_t cur_size_;
  // Current size of the set.
  
  size_t max_size_;
  // Maximum size of the set.
};

// Forward declaration.
template <class T> 
class ACE_Bounded_Set;

template <class T>
class ACE_Bounded_Set_Iterator
  // = TITLE
  //     Interates through an unordered set.
  //     
  // = DESCRIPTION
  //     This implementation of an unordered set uses a Bounded array.
  //     Allows deletions while iteration is occurring.
{
public:
  // = Initialization method.
  ACE_Bounded_Set_Iterator (ACE_Bounded_Set<T> &s);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Bounded_Set<T> &s_; 
  // Set we are iterating over.

  ssize_t next_;
  // How far we've advanced over the set.
};

template <class T>
class ACE_Bounded_Set
  // = TITLE
  //     Implement a simple unordered set of <T> with maximum 
  //     set at creation time.
  // 
  // = DESCRIPTION
  //     This implementation of an unordered set uses a Bounded array.
  //     This implementation does not allow duplicates...
{
friend class ACE_Bounded_Set_Iterator<T>;
public:
  enum 
  {
    DEFAULT_SIZE = 10
  };

  // = Initialization and termination methods.
  ACE_Bounded_Set (void);
  ACE_Bounded_Set (size_t size);
  ~ACE_Bounded_Set (void);
  
  // = Classic unordered set operations.
  int insert (const T &new_item);
  // Insert <new_item> into the set (doesn't allow duplicates).
  // Returns -1 if failures occur, 1 if item is already present, else
  // 0.
  
  int remove (const T &item);
  // Remove first occurrence of <item> from the set.  Returns 1 if it
  // removes the item, 0 if it can't find the item, and -1 if a
  // failure occurs.
  
  int find (const T &item) const;
  // Return first occurrence of <item> from the set.
  // Returns 0 if can't find, else 1.

  size_t size (void) const;
  // Size of the set.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  struct Search_Structure
  {
    T item_;
    // Item in the set.
    int is_free_;
    // Keeps track of whether this item is in use or not.
  };

  Search_Structure *search_structure_;
  // Holds the contents of the set.

  size_t cur_size_;
  // Current size of the set.
  
  size_t max_size_;
  // Maximum size of the set.
};

#if defined (__ACE_INLINE__)
#include "ace/Set.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Set.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Set.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_SET */
