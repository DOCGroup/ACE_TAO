// -*- C++ -*-

// ===================================================================
/**
 *  @file   Interceptor_List.h
 *
 *  $Id$
 *
 *  @author Ossama Othman <ossama@uci.edu>
 *  @author Johnny Willemsen  <jwillemsen@remedy.nl>
 */
// ===================================================================

#ifndef TAO_INTERCEPTOR_LIST_H
#define TAO_INTERCEPTOR_LIST_H

#include /**/ "ace/pre.h"

#include "ace/Array_Base.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Array_Base.h"

namespace PortableInterceptor
{
  class Interceptor;
  typedef Interceptor *Interceptor_ptr;
}

namespace TAO
{
  /**
   * @class Interceptor_List
   *
   * @brief Template for portable interceptor lists.
   *
   * Template for the various portable interceptor lists used
   * internally by TAO.
   */
  template <typename InterceptorType>
  class Interceptor_List
  {
  public:
    /// Define the traits for the underlying portable interceptor array.
    typedef typename InterceptorType::_var_type InterceptorType_var_type;
    typedef typename InterceptorType::_ptr_type InterceptorType_ptr_type;
    typedef ACE_Array_Base<InterceptorType_var_type> TYPE;

    /// Constructor.
    Interceptor_List (void);

    /**
     * Register an interceptor.
     * @retval 0 Interceptor added without problems
     * @retval -1 Interceptor not added because of a duplicate name
     */
    int add_interceptor (
      InterceptorType_ptr_type i
      ACE_ENV_ARG_DECL);

    /// Return reference to the underlying Portable Interceptor array.
    TYPE & interceptors (void);

    void destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL);

  protected:

    /// Return the interceptor in sequence element @a index.
    PortableInterceptor::Interceptor_ptr interceptor (size_t index);

  private:
    /// Dynamic array of registered interceptors.
    TYPE interceptors_;

  };
}

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "tao/Interceptor_List.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Interceptor_List.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#include /**/ "ace/post.h"

#endif /* TAO_INTERCEPTOR_LIST_H */
