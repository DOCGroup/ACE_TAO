// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerRequestDetails.h
 *
 *  $Id$
 *
 *   This file declares a class that manages the details
 *   about a registered server request interceptor.  Policies can be
 *   used when interceptors are registered, and the policy values
 *   will be processed and used to modify the values of the
 *   ServerRequestDetails object associated with the registered
 *   server request interceptor.
 *
 *  @author  Tim Bradley <bradley_t@ociweb.com>
 */
//=============================================================================

#ifndef TAO_SERVER_REQUEST_DETAILS_H
#define TAO_SERVER_REQUEST_DETAILS_H

#include /**/ "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/PI_includeC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace TAO
{
  /**
   * @class ServerRequestDetails
   *
   * @brief The policy-driven details for a registered server request
   *        interceptor
   *
   * Each time a server request interceptor is registered with an ORB,
   * a ServerRequestDetails object will be created and associated with
   * the registered server request interceptor.  If the interceptor is
   * registered with policies, the policies will be used to adjust the
   * values in the ServerRequestDetails appropriately.
   */
  class ServerRequestDetails
  {
  public:
    ServerRequestDetails (void);

    void apply_policies (const CORBA::PolicyList& policies);

    /// Returns true if the ProcessingMode setting permits the "firing"
    /// of the associated server request interceptor based upon the
    /// remote vs. collocated nature of the current servant request
    /// that is being dispatched.
    bool should_be_processed (bool is_remote_request) const;

  private:

    /// The ProcessingMode setting that can be adjusted via the
    /// PortableInterceptor::ProcessingModePolicy.
    PortableInterceptor::ProcessingMode processing_mode_;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
#include "tao/PI_Server/ServerRequestDetails.inl"
#endif  /* __ACE_INLINE__ */

#endif  /* TAO_HAS_INTERCEPTORS == 1 */

#include /**/ "ace/post.h"

#endif /* TAO_SERVER_REQUEST_DETAILS_H */
