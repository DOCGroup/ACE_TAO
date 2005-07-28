// -*- C++ -*-

//=============================================================================
/**
 *  @file    RefCount_Policy_Traits.h
 *
 *  $Id$
 *
 *  Header file for TAO's reference count policy (unrelated to CORBA
 *  policies) traits.
 *
 *  @author Ossama Othman <ossama@dre.vanderbilt.edu>
 */
//=============================================================================

#ifndef TAO_REFCOUNT_POLICY_TRAITS_H
#define TAO_REFCOUNT_POLICY_TRAITS_H

#include /**/ "ace/pre.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/CORBA_methods.h"


namespace TAO
{
  class Null_RefCount_Policy;
  class True_RefCount_Policy;

#if !defined (_MSC_VER) || (_MSC_VER >= 1310)
  /**
   * @struct RefCount_Policy_Traits
   *
   * @brief Compile-time selection of RefCount_Policy operations,
   *        etc.
   *
   * This primary template is used to select RefCount_Policy
   * operations, etc at compile-time based on the reference counting
   * policy and type being operated on.
   *
   * @note This merely a forward declaration since we really only care
   *       about the partial specializations below.
   */
  template<class RefCountPolicy, typename TypePtr>
  struct RefCount_Policy_Traits;

  /**
   * @struct RefCount_Policy_Traits
   *
   * @brief No-op reference count policy traits.
   *
   * This partial specialization performs no-op reference counting
   * operations on values of type @a TypePtr if the @a RefCountPolicy
   * first argument is @c Null_RefCount_Policy.
   */
  template<typename TypePtr>
  struct RefCount_Policy_Traits<Null_RefCount_Policy, TypePtr>
  {
    /// No-op "release" operation.
    static void release (TypePtr) { }
  };

  template<typename TypePtr>
  struct RefCount_Policy_Traits<True_RefCount_Policy, TypePtr>
  {
    /// Call actual "release" operation on @a ptr value.
    static void release (TypePtr ptr)
    {
      CORBA::release (ptr);
    }
  };

#else

  // MSVC++ 6 can't handle partial template specializations so fall
  // back on an alternate implementation.

  template<class RefCountPolicy> struct RefCount_Policy_Traits;

  template<>
  struct RefCount_Policy_Traits<Null_RefCount_Policy>
  {
    static bool is_refcounted (void)
    {
      return false;
    }
  };

  template<>
  struct RefCount_Policy_Traits<True_RefCount_Policy>
  {
    static bool is_refcounted (void)
    {
      return true;
    }
  };

#endif  /* !_MSC_VER || _MSC_VER >= 1310 */

}  // End namespace TAO

#include /**/ "ace/post.h"

#endif /* TAO_REFCOUNT_POLICY_TRAITS_H */
