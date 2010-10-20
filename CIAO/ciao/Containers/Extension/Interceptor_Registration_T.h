// -*- C++ -*-

/**
 *  @file   Interceptor_Registration_T.h
 *
 *  $Id$
 *
 *  @author Marcel Smit <msmit@remedy.nl>
 */

#ifndef CIAO_INTERCEPTOR_REGISTRATION_H
#define CIAO_INTERCEPTOR_REGISTRATION_H

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include <map>

struct priority_compare {
  bool operator() (const ::CORBA::UShort& lhs, const ::CORBA::UShort& rhs) const
  {return lhs>rhs;}
};

namespace CIAO
{
  /**
   * @class Interceptor_Registration
   *
   * @brief Template for Container Portable Interceptor registrations.
   *
   * Template for the various Container Portable Interceptor lists used
   * internally by CIAO. The following Interceptor Types are known:
   *    - ClientContainerInterceptor
   *    - ServantContainerInterceptor
   *    - ServerContainerInterceptor
   *    - StubContainerInterceptor
   *
   *  Part of COPI (Container Portable Interceptor)
   */
  template <typename BASE, typename COPITYPE>
  class Interceptor_Registration_T
    : public BASE
  {
  protected :
    /// Constructor.
    Interceptor_Registration_T (void);
    ~Interceptor_Registration_T (void);

    /// Define the traits for the underlying portable interceptor array.
    typedef typename COPITYPE::_ptr_type InterceptorType_ptr_type;

    /// Return the Cookie of a new registered interceptor.
    ::Components::Cookie *register_interceptor (
      InterceptorType_ptr_type ict);

    /// Removes a registered interceptor from the list. The registered
    /// interceptor is indicated by a cookie. If the passed cookie
    /// cannot be found in the list, a InvalidRegistration exception
    /// will be thrown.
    /// Returns the interceptor found.
    InterceptorType_ptr_type unregister_interceptor (
      ::Components::Cookie *ck);

  public:
    /// List accessors.
    size_t size (void) const;
    InterceptorType_ptr_type interceptor (size_t index);

    /// Return the interceptor with the given cookie.
    InterceptorType_ptr_type interceptor (::Components::Cookie *ck);

  private:
    /// Dynamic array of registered interceptors.
    typedef std::pair< ::Components::Cookie *,
                       InterceptorType_ptr_type>
            InterceptorCookie;

    typedef std::multimap< ::CORBA::UShort,
                           InterceptorCookie,
                           priority_compare >
            RegisteredInterceptors;

    RegisteredInterceptors interceptors_;
  };
}

#include "ciao/Containers/Extension/Interceptor_Registration_T.cpp"

#endif /* CIAO_INTERCEPTOR_REGISTRATION_H */
