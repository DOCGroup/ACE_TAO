// -*- C++ -*-

//=============================================================================
/**
 *  @file    True_RefCount_Policy.h
 *
 *  $Id$
 *
 *  Header file for TAO's true reference count policy (unrelated to
 *  CORBA policies).
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_TRUE_REFCOUNT_POLICY_H
#define TAO_TRUE_REFCOUNT_POLICY_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"

#include "ace/Thread_Mutex.h"
#include "ace/Atomic_Op.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class True_RefCount_Policy
   *
   * @brief True reference counting policy.
   *
   * This class is intended to be used as a "policy" argument to a
   * host class template that requires true/functional reference
   * counting.  That class would then inherit privately from it like
   * so:
   *
   * @code
   *   template <class RefCountPolicy>
   *   class MyHostClass : private RefCountPolicy
   *   {
   *   public:
   *     void my_add_ref (void)    { this->RefCountPolicy::add_ref (); }
   *     void my_remove_ref (void) { this->RefCountPolicy::remove_ref (); }
   *   };
   * @endcode
   *
   * and use it like so:
   *
   * @code
   *   typedef MyHostClass<TAO::True_RefCount_Policy> MyRefCountedClass;
   *   MyRefCountedClass * p = new MyRefCountedClass;
   *   ...
   *   p->my_remove_ref ();
   * @endcode
   *
   * @note Ideally, the host class should declare a protected
   *       destructor to enforce proper memory management through the
   *       reference counting mechanism, i.e. to prevent clients from
   *       calling @c operator @c delete() directly on the host class
   *       object.
   */
  class TAO_AnyTypeCode_Export True_RefCount_Policy
  {
  public:

    /// Increase the reference count on this object.
    void add_ref (void);

    /// Decrease the reference count on this object.
    /**
     * Decrease the reference count on this object.  Once the
     * reference count drops to zero, call @c operator @c delete()
     * on this object.
     */
    void remove_ref (void);

  protected:

    /// Constructor.
    /**
     * @note This constructor is protected since it not meant to be
     *       instantiated/used as a standalone object.
     */
    True_RefCount_Policy (void);

    /// Destructor.
    /**
     * @note The destructor must be virtual to ensure that subclass
     *       destructors are called when the reference count drops to
     *       zero, i.e. when @c remove_ref() calls @c operator
     *       @c delete @c this.
     */
    virtual ~True_RefCount_Policy (void);

  private:
    /// Reference counter.
    ACE_Atomic_Op<TAO_SYNCH_MUTEX, unsigned long> refcount_;
  };

}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#ifdef __ACE_INLINE__
# include "tao/AnyTypeCode/True_RefCount_Policy.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_TRUE_REFCOUNT_POLICY_H */
