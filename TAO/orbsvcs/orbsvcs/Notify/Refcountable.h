/* -*- C++ -*- */
/**
 *  @file Refcountable.h
 *
 *  $Id$
 *
 *  @author Pradeep Gore <pradeep@oomworks.com>
 *
 *
 */

#ifndef TAO_NS_REFCOUNTABLE_H
#define TAO_NS_REFCOUNTABLE_H
#include "ace/pre.h"

#include "notify_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Synch_T.h"
#include "tao/orbconf.h"
#include "tao/corbafwd.h"

/**
 * @class TAO_NS_Refcountable
 *
 * @brief Thread-safe refounting, calls the <release> method when refcount falls to 0.
 *
 */
class TAO_Notify_Export TAO_NS_Refcountable
{
public:
  /// Constuctor
  TAO_NS_Refcountable (void);

  /// Destructor
  virtual ~TAO_NS_Refcountable ();

  /// This method sigantures deliberately match the RefCounting methods required for ESF Proxy
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

  /// The release method is called when the refcount reaches 0.
  virtual void release (void) = 0;

protected:
  /// The reference count.
  CORBA::ULong refcount_;

  /// The mutex to serialize access to state variables.
  TAO_SYNCH_MUTEX lock_;
};

/***********************************************************************/

/**
 * @class TAO_NS_Refcountable_Guard
 *
 * @brief Ref. Count Guard
 *        Increments the reference count in the constructor, the count is decremented when the guard's is destructor.
 *
 */
class TAO_Notify_Export TAO_NS_Refcountable_Guard
{
public:
  TAO_NS_Refcountable_Guard (TAO_NS_Refcountable& refcountable);

  ~TAO_NS_Refcountable_Guard ();

protected:
  TAO_NS_Refcountable& refcountable_;
};

#if defined (__ACE_INLINE__)
#include "Refcountable.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_NS_REFCOUNTABLE_H */
