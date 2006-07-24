// -*- C++ -*-

//=============================================================================
/**
 *  @file    Null_RefCount_Policy.h
 *
 *  $Id$
 *
 *  Header file for TAO's reference count policy (unrelated to CORBA
 *  policies).
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_NULL_REFCOUNT_POLICY_H
#define TAO_NULL_REFCOUNT_POLICY_H

#include /**/ "ace/pre.h"

#include "tao/AnyTypeCode/TAO_AnyTypeCode_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{

  /**
   * @class Null_RefCount_Policy
   *
   * @brief No-op reference counting policy.
   *
   * This class is intended to be used as a "policy" argument to a
   * host class template that implements no-op reference counting.
   * That class would then inherit privately from it like so:
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
   *   typedef MyHostClass<TAO::Null_RefCount_Policy> MyNonRefCountedClass;
   *   MyNonRefCountedClass m;
   *   ...
   * @endcode
   *
   * @note In order to incur no size overhead on the host class due to
   *       virtual tables, no base class defining an interface is
   *       defined.  This allows C++ compilers to apply the Empty Base
   *       Class Optimization.
   */
  class TAO_AnyTypeCode_Export Null_RefCount_Policy
  {
  public:

    /// No-op reference increment.
    void add_ref (void)  { }

    /// No-op reference decrement.
    void remove_ref (void) { }

  };

}  // End namespace TAO

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_NULL_REFCOUNT_POLICY_H */
