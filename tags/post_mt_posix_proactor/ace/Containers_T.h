/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Containers.h
//
// = AUTHOR
//    Doug Schmidt
//
// ============================================================================

#ifndef ACE_CONTAINERS_T_H
#define ACE_CONTAINERS_T_H

#include "ace/ACE.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers.h"      /* Need ACE_DLList_Node */

class ACE_Allocator;

template <class T>
class ACE_Bounded_Stack
{
  // = TITLE
  //     Implement a generic LIFO abstract data type.
  //
  // = DESCRIPTION
  //     This implementation of a Stack uses a bounded array
  //     that is allocated dynamically.
public:
  // = Initialization, assignemnt, and termination methods.

  ACE_Bounded_Stack (size_t size);
  // Initialize a new stack so that it is empty.

  ACE_Bounded_Stack (const ACE_Bounded_Stack<T> &s);
  // The copy constructor (performs initialization).

  void operator= (const ACE_Bounded_Stack<T> &s);
  // Assignment operator (performs assignment).

  ~ACE_Bounded_Stack (void);
 // Perform actions needed when stack goes out of scope.

  // = Classic Stack operations.

  int push (const T &new_item);
  // Place a new item on top of the stack.  Returns -1 if the stack
  // is already full, 0 if the stack is not already full, and -1 if
  // failure occurs.

  int pop (T &item);
  // Remove and return the top stack item.  Returns -1 if the stack is
  // already empty, 0 if the stack is not already empty, and -1 if
  // failure occurs.

  int top (T &item) const;
  // Return top stack item without removing it.  Returns -1 if the
  // stack is already empty, 0 if the stack is not already empty, and
  // -1 if failure occurs.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  size_t size (void) const;
  // The number of items in the stack.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  size_t size_;
  // Size of the dynamically allocated data.

  size_t top_;
  // Keeps track of the current top of stack.

  T *stack_;
  // Holds the stack's contents.
};

//----------------------------------------

template <class T, size_t ACE_SIZE>
class ACE_Fixed_Stack
{
  // = TITLE
  //     Implement a generic LIFO abstract data type.
  //
  // = DESCRIPTION
  //     This implementation of a Stack uses a fixed array
  //     with the size fixed at instantiation time.
public:
  // = Initialization, assignemnt, and termination methods.
  ACE_Fixed_Stack (void);
  // Initialize a new stack so that it is empty.

  ACE_Fixed_Stack (const ACE_Fixed_Stack<T, ACE_SIZE> &s);
  // The copy constructor (performs initialization).

  void operator= (const ACE_Fixed_Stack<T, ACE_SIZE> &s);
  // Assignment operator (performs assignment).

  ~ACE_Fixed_Stack (void);
  // Perform actions needed when stack goes out of scope.

  // = Classic Stack operations.

  int push (const T &new_item);
  // Place a new item on top of the stack.  Returns -1 if the stack
  // is already full, 0 if the stack is not already full, and -1 if
  // failure occurs.

  int pop (T &item);
  // Remove and return the top stack item.  Returns -1 if the stack is
  // already empty, 0 if the stack is not already empty, and -1 if
  // failure occurs.

  int top (T &item) const;
  // Return top stack item without removing it.  Returns -1 if the
  // stack is already empty, 0 if the stack is not already empty, and
  // -1 if failure occurs.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  size_t size (void) const;
  // The number of items in the stack.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  size_t size_;
  // Size of the allocated data.

  size_t top_;
  // Keeps track of the current top of stack.

  T stack_[ACE_SIZE];
  // Holds the stack's contents.
};

//----------------------------------------

// Forward declarations.
template <class T> class ACE_Unbounded_Set;
template <class T> class ACE_Unbounded_Set_Iterator;
template <class T> class ACE_Unbounded_Queue;
template <class T> class ACE_Unbounded_Queue_Iterator;
template <class T> class ACE_Unbounded_Stack;
template <class T> class ACE_Unbounded_Stack_Iterator;
template <class T> class ACE_Ordered_MultiSet;
template <class T> class ACE_Ordered_MultiSet_Iterator;

template<class T>
class ACE_Node
{
  // = TITLE
  //     Implementation element in a Queue, Set, and Stack.
public:
  friend class ACE_Unbounded_Queue<T>;
  friend class ACE_Unbounded_Queue_Iterator<T>;
  friend class ACE_Unbounded_Set<T>;
  friend class ACE_Unbounded_Set_Iterator<T>;
  friend class ACE_Unbounded_Stack<T>;
  friend class ACE_Unbounded_Stack_Iterator<T>;

# if ! defined (ACE_HAS_BROKEN_NOOP_DTORS)
  ~ACE_Node (void);
  // This isn't necessary, but it keeps some compilers happy.
# endif /* ! defined (ACE_HAS_BROKEN_NOOP_DTORS) */

private:
  // = Initialization methods
  ACE_Node (const T &i, ACE_Node<T> *n);
  ACE_Node (ACE_Node<T> *n = 0, int = 0);
  ACE_Node (const ACE_Node<T> &n);

  ACE_Node<T> *next_;
  // Pointer to next element in the list of <ACE_Node>s.

  T item_;
  // Current value of the item in this node.
};

template<class T>
class ACE_DNode
{
  // = TITLE
  //     Implementation element in a bilinked list.
  friend class ACE_Ordered_MultiSet<T>;
  friend class ACE_Ordered_MultiSet_Iterator<T>;

public:

  ~ACE_DNode (void);
  // This isn't necessary, but it keeps the compiler happy.

private:

  // = Initialization methods
  ACE_DNode (const T &i, ACE_DNode<T> *n = 0, ACE_DNode<T> *p = 0);

  ACE_DNode<T> *next_;
  // Pointer to next element in the list of <ACE_DNode>s.

  ACE_DNode<T> *prev_;
  // Pointer to previous element in the list of <ACE_DNode>s.

  T item_;
  // Current value of the item in this node.
};


template <class T>
class ACE_Unbounded_Stack
{
  // = TITLE
  //     Implement a generic LIFO abstract data type.
  //
  // = DESCRIPTION
  //     This implementation of an unbounded Stack uses a linked list.
  //     If you use the <insert> or <remove> methods you should keep
  //     in mind that duplicate entries aren't allowed.  In general,
  //     therefore, you should avoid the use of these methods since
  //     they aren't really part of the ADT stack.
public:
  friend class ACE_Unbounded_Stack_Iterator<T>;

  // Trait definition.
  typedef ACE_Unbounded_Stack_Iterator<T> ITERATOR;

  // = Initialization, assignemnt, and termination methods.
  ACE_Unbounded_Stack (ACE_Allocator *alloc = 0);
  // Initialize a new stack so that it is empty.  Use user defined
  // allocation strategy if specified.

  ACE_Unbounded_Stack (const ACE_Unbounded_Stack<T> &s);
  // The copy constructor (performs initialization).

  void operator= (const ACE_Unbounded_Stack<T> &s);
  // Assignment operator (performs assignment).

  ~ACE_Unbounded_Stack (void);
  // Perform actions needed when stack goes out of scope.

  // = Classic Stack operations.

  int push (const T &new_item);
  // Place a new item on top of the stack.  Returns -1 if the stack
  // is already full, 0 if the stack is not already full, and -1 if
  // failure occurs.

  int pop (T &item);
  // Remove and return the top stack item.  Returns -1 if the stack is
  // already empty, 0 if the stack is not already empty, and -1 if
  // failure occurs.

  int top (T &item) const;
  // Return top stack item without removing it.  Returns -1 if the
  // stack is already empty, 0 if the stack is not already empty, and
  // -1 if failure occurs.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  // = Auxiliary methods (not strictly part of the Stack ADT).

  int insert (const T &new_item);
  // Insert <new_item> into the Stack at the head (but doesn't allow
  // duplicates).  Returns -1 if failures occur, 1 if item is already
  // present (i.e., no duplicates are allowed), else 0.

  int remove (const T &item);
  // Remove <item> from the Stack.  Returns 0 if it removes the item,
  // -1 if it can't find the item, and -1 if a failure occurs.

  int find (const T &item) const;
  // Finds if <item> occurs the set.  Returns 0 if finds, else -1.

  size_t size (void) const;
  // The number of items in the stack.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  void delete_all_nodes (void);
  // Delete all the nodes in the stack.

  void copy_all_nodes (const ACE_Unbounded_Stack<T> &s);
  // Copy all nodes from <s> to <this>.

  ACE_Node<T> *head_;
   // Head of the linked list of Nodes.

  size_t cur_size_;
  // Current size of the stack.

  ACE_Allocator *allocator_;
  // Allocation strategy of the stack.
};

template <class T>
class ACE_Unbounded_Stack_Iterator
{
  // = TITLE
  //     Implement an iterator over an unbounded Stack.
public:
  // = Initialization method.
  ACE_Unbounded_Stack_Iterator (ACE_Unbounded_Stack<T> &stack);
  // Move to the first element in the <stack>.

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Stack.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the Stack.  Returns 0 when all the
  // items in the Stack have been seen, else 1.

  int first (void);
  // Move to the first element in the Stack.  Returns 0 if the
  // Stack is empty, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Node<T> *current_;
  // Pointer to the current node in the iteration.

  ACE_Unbounded_Stack<T> &stack_;
  // Pointer to the Stack we're iterating over.
};

template <class T>
class ACE_Unbounded_Queue;

template <class T>
class ACE_Unbounded_Queue_Iterator
{
  // = TITLE
  //     Implement an iterator over an unbounded queue.
public:
  // = Initialization method.
  ACE_Unbounded_Queue_Iterator (ACE_Unbounded_Queue<T> &q, int end = 0);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the queue.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the queue have been seen, else 1.

  int first (void);
  // Move to the first element in the queue.  Returns 0 if the
  // queue is empty, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Node<T> *current_;
  // Pointer to the current node in the iteration.

  ACE_Unbounded_Queue<T> &queue_;
  // Pointer to the queue we're iterating over.
};

template <class T>
class ACE_Unbounded_Queue
{
  // = TITLE
  //     A Queue of "infinite" length.
  //
  // = DESCRIPTION
  //     This implementation of an unbounded queue uses a circular
  //     linked list with a dummy node.
public:
  friend class ACE_Unbounded_Queue_Iterator<T>;

  // Trait definition.
  typedef ACE_Unbounded_Queue_Iterator<T> ITERATOR;

 // = Initialization and termination methods.
  ACE_Unbounded_Queue (ACE_Allocator *alloc = 0);
  // construction.  Use user specified allocation strategy
  // if specified.

  ACE_Unbounded_Queue (const ACE_Unbounded_Queue<T> &);
  // Copy constructor.

  void operator= (const ACE_Unbounded_Queue<T> &);
  // Assignment operator.

  ~ACE_Unbounded_Queue (void);
  // construction.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  // = Classic queue operations.

  int enqueue_tail (const T &new_item);
  // Adds <new_item> to the tail of the queue.  Returns 0 on success,
  // -1 on failure.

  int enqueue_head (const T &new_item);
  // Adds <new_item> to the head of the queue.  Returns 0 on success,
  // -1 on failure.

  int dequeue_head (T &item);
  // Removes and returns the first <item> on the queue.  Returns 0 on
  // success, -1 if the queue was empty.

  // = Additional utility methods.

  void reset (void);
  // Reset the <ACE_Unbounded_Queue> to be empty.

  int get (T *&item, size_t index = 0) const;
  // Get the <index>th element in the set.  Returns -1 if the element
  // isn't in the range <0..size() - 1>, else 0.

  int set (const T &item, size_t index);
  // Set the <index>th element in the set.  Will pad out the set with
  // empty nodes if <index> is beyond the range <0..size() - 1>.
  // Returns -1 on failure, 0 if <index> isn't initially in range, and
  // 0 otherwise.

  size_t size (void) const;
  // The number of items in the queue.

  void dump (void) const;
  // Dump the state of an object.

  // = STL-styled unidirectional iterator factory.
  ACE_Unbounded_Queue_Iterator<T> begin (void);
  ACE_Unbounded_Queue_Iterator<T> end (void);

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  void delete_nodes (void);
  // Delete all the nodes in the queue.

  void copy_nodes (const ACE_Unbounded_Queue<T> &);
  // Copy nodes into this queue.

  ACE_Node<T> *head_;
  // Pointer to the dummy node in the circular linked Queue.

  size_t cur_size_;
  // Current size of the queue.

  ACE_Allocator *allocator_;
  // Allocation Strategy of the queue.
};

template <class T>
class ACE_Double_Linked_List;

template <class T>
class ACE_Double_Linked_List_Iterator
{
  // = TITLE
  //     Implement an iterator over a container double-linked list
  //
  // = DESCRIPTION
  //     Iterate thru the double-linked list.  This class provide
  //     an interface that let users access the internal element
  //     addresses directly, which (IMHO) seems to break the
  //     encasulation.  Notice <class T> must delcare
  //     ACE_Double_Linked_List<T> and
  //     ACE_Double_Linked_List_Iterator as friend classes and class T
  //     should also have data members T* next_ and T* prev_.
public:
  // = Initialization method.
  ACE_Double_Linked_List_Iterator (ACE_Double_Linked_List<T> &);

  // = Iteration methods.

  T *next (void) const;
  // Return the address of next (current) unvisited item in the list.
  // 0 if there is no more element available.

  int advance (void);
  // Move forward by one element in the list.  Returns 0 when all the
  // items in the list have been seen, else 1.

  T* advance_and_remove (int dont_remove);
  // Advance the iterator while removing the original item from the list.
  // Return a pointer points to the original (removed) item.  If
  // <dont_remove> equals 0, this function behaves like advance() but
  // return 0 (NULL) instead.

  int first (void);
  // Move to the first element in the list.  Returns 0 if the
  // list is empty, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  T *not_done (void) const ;
  // Check if we reach the end of the list.  Can also be used to get
  // the *current* element in the list.  Return the address of the
  // current item if there are still elements left , 0 if we run out
  // of element.

  T *do_advance (void);
  // Advance to the next element in the list.  Return the address of the
  // next element if there are more, 0 otherwise.

  T *current_;
  // Remember where we are.

  ACE_Double_Linked_List<T> &dllist_;
};

template <class T>
class ACE_Double_Linked_List
{
  // = TITLE
  //     A double-linked list implementation.
  //
  // = DESCRIPTION
  //     This implementation of an unbounded double-linked list uses a
  //     circular linked list with a dummy node.  It is pretty much
  //     like the ACE_Unbounded_Queue except that it allows removing
  //     of a specific element from a specific location.
public:
  friend class ACE_Double_Linked_List_Iterator<T>;

  // Trait definition.
  typedef ACE_Double_Linked_List_Iterator<T> ITERATOR;

  // = Initialization and termination methods.
  ACE_Double_Linked_List (ACE_Allocator *alloc = 0);
  // construction.  Use user specified allocation strategy
  // if specified.

  ACE_Double_Linked_List (ACE_Double_Linked_List<T> &);
  // Copy constructor.

  void operator= (ACE_Double_Linked_List<T> &);
  // Assignment operator.

  ~ACE_Double_Linked_List (void);
  // Destructor.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  // = Classic queue operations.

  T *insert_tail (T *new_item);
  // Adds <new_item> to the tail of the list. Returns the new item
  // that was inserted.

  T *insert_head (T *new_item);
  // Adds <new_item> to the head of the list.Returns the new item that
  // was inserted.

  T* delete_head (void);
  // Removes and returns the first <item> in the list.  Returns
  // internal node's address on success, 0 if the queue was empty.
  // This method will *not* free the internal node.

  T *delete_tail (void);
  // Removes and returns the last <item> in the list.  Returns
  // internal nodes's address on success, 0 if the queue was
  // empty. This method will *not* free the internal node.

  // = Additional utility methods.

  void reset (void);
  // Reset the <ACE_Double_Linked_List> to be empty.
  // Notice that since no one is interested in the items within,
  // This operation will delete all items.

  int get (T *&item, size_t index = 0);
  // Get the <index>th element in the set.  Returns -1 if the element
  // isn't in the range <0..size() - 1>, else 0.

  size_t size (void) const;
  // The number of items in the queue.

  void dump (void) const;
  // Dump the state of an object.

  int remove (T *n);
  // Use DNode address directly.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  void delete_nodes (void);
  // Delete all the nodes in the list.

  void copy_nodes (ACE_Double_Linked_List<T> &);
  // Copy nodes into this list.

  void init_head (void);
  // Setup header pointer.  Called after we create the head node in ctor.

  int insert_element (T *new_item,
                      int before = 0,
                      T *old_item = 0);
  // Insert a <new_element> into the list.  It will be added before
  // or after <old_item>.  Default is to insert the new item *after*
  // <head_>.  Return 0 if succeed, -1 if error occured.

  int remove_element (T *item);
  // Remove an <item> from the list.  Return 0 if succeed, -1 otherwise.
  // Notice that this function checks if item is <head_> and either its
  // <next_> or <prev_> is NULL.  The function resets item's <next_> and
  // <prev_> to 0 to prevent clobbering the double-linked list if a user
  // tries to remove the same node again.

  T *head_;
  // Head of the circular double-linked list.

  size_t size_;
  // Size of this list.

  ACE_Allocator *allocator_;
  // Allocation Strategy of the queue.
};


template <class T> class ACE_DLList;
template <class T> class ACE_DLList_Iterator;

typedef ACE_Double_Linked_List<ACE_DLList_Node>
        ACE_DLList_Base;
typedef ACE_Double_Linked_List_Iterator<ACE_DLList_Node>
        ACE_DLList_Iterator_Base;

template <class T>
class ACE_DLList : public ACE_DLList_Base
{
  // = TITLE
  //     A double-linked list container class.
  //
  // = DESCRIPTION
  //     This implementation uses ACE_Double_Linked_List to perform
  //     the logic behind this container class.  It delegates all of its
  //     calls to ACE_Double_Linked_List.

  friend class ACE_DLList_Node;
  friend class ACE_DLList_Iterator<T>;

public:
  void operator= (ACE_DLList<T> &l);
  // Delegates to ACE_Double_Linked_List.

  // = Classic queue operations.

  T *insert_tail (T *new_item);
  // Delegates to ACE_Double_Linked_List.

  T *insert_head (T *new_item);
  // Delegates to ACE_Double_Linked_List.

  T* delete_head (void);
  // Delegates to ACE_Double_Linked_List.

  T *delete_tail (void);
  // Delegates to ACE_Double_Linked_List.

  // = Additional utility methods.

  int get (T *&item, size_t index = 0);
  // Delegates to ACE_Double_Linked_List.

  void dump (void) const;
  // Delegates to ACE_Double_Linked_List.

  int remove (ACE_DLList_Node *n);
  // Delegates to ACE_Double_Linked_List.


  // = Initialization and termination methods.

  ACE_DLList (ACE_Allocator *alloc = 0);
  // Delegates to ACE_Double_Linked_List.

  ACE_DLList (ACE_DLList<T> &l);
  // Delegates to ACE_Double_Linked_List.

  ~ACE_DLList (void);
  // Deletes the list starting from the head.
};

template <class T>
class ACE_DLList_Iterator : public ACE_DLList_Iterator_Base
{
  // = TITLE
  //     A double-linked list container class iterator.
  //
  // = DESCRIPTION
  //     This implementation uses ACE_Double_Linked_List_Iterator to
  //     perform the logic behind this container class.  It delegates
  //     all of its calls to ACE_Double_Linked_List_Iterator.

  friend class ACE_DLList<T>;
  friend class ACE_DLList_Node;

public:

  // = Initialization method.
  ACE_DLList_Iterator (ACE_DLList<T> &l);

  // = Iteration methods.

  T *next (void) const;
  // Delegates to ACE_Double_Linked_List_Iterator.

  int remove (void);
  // Removes the current item (i.e., this->next()) from the list.

  void dump (void) const;
  // Delegates to ACE_Double_Linked_List_Iterator.

private:
  ACE_DLList<T> &list_;
};

template <class T>
class ACE_Unbounded_Set_Iterator
{
  // = TITLE
  //     Implement an iterator over an unbounded set.
public:
  // = Initialization method.
  ACE_Unbounded_Set_Iterator (ACE_Unbounded_Set<T> &s, int end = 0);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  int first (void);
  // Move to the first element in the set.  Returns 0 if the
  // set is empty, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void dump (void) const;
  // Dump the state of an object.

  // = STL styled iteration, compare, and reference functions.

  ACE_Unbounded_Set_Iterator<T> operator++ (int);
  // Postfix advance.

  ACE_Unbounded_Set_Iterator<T>& operator++ (void);
  // Prefix advance.

  T& operator* (void);
  // Returns a reference to the interal element <this> is pointing to.

  int operator== (const ACE_Unbounded_Set_Iterator<T> &) const;
  int operator!= (const ACE_Unbounded_Set_Iterator<T> &) const;
  // Check if two iterators point to the same position

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:

  ACE_Node<T> *current_;
  // Pointer to the current node in the iteration.

  ACE_Unbounded_Set<T> *set_;
  // Pointer to the set we're iterating over.
};

template <class T>
class ACE_Unbounded_Set
{
  // = TITLE
  //     Implement a simple unordered set of <T> of unbounded size.
  //
  // = DESCRIPTION
  //     This implementation of an unordered set uses a circular
  //     linked list with a dummy node.  This implementation does not
  //     allow duplicates, but it maintains FIFO ordering of insertions.
public:
  friend class ACE_Unbounded_Set_Iterator<T>;

  // Trait definition.
  typedef ACE_Unbounded_Set_Iterator<T> ITERATOR;
  typedef ACE_Unbounded_Set_Iterator<T> iterator;

  // = Initialization and termination methods.
  ACE_Unbounded_Set (ACE_Allocator *alloc = 0);
  // Constructor.  Use user specified allocation strategy
  // if specified.

  ACE_Unbounded_Set (const ACE_Unbounded_Set<T> &);
  // Copy constructor.

  void operator= (const ACE_Unbounded_Set<T> &);
  // Assignment operator.

  ~ACE_Unbounded_Set (void);
  // Destructor.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  // = Classic unordered set operations.

  int insert (const T &new_item);
  // Insert <new_item> into the set (doesn't allow duplicates).
  // Returns -1 if failures occur, 1 if item is already present, else
  // 0.

  int remove (const T &item);
  // Remove first occurrence of <item> from the set.  Returns 0 if
  // it removes the item, -1 if it can't find the item, and -1 if a
  // failure occurs.

  int find (const T &item) const;
  // Finds if <item> occurs in the set.  Returns 0 if find succeeds,
  // else -1.

  size_t size (void) const;
  // Size of the set.

  void dump (void) const;
  // Dump the state of an object.

  void reset (void);
  // Reset the <ACE_Unbounded_Set> to be empty.

  // = STL-styled unidirectional iterator factory.
  ACE_Unbounded_Set_Iterator<T> begin (void);
  ACE_Unbounded_Set_Iterator<T> end (void);

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  int insert_tail (const T &item);
  // Insert <item> at the tail of the set (doesn't check for
  // duplicates).

  void delete_nodes (void);
  // Delete all the nodes in the Set.

  void copy_nodes (const ACE_Unbounded_Set<T> &);
  // Copy nodes into this set.

  ACE_Node<T> *head_;
   // Head of the linked list of Nodes.

  size_t cur_size_;
  // Current size of the set.

  ACE_Allocator *allocator_;
  // Allocation strategy of the set.
};

// Forward declaration.
template <class T, size_t ACE_SIZE>
class ACE_Fixed_Set;

template <class T, size_t ACE_SIZE>
class ACE_Fixed_Set_Iterator
{
  // = TITLE
  //     Interates through an unordered set.
  //
  // = DESCRIPTION
  //     This implementation of an unordered set uses a fixed array.
  //     Allows deletions while iteration is occurring.
public:
  // = Initialization method.
  ACE_Fixed_Set_Iterator (ACE_Fixed_Set<T, ACE_SIZE> &s);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  int first (void);
  // Move to the first element in the set.  Returns 0 if the
  // set is empty, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  ACE_Fixed_Set<T, ACE_SIZE> &s_;
  // Set we are iterating over.

  ssize_t next_;
  // How far we've advanced over the set.
};

template <class T, size_t ACE_SIZE>
class ACE_Fixed_Set
{
  // = TITLE
  //     Implement a simple unordered set of <T> with maximum <ACE_SIZE>.
  //
  // = DESCRIPTION
  //     This implementation of an unordered set uses a fixed array.
  //     This implementation does not allow duplicates...
public:
  friend class ACE_Fixed_Set_Iterator<T, ACE_SIZE>;

  // Trait definition.
  typedef ACE_Fixed_Set_Iterator<T, ACE_SIZE> ITERATOR;

  // = Initialization and termination methods.
  ACE_Fixed_Set (void);
  // Constructor.

  ACE_Fixed_Set (const ACE_Fixed_Set<T, ACE_SIZE> &);
  // Copy constructor.

  void operator= (const ACE_Fixed_Set<T, ACE_SIZE> &);
  // Assignment operator.

  ~ACE_Fixed_Set (void);
  // Destructor.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  // = Classic unordered set operations.

  int insert (const T &new_item);
  // Insert <new_item> into the set (doesn't allow duplicates).
  // Returns -1 if failures occur, 1 if item is already present, else
  // 0.

  int remove (const T &item);
  // Remove first occurrence of <item> from the set.  Returns 0 if
  // it removes the item, -1 if it can't find the item, and -1 if a
  // failure occurs.

  int find (const T &item) const;
  // Finds if <item> occurs in the set.  Returns 0 if finds, else -1.

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
  } search_structure_[ACE_SIZE];
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
{
  // = TITLE
  //     Interates through an unordered set.
  //
  // = DESCRIPTION
  //     This implementation of an unordered set uses a Bounded array.
  //     Allows deletions while iteration is occurring.
public:
  // = Initialization method.
  ACE_Bounded_Set_Iterator (ACE_Bounded_Set<T> &s);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Set.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  int first (void);
  // Move to the first element in the set.  Returns 0 if the
  // set is empty, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

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
{
  // = TITLE
  //     Implement a simple unordered set of <T> with maximum
  //     set at creation time.
  //
  // = DESCRIPTION
  //     This implementation of an unordered set uses a Bounded array.
  //     This implementation does not allow duplicates...
public:
  friend class ACE_Bounded_Set_Iterator<T>;

  // Trait definition.
  typedef ACE_Bounded_Set_Iterator<T> ITERATOR;

  enum
  {
    DEFAULT_SIZE = 10
  };

  // = Initialization and termination methods.
  ACE_Bounded_Set (void);
  // Constructor.

  ACE_Bounded_Set (size_t size);
  // Constructor.

  ACE_Bounded_Set (const ACE_Bounded_Set<T> &);
  // Copy constructor.

  void operator= (const ACE_Bounded_Set<T> &);
  // Assignment operator.

  ~ACE_Bounded_Set (void);
  // Destructor

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  int is_full (void) const;
  // Returns 1 if the container is full, otherwise returns 0.

  // = Classic unordered set operations.

  int insert (const T &new_item);
  // Insert <new_item> into the set (doesn't allow duplicates).
  // Returns -1 if failures occur, 1 if item is already present, else
  // 0.

  int remove (const T &item);
  // Remove first occurrence of <item> from the set.  Returns 0 if it
  // removes the item, -1 if it can't find the item, and -1 if a
  // failure occurs.

  int find (const T &item) const;
  // Finds if <item> occurs in the set.  Returns 0 if finds, else -1.

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

template <class T>
class ACE_Ordered_MultiSet_Iterator
{
  // = TITLE
  //     Implement a bidirectional iterator over an ordered multiset.
  //     This class template requires that < operator semantics be
  //     defined for the parameterized type <T>, but does not impose
  //     any restriction on how that ordering operator is implemented.
public:
  friend class ACE_Ordered_MultiSet<T>;

  // = Initialization method.
  ACE_Ordered_MultiSet_Iterator (ACE_Ordered_MultiSet<T> &s);

  // = Iteration methods.

  int next (T *&next_item) const;
  // Pass back the <next_item> that hasn't been seen in the ordered multiset.
  // Returns 0 when all items have been seen, else 1.

  int first (void);
  // Repositions the iterator at the first item in the ordered multiset
  // Returns 0 if the list is empty else 1.

  int last (void);
  // Repositions the iterator at the last item in the ordered multiset
  // Returns 0 if the list is empty else 1.

  int advance (void);
  // Move forward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  int retreat (void);
  // Move backward by one element in the set.  Returns 0 when all the
  // items in the set have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:

  ACE_DNode<T> *current_;
  // Pointer to the current node in the iteration.

  ACE_Ordered_MultiSet<T> &set_;
  // Pointer to the set we're iterating over.
};

template <class T>
class ACE_Ordered_MultiSet
{
  // = TITLE
  //     Implement a simple ordered multiset of <T> of unbounded size.
  //     This class template requires that < operator semantics be
  //     defined for the parameterized type <T>, but does not impose
  //     any restriction on how that ordering operator is implemented.
  //
  // = DESCRIPTION
  //     This implementation of an unordered set uses a circular
  //     linked list with a dummy node.  This implementation does not
  //     allow duplicates, but it maintains FIFO ordering of
  //     insertions.
public:
  friend class ACE_Ordered_MultiSet_Iterator<T>;

  // Trait definition.
  typedef ACE_Ordered_MultiSet_Iterator<T> ITERATOR;

  // = Initialization and termination methods.
  ACE_Ordered_MultiSet (ACE_Allocator *alloc = 0);
  // Constructor.  Use user specified allocation strategy
  // if specified.

  ACE_Ordered_MultiSet (const ACE_Ordered_MultiSet<T> &);
  // Copy constructor.

  ~ACE_Ordered_MultiSet (void);
  // Destructor.

  void operator= (const ACE_Ordered_MultiSet<T> &);
  // Assignment operator.

  // = Check boundary conditions.

  int is_empty (void) const;
  // Returns 1 if the container is empty, otherwise returns 0.

  size_t size (void) const;
  // Size of the set.

  // = Classic unordered set operations.

  int insert (const T &new_item);
  // Insert <new_item> into the ordered multiset.
  // Returns -1 if failures occur, else 0.

  int insert (const T &new_item, ITERATOR &iter);
  // Insert <new_item> into the ordered multiset, starting its search at
  // the node pointed to by the iterator, and if insetion was successful,
  // updates the iterator to point to the newly inserted node.
  // Returns -1 if failures occur, else 0.

  int remove (const T &item);
  // Remove first occurrence of <item> from the set.  Returns 0 if
  // it removes the item, -1 if it can't find the item.

  int find (const T &item, ITERATOR &iter) const;
  // Finds first occurrance of <item> in the multiset, using the iterator's
  // current position as a hint to improve performance. If find succeeds,
  // it positions the iterator at that node and returns 0, or if it cannot
  // locate the node, it leaves the iterator alone and just returns -1.

  void reset (void);
  // Reset the <ACE_Ordered_MultiSet> to be empty.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:

  int insert_from (const T &item, ACE_DNode<T> *start_position,
                   ACE_DNode<T> **new_position);
  // Insert <item>, starting its search at the position given,
  // and if successful updates the passed pointer to point to
  // the newly inserted item's node.

  int locate (const T &item, ACE_DNode<T> *start_position,
              ACE_DNode<T> *&new_position) const;
  // looks for first occurance of <item> in the ordered set, using the
  // passed starting position as a hint: if there is such an instance, it
  // updates the new_position pointer to point to this node and returns 0;
  // if there is no such node, then if there is a node before where the
  // item would have been, it updates the new_position pointer to point
  // to this node and returns -1; if there is no such node, then if there
  // is a node after where the item would have been, it updates the
  // new_position pointer to point to this node (or 0 if there is no such
  // node) and returns 1;

  void delete_nodes (void);
  // Delete all the nodes in the Set.

  void copy_nodes (const ACE_Ordered_MultiSet<T> &);
  // Copy nodes into this set.

  ACE_DNode<T> *head_;
   // Head of the bilinked list of Nodes.

  ACE_DNode<T> *tail_;
   // Head of the bilinked list of Nodes.

  size_t cur_size_;
  // Current size of the set.

  ACE_Allocator *allocator_;
  // Allocation strategy of the set.
};

// ****************************************************************

// Forward declaration.
template <class T> class ACE_Array_Iterator;

template<class T>
class ACE_Array_Base
{
  // = TITLE
  //   Implement a simple dynamic array
  //
  // = DESCRIPTION
  //   This parametric class implements a simple dynamic array;
  //   resizing must be controlled by the user. No comparison or find
  //   operations are implemented.
  //
public:

  // Define a "trait"
  typedef T TYPE;
  typedef ACE_Array_Iterator<T> ITERATOR;

  // = Initialization and termination methods.

  ACE_Array_Base (size_t size = 0,
                  ACE_Allocator *alloc = 0);
  // Dynamically create an uninitialized array.

  ACE_Array_Base (size_t size,
                  const T &default_value,
                  ACE_Allocator *alloc = 0);
  // Dynamically initialize the entire array to the <default_value>.

  ACE_Array_Base (const ACE_Array_Base<T> &s);
  // The copy constructor performs initialization by making an exact
  // copy of the contents of parameter <s>, i.e., *this == s will
  // return true.

  void operator= (const ACE_Array_Base<T> &s);
  // Assignment operator performs an assignment by making an exact
  // copy of the contents of parameter <s>, i.e., *this == s will
  // return true.  Note that if the <max_size_> of <array_> is >= than
  // <s.max_size_> we can copy it without reallocating.  However, if
  // <max_size_> is < <s.max_size_> we must delete the <array_>,
  // reallocate a new <array_>, and then copy the contents of <s>.

  ~ACE_Array_Base (void);
  // Clean up the array (e.g., delete dynamically allocated memory).

  // = Set/get methods.

  T &operator [] (size_t index);
  // Set item in the array at location <index>.  Doesn't
  // perform range checking.

  const T &operator [] (size_t index) const;
  // Get item in the array at location <index>.  Doesn't
  // perform range checking.

  int set (const T &new_item, size_t index);
  // Set an item in the array at location <index>.  Returns
  // -1 if <index> is not in range, else returns 0.

  int get (T &item, size_t index) const;
  // Get an item in the array at location <index>.  Returns -1 if
  // <index> is not in range, else returns 0.  Note that this function
  // copies the item.  If you want to avoid the copy, you can use
  // the const operator [], but then you'll be responsible for range checking.

  size_t size (void) const;
  // Returns the <cur_size_> of the array.

  int size (size_t new_size);
  // Changes the size of the array to match <new_size>.
  // It copies the old contents into the new array.
  // Return -1 on failure.

  size_t max_size (void) const;
  // Returns the <max_size_> of the array.

  int max_size (size_t new_size);
  // Changes the size of the array to match <new_size>.
  // It copies the old contents into the new array.
  // Return -1 on failure.
  // It does not affect new_size

private:
  int in_range (size_t index) const;
  // Returns 1 if <index> is within range, i.e., 0 >= <index> <
  // <cur_size_>, else returns 0.

  size_t max_size_;
  // Maximum size of the array, i.e., the total number of <T> elements
  // in <array_>.

  size_t cur_size_;
  // Current size of the array.  This starts out being == to
  // <max_size_>.  However, if we are assigned a smaller array, then
  // <cur_size_> will become less than <max_size_>.  The purpose of
  // keeping track of both sizes is to avoid reallocating memory if we
  // don't have to.

  T *array_;
  // Pointer to the array's storage buffer.

  ACE_Allocator *allocator_;
  // Allocation strategy of the ACE_Array_Base.

  friend class ACE_Array_Iterator<T>;
};

// ****************************************************************

template <class T>
class ACE_Array : public ACE_Array_Base<T>
{
  // = TITLE
  //     Implement a dynamic array class.
  //
  // = DESCRIPTION
  //   This class extends ACE_Array_Base, it provides comparison
  //   operators.
public:
  // Define a "trait"
  typedef T TYPE;

  typedef ACE_Array_Iterator<T> ITERATOR;

  // = Exceptions.

  // = Initialization and termination methods.

  ACE_Array (size_t size = 0,
             ACE_Allocator* alloc = 0);
  // Dynamically create an uninitialized array.

  ACE_Array (size_t size,
             const T &default_value,
             ACE_Allocator* alloc = 0);
  // Dynamically initialize the entire array to the <default_value>.

  ACE_Array (const ACE_Array<T> &s);
  // The copy constructor performs initialization by making an exact
  // copy of the contents of parameter <s>, i.e., *this == s will
  // return true.

  void operator= (const ACE_Array<T> &s);
  // Assignment operator performs an assignment by making an exact
  // copy of the contents of parameter <s>, i.e., *this == s will
  // return true.  Note that if the <max_size_> of <array_> is >= than
  // <s.max_size_> we can copy it without reallocating.  However, if
  // <max_size_> is < <s.max_size_> we must delete the <array_>,
  // reallocate a new <array_>, and then copy the contents of <s>.

  // = Compare operators

  int operator== (const ACE_Array<T> &s) const;
  // Compare this array with <s> for equality.  Two arrays are equal
  // if their size()'s are equal and all the elements from 0 .. size()
  // are equal.

  int operator!= (const ACE_Array<T> &s) const;
  // Compare this array with <s> for inequality such that <*this> !=
  // <s> is always the complement of the boolean return value of
  // <*this> == <s>.
};

template <class T>
class ACE_Array_Iterator
{
  // = TITLE
  //     Implement an iterator over an ACE_Array.
  //
  // = DESCRIPTION
  //     This iterator is safe in the face of array element deletions.
  //     But it is NOT safe if the array is resized (via the ACE_Array
  //     assignment operator) during iteration.  That would be very
  //     odd, and dangerous.
public:
  // = Initialization method.
  ACE_Array_Iterator (ACE_Array_Base<T> &);

  // = Iteration methods.

  int next (T *&next_item);
  // Pass back the <next_item> that hasn't been seen in the Array.
  // Returns 0 when all items have been seen, else 1.

  int advance (void);
  // Move forward by one element in the Array.  Returns 0 when all the
  // items in the Array have been seen, else 1.

  int done (void) const;
  // Returns 1 when all items have been seen, else 0.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  u_int current_;
  // Pointer to the current item in the iteration.

  ACE_Array_Base<T> &array_;
  // Pointer to the Array we're iterating over.
};

#if defined (__ACE_INLINE__)
#include "ace/Containers_T.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Containers_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Containers_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_CONTAINERS_T_H */
