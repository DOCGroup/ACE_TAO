/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Event Service Framework
//
// = FILENAME
//   ESF_Copy_On_Write
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = CREDITS
//   http://www.cs.wustl.edu/~coryan/ESF/index.html
//
// ============================================================================

#ifndef TAO_ESF_COPY_ON_WRITE_H
#define TAO_ESF_COPY_ON_WRITE_H

#include "ESF_Proxy_Collection.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template<class COLLECTION, class ITERATOR>
class TAO_ESF_Copy_On_Write_Collection
{
public:
  TAO_ESF_Copy_On_Write_Collection (void);

  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);
  // Increment and decrement the reference count

  COLLECTION collection;
  // The actual collection

private:
  CORBA::ULong refcount_;
  // The reference count
};

// ****************************************************************

template<class COLLECTION, class ITERATOR, class ACE_LOCK>
class TAO_ESF_Copy_On_Write_Read_Guard
{
  // = TITLE
  //   TAO_ESF_Copy_On_Guard
  //
  // = DESCRIPTION
  //   This helper class atomically increments the reference count of
  //   a TAO_ESF_Copy_On_Write_Collection and reads the current
  //   collection in the Copy_On_Write class.
  //
  // = TODO
  //
public:
  typedef TAO_ESF_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;
  TAO_ESF_Copy_On_Write_Read_Guard (ACE_LOCK &mutex,
                                   Collection*& collection);
  // Constructor

  ~TAO_ESF_Copy_On_Write_Read_Guard (void);
  // Destructor

  Collection *collection;

private:
  ACE_LOCK &mutex;
};

// ****************************************************************

template<class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
class TAO_ESF_Copy_On_Write_Write_Guard
{
  // = TITLE
  //   TAO_ESF_Copy_On_Write_Guard
  //
  // = DESCRIPTION
  //   This helper class atomically increments the reference count of
  //   a TAO_ESF_Copy_On_Write_Collection and reads the current
  //   collection in the Copy_On_Write class.
  //
  // = TODO
  //
public:
  typedef TAO_ESF_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;
  TAO_ESF_Copy_On_Write_Write_Guard (ACE_SYNCH_MUTEX_T &mutex,
                                    ACE_SYNCH_CONDITION_T &cond,
                                    int &writing_flag,
                                    Collection*& collection);
  // Constructor

  ~TAO_ESF_Copy_On_Write_Write_Guard (void);
  // Destructor

  Collection *copy;

private:
  ACE_SYNCH_MUTEX_T &mutex;
  ACE_SYNCH_CONDITION_T &cond;
  int &writing_flag;
  Collection *&collection;
};

// ****************************************************************

template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
class TAO_ESF_Copy_On_Write : public TAO_ESF_Proxy_Collection<PROXY>
{
  // = TITLE
  //   TAO_ESF_Copy_On_Write
  //
  // = DESCRIPTION
  //   Implement the Copy_On_Write protocol
  //   The class is parametric on the kind of collection and locking
  //   mechanism used.
  //
  // = TODO
  //
public:
  typedef TAO_ESF_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR,ACE_SYNCH_MUTEX_T> Read_Guard;
  typedef TAO_ESF_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR,ACE_SYNCH_USE> Write_Guard;
  TAO_ESF_Copy_On_Write (void);
  // Constructor

  ~TAO_ESF_Copy_On_Write (void);
  // Destructor

  // = The TAO_ESF_Proxy methods
  virtual void for_each (TAO_ESF_Worker<PROXY> *worker,
                         CORBA::Environment &ACE_TRY_ENV);
  virtual void connected (PROXY *proxy,
                          CORBA::Environment &ACE_TRY_ENV);
  virtual void reconnected (PROXY *proxy,
                            CORBA::Environment &ACE_TRY_ENV);
  virtual void disconnected (PROXY *proxy,
                             CORBA::Environment &ACE_TRY_ENV);
  virtual void shutdown (CORBA::Environment &ACE_TRY_ENV);

private:
  typedef TAO_ESF_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;

  ACE_SYNCH_MUTEX_T mutex_;
  // A mutex to serialize access to the collection pointer.

  int writing_;
  // If non-zero then a thread is changing the collection.  Many
  // threads can use the collection simulatenously, but only one
  // change it.

  ACE_SYNCH_CONDITION_T cond_;
  // A condition variable to wait to synchronize multiple writers.

  Collection *collection_;
  // The collection, with reference counting added
};

// ****************************************************************

#if defined (__ACE_INLINE__)
#include "ESF_Copy_On_Write.i"
#endif /* __ACE_INLINE__ */

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "ESF_Copy_On_Write.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("ESF_Copy_On_Write.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_ESF_COPY_ON_WRITE_H */
