// -*- C++ -*-

// ===================================================================
/**
 *  @file   Interceptor_List_T.h
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

#include "tao/orbconf.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace PortableInterceptor
{
  class Interceptor;
  using Interceptor_ptr = Interceptor *;
}

namespace CORBA
{
  class PolicyList;
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
  template <typename InterceptorType, typename DetailsType>
  class Interceptor_List
  {
  public:
    /// Define the traits for the underlying portable interceptor array.
    using InterceptorType_var_type = typename InterceptorType::_var_type;
    using InterceptorType_ptr_type = typename InterceptorType::_ptr_type;

    struct RegisteredInterceptor
    {
      InterceptorType_var_type interceptor_;
      DetailsType              details_;
    };

    /// Constructor.
    Interceptor_List () = default;

    void add_interceptor (InterceptorType_ptr_type i);

    /// Register an interceptor with policies.
    void add_interceptor (InterceptorType_ptr_type i,
                          CORBA::PolicyList const &policies);

    void destroy_interceptors ();

    /// Return the registered interceptor in sequence element @a index.
    RegisteredInterceptor& registered_interceptor (size_t index);

    /// Return the interceptor in sequence element @a index.
    InterceptorType_ptr_type interceptor (size_t index);

    size_t size () const;

  private:
    using RegisteredArray = ACE_Array_Base<RegisteredInterceptor >;

    /// Dynamic array of registered interceptors.
    RegisteredArray interceptors_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include "tao/PI/Interceptor_List_T.cpp"

#include /**/ "ace/post.h"

#endif /* TAO_INTERCEPTOR_LIST_H */
