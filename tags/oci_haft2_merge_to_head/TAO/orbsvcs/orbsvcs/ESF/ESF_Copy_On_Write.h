/* -*- C++ -*- */
/**
 *  @file   ESF_Copy_On_Write.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

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

  /// Increment the reference count
  CORBA::ULong _incr_refcnt (void);

  /// Decrement the reference count
  CORBA::ULong _decr_refcnt (void);

  /// The actual collection
  COLLECTION collection;

private:
  /// The reference count
  CORBA::ULong refcount_;
};

// ****************************************************************

/**
 * @class TAO_ESF_Copy_On_Write_Read_Guard
 *
 * @brief TAO_ESF_Copy_On_Write_Read_Guard
 *
 * This helper class atomically increments the reference count of
 * a TAO_ESF_Copy_On_Write_Collection and reads the current
 * collection in the Copy_On_Write class.
 */
template<class COLLECTION, class ITERATOR, class ACE_LOCK>
class TAO_ESF_Copy_On_Write_Read_Guard
{
public:
  /// Constructor
  typedef TAO_ESF_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;
  TAO_ESF_Copy_On_Write_Read_Guard (ACE_LOCK &mutex,
                                    Collection *&collection);

  /// Destructor
  ~TAO_ESF_Copy_On_Write_Read_Guard (void);

  Collection *collection;

private:
  ACE_LOCK &mutex;
};

// ****************************************************************

/**
 * @class TAO_ESF_Copy_On_Write_Write_Guard
 *
 * @brief TAO_ESF_Copy_On_Write_Write_Guard
 *
 * This helper class atomically increments the reference count of
 * a TAO_ESF_Copy_On_Write_Collection and reads the current
 * collection in the Copy_On_Write class.
 */
template<class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
class TAO_ESF_Copy_On_Write_Write_Guard
{
public:
  /// Constructor
  typedef TAO_ESF_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;
  TAO_ESF_Copy_On_Write_Write_Guard (ACE_SYNCH_MUTEX_T &mutex,
                                     ACE_SYNCH_CONDITION_T &cond,
                                     int &pending_writes,
                                     int &writing_flag,
                                     Collection*& collection);

  /// Destructor
  ~TAO_ESF_Copy_On_Write_Write_Guard (void);

  Collection *copy;

private:
  ACE_SYNCH_MUTEX_T &mutex;
  ACE_SYNCH_CONDITION_T &cond;
  int &pending_writes;
  int &writing_flag;
  Collection *&collection;
};

// ****************************************************************

/**
 * @class TAO_ESF_Copy_On_Write
 *
 * @brief TAO_ESF_Copy_On_Write
 *
 * Implement the Copy_On_Write protocol
 * The class is parametric on the kind of collection and locking
 * mechanism used.
 */
template<class PROXY, class COLLECTION, class ITERATOR, ACE_SYNCH_DECL>
class TAO_ESF_Copy_On_Write : public TAO_ESF_Proxy_Collection<PROXY>
{
public:
  /// Constructor
  typedef TAO_ESF_Copy_On_Write_Read_Guard<COLLECTION,ITERATOR,ACE_SYNCH_MUTEX_T> Read_Guard;
  typedef TAO_ESF_Copy_On_Write_Write_Guard<COLLECTION,ITERATOR,ACE_SYNCH_USE> Write_Guard;
  TAO_ESF_Copy_On_Write (void);

  /// Destructor
  ~TAO_ESF_Copy_On_Write (void);

  // = The TAO_ESF_Proxy methods
  virtual void for_each (TAO_ESF_Worker<PROXY> *worker
                         ACE_ENV_ARG_DECL);
  virtual void connected (PROXY *proxy
                          ACE_ENV_ARG_DECL);
  virtual void reconnected (PROXY *proxy
                            ACE_ENV_ARG_DECL);
  virtual void disconnected (PROXY *proxy
                             ACE_ENV_ARG_DECL);
  virtual void shutdown (ACE_ENV_SINGLE_ARG_DECL);

private:
  typedef TAO_ESF_Copy_On_Write_Collection<COLLECTION,ITERATOR> Collection;

  /// A mutex to serialize access to the collection pointer.
  ACE_SYNCH_MUTEX_T mutex_;

  /// Number of pending writes
  int pending_writes_;

  /**
   * If non-zero then a thread is changing the collection.  Many
   * threads can use the collection simulatenously, but only one
   * change it.
   */
  int writing_;

  /// A condition variable to wait to synchronize multiple writers.
  ACE_SYNCH_CONDITION_T cond_;

  /// The collection, with reference counting added
  Collection *collection_;
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
