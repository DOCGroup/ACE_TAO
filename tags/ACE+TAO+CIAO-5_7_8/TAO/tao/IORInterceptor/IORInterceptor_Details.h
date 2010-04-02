// -*- C++ -*-

//=============================================================================
/**
 *  @file    IORInterceptor_Details.h
 *
 *  $Id$
 *
 *   This file declares a class that manages the details
 *   about a registered client request interceptor.  Policies can be
 *   used when interceptors are registered, and the policy values
 *   will be processed and used to modify the values of the
 *   IORInterceptor_Details object associated with the registered
 *   client request interceptor.
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_IOR_INTERCEPTOR_DETAILS_H
#define TAO_IOR_INTERCEPTOR_DETAILS_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Policy_ForwardC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class IORInterceptor_Details
   *
   * @brief The policy-driven details for a registered IOR interceptor
   *
   * Each time an IOR interceptor is registered with an ORB, an
   * IORInterceptor_Details object will be created and associated with
   * the registered IOR interceptor.  If the interceptor is
   * registered with policies, the policies will be used to adjust the
   * values in the IORInterceptor_Details appropriately.
   *
   * Currently, there are no policies that are applicable to
   * IOR Interceptors.
   */
  class IORInterceptor_Details
  {
  public:
    void apply_policies (const CORBA::PolicyList& policies);
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_IOR_INTERCEPTOR_DETAILS_H */
