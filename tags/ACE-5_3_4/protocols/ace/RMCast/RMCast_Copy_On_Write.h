/* -*- C++ -*- */
// $Id$

#ifndef ACE_RMCAST_COPY_ON_WRITE_H
#define ACE_RMCAST_COPY_ON_WRITE_H
#include /**/ "ace/pre.h"

#include "RMCast_Worker.h"
#include "ace/Synch.h"

//! A wrapper to implement reference counted collections
template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Collection
{
public:
  //! Constructor
  ACE_RMCast_Copy_On_Write_Collection (void);

  //! Increment the reference count
  void _incr_refcnt (void);

  //! Decrement the reference count
  void _decr_refcnt (void);

  //! The actual collection
  COLLECTION collection;

private:
  //! The reference count
  ACE_UINT32 refcount_;
};

// ****************************************************************

template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Read_Guard;

template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Write_Guard;

//! Base class for the Copy_On_Write collection, used to simplify the
//! declaration of the Write_Guard
template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Container
{
public:
  //! Constructor
  ACE_RMCast_Copy_On_Write_Container (void);

  //! Let the Write_Guard access the internal fields.
  friend class ACE_RMCast_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR>;

  //! Let the Read_Guard access the internal fields.
  friend class ACE_RMCast_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR>;

  //! A shorter name for the actual collection type
  typedef ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;

protected:
  //! Number of pending writes
  int pending_writes_;

  //! If non-zero then a thread is changing the collection.
  /*!
   * Many threads can use the collection simulatenously, but only one
   * change it.
   */
  int writing_;

  //! A mutex to serialize access to the collection pointer.
  ACE_SYNCH_MUTEX mutex_;

  //! A condition variable to wait to synchronize multiple writers.
  ACE_SYNCH_CONDITION cond_;

  //! The collection, with reference counting added
  Collection *collection_;
};

// ****************************************************************

//! Implement a read guard for a reference counted collection
template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Read_Guard
{
public:
  typedef ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;
  typedef ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR> Container;

  //! Constructor
  ACE_RMCast_Copy_On_Write_Read_Guard (Container &container);

  //! Destructor
  ~ACE_RMCast_Copy_On_Write_Read_Guard (void);

  //! A reference to the collection
  Collection *collection;

private:
  //! Synchronization
  ACE_SYNCH_MUTEX &mutex_;
};

// ****************************************************************

//! Implement the write guard for a reference counted collecion
/*!
 * This helper class atomically increments the reference count of a
 * ACE_RMCast_Copy_On_Write_Collection and reads the current
 * collection in the Copy_On_Write class.
 */
template<class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write_Write_Guard
{
public:
  typedef ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;
  typedef ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR> Container;

  //! Constructor
  ACE_RMCast_Copy_On_Write_Write_Guard (Container &container);

  //! Destructor
  ~ACE_RMCast_Copy_On_Write_Write_Guard (void);

  //! The collection
  Collection *copy;

private:
  //! Keep a reference to the mutex
  ACE_SYNCH_MUTEX &mutex;

  //! Keep a reference to the condition variable
  ACE_SYNCH_CONDITION &cond;

  //! Use a reference to update the pending writes count
  int &pending_writes;

  //! Use a reference to update the writing flag
  int &writing_flag;

  //! Use this reference to update the collection once the
  //! modifications are finished.
  Collection *&collection;
};


// ****************************************************************

//! Implement a copy on write wrapper for a map-like collection
/*
 *
 * <B>WARNING: </B>  This class may be moved away in the future, I'm
 * investigating how it could be converted into a reusable component
 * in ACE.   I won't make promises on when will that happen, but I
 * won't promise that it will stay here either.
 *
 */
template<class KEY, class ITEM, class COLLECTION, class ITERATOR>
class ACE_RMCast_Copy_On_Write : public ACE_RMCast_Copy_On_Write_Container<COLLECTION,ITERATOR>
{
public:
  //! The Read_Guard trait
  typedef ACE_RMCast_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR> Read_Guard;

  //! The Write_Guard trait
  typedef ACE_RMCast_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR> Write_Guard;

  //! The underlying collection type
  typedef ACE_RMCast_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;

  //! Constructor
  ACE_RMCast_Copy_On_Write (void);

  //! Destructor
  ~ACE_RMCast_Copy_On_Write (void);

  //! Iterate over all the elements invoking \param worker on each one.
  int for_each (ACE_RMCast_Worker<KEY,ITEM> *worker);

  //! Get the first key
  KEY first_key (void);

  /// Return non-zero if the collection is empty
  int empty (void);

  //! Add a new element
  int bind (KEY const & key, ITEM const & item);

  //! Remove an element
  int unbind (KEY const & key);

  //! Bind assuming the Write_Guard is held
  int bind_i (Write_Guard &guard, KEY const & key, ITEM const & item);

  //! Unbind assuming the Write_Guard is held
  int unbind_i (Write_Guard &guard, KEY const & key);
};

#if defined (__ACE_INLINE__)
#include "RMCast_Copy_On_Write.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "RMCast_Copy_On_Write.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("RMCast_Copy_On_Write.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"
#endif /* ACE_RMCAST_COPY_ON_WRITE_H */
