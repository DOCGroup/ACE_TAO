/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Stack.h
//
// = AUTHOR
//    Doug Schmidt 
// 
// ============================================================================

#if !defined (ACE_STACK_H)
#define ACE_STACK_H

#include "ace/ACE.h"

template <class T>
class ACE_Bounded_Stack 
  // = TITLE
  //     Implement a generic LIFO abstract data type.
  //     
  // = DESCRIPTION
  //     This implementation of a Stack uses a bounded array
  //     that is allocated dynamically.
{
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

  void push (const T &new_item);
    // Place a new item on top of the stack. Does not check if the
    // stack is full.  

  void pop (T &item);
    // Remove and return the top stack item. Does not check if stack
    // is full. 

  void top (T &item) const; 
    // Return top stack item without removing it. Does not check if
    // stack is empty. 

  // = Check boundary conditions for Stack operations. 

  int is_empty (void) const;
    // Returns 1 if the stack is empty, otherwise returns 0. 

  int is_full (void) const;
    // Returns 1 if the stack is full, otherwise returns 0. 

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

template <class T, size_t SIZE>
class ACE_Fixed_Stack 
  // = TITLE
  //     Implement a generic LIFO abstract data type.
  //     
  // = DESCRIPTION
  //     This implementation of a Stack uses a fixed array
  //     with the size fixed at instantiation time.
{
public:
  // = Initialization, assignemnt, and termination methods.
  ACE_Fixed_Stack (void);
    // Initialize a new stack so that it is empty. 

  ACE_Fixed_Stack (const ACE_Fixed_Stack<T, SIZE> &s);
    // The copy constructor (performs initialization). 

  void operator= (const ACE_Fixed_Stack<T, SIZE> &s);
    // Assignment operator (performs assignment). 

 ~ACE_Fixed_Stack (void); 
    // Perform actions needed when stack goes out of scope. 

  // = Classic Stack operations.

  void push (const T &new_item);
    // Place a new item on top of the stack. Does not check if the
    // stack is full.  

  void pop (T &item);
    // Remove and return the top stack item. Does not check if stack
    // is full. 

  void top (T &item) const; 
    // Return top stack item without removing it. Does not check if
    // stack is empty. 

  // = Check boundary conditions for Stack operations. 

  int is_empty (void) const;
    // Returns 1 if the stack is empty, otherwise returns 0. 

  int is_full (void) const;
    // Returns 1 if the stack is full, otherwise returns 0. 

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  size_t size_;
  // Size of the dynamically allocated data.

  size_t top_; 
  // Keeps track of the current top of stack. 

  T stack_[SIZE];
  // Holds the stack's contents. 
};

//----------------------------------------

// Forward declaration (use the "Cheshire Cat" approach to information
// hiding). 
template <class T>
class ACE_Stack_Node;

template <class T>
class ACE_Unbounded_Stack 
  // = TITLE
  //     Implement a generic LIFO abstract data type.
  //     
  // = DESCRIPTION
  //     This implementation of an unbounded Stack uses a linked list.
{
public:
  // = Initialization, assignemnt, and termination methods.
  ACE_Unbounded_Stack (void);
    // Initialize a new stack so that it is empty. 

  ACE_Unbounded_Stack (const ACE_Unbounded_Stack<T> &s);
    // The copy constructor (performs initialization). 

  void operator= (const ACE_Unbounded_Stack<T> &s);
    // Assignment operator (performs assignment). 

 ~ACE_Unbounded_Stack (void); 
    // Perform actions needed when stack goes out of scope. 

  // = Classic Stack operations.

  void push (const T &new_item);
    // Place a new item on top of the stack. Does not check if the
    // stack is full.  

  void pop (T &item);
    // Remove and return the top stack item. Does not check if stack
    // is full. 

  void top (T &item) const;
    // Return top stack item without removing it. Does not check if
    // stack is empty. 

  // = Check boundary conditions for Stack operations. 

  int is_empty (void) const;
    // Returns 1 if the stack is empty, otherwise returns 0. 

  int is_full (void) const;
    // Returns 1 if the stack is full, otherwise returns 0. 

   static void delete_free_list (void);
   // Returns all dynamic memory on the free list to the free store.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

private:
  void delete_all_nodes (void);
  // Delete all the nodes in the stack.

  void copy_all_nodes (const ACE_Unbounded_Stack<T> &s);
  // Copy all nodes from <s> to <this>.

  ACE_Stack_Node<T> *head_;
   // Head of the linked list of Nodes.

  ACE_Stack_Node<T> *last_resort_;
  // Use this node when all memory is exhausted... 
};

// Forward declaration (use the "Cheshire Cat" approach to information
// hiding). 
template <class T>
class ACE_Queue_Node;

template <class TYPE>
class ACE_Unbounded_Queue
  // = TITLE
  //     A Queue of "infinite" length.

  // = DESCRIPTION
  //     Implemented using dynamic memory...
{
public:
  ACE_Unbounded_Queue (void);
  // construction.

  ~ACE_Unbounded_Queue (void);
  // construction.

  int enqueue (const TYPE &new_item);
  // Addes <new_item> to the queue.  Returns 0 on success, -1 on failure.

  int dequeue (TYPE &item);
  // Removes and returns the first <item> on the queue.  Returns 0 on
  // success, -1 if nothing was found.

  int peek (TYPE &item) const;
  // Returns the first <item> on the queue without removing it.
  // Returns 0 on success, -1 if nothing was found.

  TYPE *peek (const u_int index = 0) const;
  // Returns a pointer to the item indicated by index without removing it,
  // or 0 if nothing was found because index is greater than the number of
  // enqueued items, less one.  An index of 0 indicates the first item, and
  // so on.

  size_t size (void) const;
  // The number of items in the queue.

  void dump (void) const;
  // Dump the state of an object.

  ACE_ALLOC_HOOK_DECLARE;
  // Declare the dynamic allocation hooks.

protected:
  ACE_Queue_Node<TYPE> *head_;
  // Head of the Queue.

  ACE_Queue_Node<TYPE> *tail_;
  // Tail of the Queue.

  size_t cur_size_;
  // Current size of the queue.
};

#if defined (__ACE_INLINE__)
#include "ace/Stack.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Stack.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Stack.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_STACK_H */
