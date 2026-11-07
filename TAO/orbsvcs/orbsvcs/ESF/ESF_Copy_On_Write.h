// -*- C++ -*-

/**
 *  @file   ESF_Copy_On_Write.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 *  http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_ESF_COPY_ON_WRITE_H
#define TAO_ESF_COPY_ON_WRITE_H

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"

#include "tao/Basic_Types.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template<class COLLECTION, class ITERATOR>
class TAO_ESF_Copy_On_Write_Collection
{
public:
  TAO_ESF_Copy_On_Write_Collection ();

  /// Increment the reference count
  CORBA::ULong _incr_refcnt ();

  /// Decrement the reference count
  CORBA::ULong _decr_refcnt ();

  /// The actual collection
  COLLECTION collection;

  TAO_ESF_Copy_On_Write_Collection (TAO_ESF_Copy_On_Write_Collection const &) = delete;
  TAO_ESF_Copy_On_Write_Collection (TAO_ESF_Copy_On_Write_Collection &&) = delete;
  TAO_ESF_Copy_On_Write_Collection &operator= (TAO_ESF_Copy_On_Write_Collection const &) = delete;
  TAO_ESF_Copy_On_Write_Collection &operator= (TAO_ESF_Copy_On_Write_Collection &&) = delete;

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
  using Collection = TAO_ESF_Copy_On_Write_Collection<COLLECTION, ITERATOR>;
  TAO_ESF_Copy_On_Write_Read_Guard (ACE_LOCK &mutex,
                                    Collection *&collection);

  /// Destructor
  ~TAO_ESF_Copy_On_Write_Read_Guard ();

  TAO_ESF_Copy_On_Write_Read_Guard (TAO_ESF_Copy_On_Write_Read_Guard const &) = delete;
  TAO_ESF_Copy_On_Write_Read_Guard (TAO_ESF_Copy_On_Write_Read_Guard &&) = delete;
  TAO_ESF_Copy_On_Write_Read_Guard &operator= (TAO_ESF_Copy_On_Write_Read_Guard const &) = delete;
  TAO_ESF_Copy_On_Write_Read_Guard &operator= (TAO_ESF_Copy_On_Write_Read_Guard &&) = delete;

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
  using Collection = TAO_ESF_Copy_On_Write_Collection<COLLECTION, ITERATOR>;
  TAO_ESF_Copy_On_Write_Write_Guard (ACE_SYNCH_MUTEX_T &mutex,
                                     ACE_SYNCH_CONDITION_T &cond,
                                     int &pending_writes,
                                     bool &writing_flag,
                                     Collection*& collection);

  /// Destructor
  ~TAO_ESF_Copy_On_Write_Write_Guard ();

  TAO_ESF_Copy_On_Write_Write_Guard (TAO_ESF_Copy_On_Write_Write_Guard const &) = delete;
  TAO_ESF_Copy_On_Write_Write_Guard (TAO_ESF_Copy_On_Write_Write_Guard &&) = delete;
  TAO_ESF_Copy_On_Write_Write_Guard &operator= (TAO_ESF_Copy_On_Write_Write_Guard const &) = delete;
  TAO_ESF_Copy_On_Write_Write_Guard &operator= (TAO_ESF_Copy_On_Write_Write_Guard &&) = delete;

  Collection *copy;

private:
  ACE_SYNCH_MUTEX_T &mutex;
  ACE_SYNCH_CONDITION_T &cond;
  int &pending_writes;
  bool &writing_flag;
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
  using Read_Guard = TAO_ESF_Copy_On_Write_Read_Guard<COLLECTION, ITERATOR, ACE_SYNCH_MUTEX_T>;
  using Write_Guard = TAO_ESF_Copy_On_Write_Write_Guard<COLLECTION, ITERATOR, ACE_SYNCH_USE>;
  TAO_ESF_Copy_On_Write ();

  /// Destructor
  ~TAO_ESF_Copy_On_Write () override;

  TAO_ESF_Copy_On_Write (TAO_ESF_Copy_On_Write const &) = delete;
  TAO_ESF_Copy_On_Write (TAO_ESF_Copy_On_Write &&) = delete;
  TAO_ESF_Copy_On_Write &operator= (TAO_ESF_Copy_On_Write const &) = delete;
  TAO_ESF_Copy_On_Write &operator= (TAO_ESF_Copy_On_Write &&) = delete;

  // = The TAO_ESF_Proxy methods
  void for_each (TAO_ESF_Worker<PROXY> *worker) override;
  void connected (PROXY *proxy) override;
  void reconnected (PROXY *proxy) override;
  void disconnected (PROXY *proxy) override;
  void shutdown () override;

private:
  using Collection = TAO_ESF_Copy_On_Write_Collection<COLLECTION, ITERATOR>;

  /// A mutex to serialize access to the collection pointer.
  ACE_SYNCH_MUTEX_T mutex_;

  /// Number of pending writes
  int pending_writes_ {};

  /**
   * If true then a thread is changing the collection.  Many
   * threads can use the collection simulatenously, but only one
   * change it.
   */
  bool writing_ {};

  /// A condition variable to wait to synchronize multiple writers.
  ACE_SYNCH_CONDITION_T cond_;

  /// The collection, with reference counting added
  Collection *collection_ {};
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "orbsvcs/ESF/ESF_Copy_On_Write.inl"
#endif /* __ACE_INLINE__ */

#include "orbsvcs/ESF/ESF_Copy_On_Write.cpp"

#endif /* TAO_ESF_COPY_ON_WRITE_H */
