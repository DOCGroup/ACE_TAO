/* -*- C++ -*- */

// ============================================================================
//
// = LIBRARY
//    ace
// 
// = FILENAME
//    Free_List.h
//
// = AUTHOR
//    Darrell Brunsch (brunsch@cs.wustl.edu)
// 
// ============================================================================

#if !defined (ACE_FREE_LIST_H)
#define ACE_FREE_LIST_H

#include "ace/OS.h"
#include "ace/Synch_T.h"

template <class T>
class ACE_Free_List  
  // = TITLE
  //      Implements a free list.
  //
  // = DESCRIPTION
  //      This class maintains a free list of nodes of type T.
{
public:
  virtual ~ACE_Free_List (void);
  // Destructor - removes all the elements from the free_list.

  virtual void add (T *element) = 0;
  // Inserts an element onto the free list (if it isn't past the high
  // water mark).
  
  virtual T *remove (void) = 0;
  // Takes a element off the freelist and returns it.  It creates
  // <inc> new elements if the size is at or below the low water mark.
  
  virtual size_t size (void) = 0;
  // Returns the current size of the free list.

  virtual void resize (size_t newsize) = 0;
  // Resizes the free list to <newsize>.
};

template <class T, class LOCK>
class ACE_Locked_Free_List : public ACE_Free_List<T> 
  // = TITLE
  //      Implements a free list.
  //
  // = DESCRIPTION
  //      This class maintains a free list of nodes of type T.  It depends on
  //      the type T having a get_next () and set_next () method.  It maintains
  //      a mutex so the freelist can be used in a multithreaded program .
{
public:
  // = Initialization and termination.
  ACE_Locked_Free_List (int mode = ACE_FREE_LIST_WITH_POOL,
			size_t prealloc = ACE_DEFAULT_FREE_LIST_PREALLOC, 
                        size_t lwm = ACE_DEFAULT_FREE_LIST_LWM, 
                        size_t hwm = ACE_DEFAULT_FREE_LIST_HWM, 
                        size_t inc = ACE_DEFAULT_FREE_LIST_INC);
  // Constructor takes a <mode> (i.e., ACE_FREE_LIST_WITH_POOL or
  // ACE_PURE_FREE_LIST), a count of the number of nodes to
  // <prealloc>, a low and high water mark (<lwm> and <hwm>) that
  // indicate when to allocate more nodes, an increment value (<inc>)
  // that indicates how many nodes to allocate when the list must
  // grow.

  virtual ~ACE_Locked_Free_List (void);
  // Destructor - removes all the elements from the free_list.

  virtual void add (T *element);
  // Inserts an element onto the free list (if it isn't past the high
  // water mark).
  
  virtual T *remove (void);
  // Takes a element off the freelist and returns it.  It creates
  // <inc> new elements if the size is at or below the low water mark.
  
  virtual size_t size (void);
  // Returns the current size of the free list.

  virtual void resize (size_t newsize);
  // Resizes the free list to <newsize>.

protected:
  virtual void alloc (size_t n);
  // Allocates <n> extra nodes for the freelist.

  virtual void dealloc (size_t n);
  // Removes and frees <n> nodes from the freelist.

  int mode_;
  // Free list operation mode, either ACE_FREE_LIST_WITH_POOL or
  // ACE_PURE_FREE_LIST.

  T *free_list_;
  // Pointer to the first node in the freelist.

  size_t lwm_;
  // Low water mark.

  size_t hwm_;
  // High water mark.

  size_t inc_;
  // Increment value.

  size_t size_;
  // Keeps track of the size of the list.

  LOCK mutex_; 
  // Synchronization variable for <ACE_Timer_Queue>.

private:
  // = Don't allow these operations for now.
  ACE_UNIMPLEMENTED_FUNC (ACE_Locked_Free_List (const ACE_Locked_Free_List<T, LOCK> &))
  ACE_UNIMPLEMENTED_FUNC (void operator= (const ACE_Locked_Free_List<T, LOCK> &))
};

#if defined (__ACE_INLINE__)
#include "ace/Free_List.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ace/Free_List.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Free_List.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* ACE_FREE_LIST_H */
