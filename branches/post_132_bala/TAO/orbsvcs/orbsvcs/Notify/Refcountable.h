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

class TAO_NS_Destroy_Callback;

/**
 * @class TAO_NS_Refcountable
 *
 * @brief Thread-safe refounting, calls a Destroy_Callback when refcount falls to 0.
 *
 */
class TAO_Notify_Export TAO_NS_Refcountable
{
public:
  /// Constuctor
  TAO_NS_Refcountable (void);

  /// Destructor
  ~TAO_NS_Refcountable ();

  /// Set the destroy callback.
  void destroy_callback (TAO_NS_Destroy_Callback* destroy_callback);

  /// This method sigantures deliberately match the RefCounting methods required for ESF Proxy
  CORBA::ULong _incr_refcnt (void);
  CORBA::ULong _decr_refcnt (void);

protected:
  /// The reference count.
  CORBA::ULong refcount_;

  /// The mutex to serialize access to state variables.
  TAO_SYNCH_MUTEX lock_;

  /// The callback when refcount falls to 0.
  TAO_NS_Destroy_Callback* destroy_callback_;
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
