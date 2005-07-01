// -*- C++ -*-

//=============================================================================
/**
 *  @file    ClientRequestInterceptor_Adapter.h
 *
 *  $Id$
 *
 *   This file an adapter class to simplify the support of
 *   interceptors.
 *
 *  @author  Nanbor Wang <nanbor@cs.wustl.edu>
 *  @author  Ossama Othman <ossama@uci.edu>
 *  @author  Kirthika Parameswaran  <kirthika@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CLIENT_REQUEST_INTERCEPTOR_ADAPTER_IMPL_H
#define TAO_CLIENT_REQUEST_INTERCEPTOR_ADAPTER_IMPL_H

#include /**/ "ace/pre.h"

#include "pi_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ClientRequestInterceptorC.h"
#include "tao/ClientRequestInterceptor_Adapter.h"

#include "Interceptor_List_T.h"

namespace TAO
{
  typedef Interceptor_List< ::PortableInterceptor::ClientRequestInterceptor>
    ClientRequestInterceptor_List;
}

class TAO_ClientRequestInfo;

namespace TAO
{
  class Invocation_Base;

  /**
   * @class ClientRequestInterceptor_Adapter
   *
   * @brief A convenient helper class to invoke registered client request
   *        interceptors
   *
   * This class invokes all registered interceptors at interception
   * point, and enforces flow rules dictated by the Portable Interceptor
   * specification/chapter.
   */
  class TAO_Export ClientRequestInterceptor_Adapter_Impl :
    public ClientRequestInterceptor_Adapter
  {
  public:

    ClientRequestInterceptor_Adapter_Impl (
//        ClientRequestInterceptor_List::TYPE & interceptors,
//        Invocation_Base *invocation
      );

    /**
     * @name PortableInterceptor Client Side Interception Points
     *
     * Each of these methods corresponds to a client side interception
     * point.  There are no "intermediate" interception points on the
     * client side, only "starting" and "ending" interception points.
     *
     */
    //@{
    /// This method implements one of the "starting" client side
    /// interception points.
    virtual void send_request (Invocation_Base &invocation,
                               TAO_ClientRequestInfo *ri
                               ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" client side
    /// interception point.
    virtual void receive_reply (Invocation_Base &invocation,
                                TAO_ClientRequestInfo *ri
                                ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" client side
    /// interception point.
    virtual void receive_exception (Invocation_Base &invocation,
                                    TAO_ClientRequestInfo *ri
                                    ACE_ENV_ARG_DECL);

    /// This method implements one of the "ending" client side
    /// interception point.
    virtual void receive_other (Invocation_Base &invocation,
                                TAO_ClientRequestInfo *ri
                                ACE_ENV_ARG_DECL);
    //@}

    /// Register an interceptor.
    virtual void add_interceptor (
      PortableInterceptor::ClientRequestInterceptor_ptr interceptor
      ACE_ENV_ARG_DECL);

    virtual void destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL);

  protected:

    /// Process the given PortableInterceptor::ForwardRequest exception,
    /// i.e. invoke the receive_other() interception point, in addition
    /// to notifying the Invocation object of the LOCATION_FORWARD.
    void process_forward_request (Invocation_Base &invocation,
                                  TAO_ClientRequestInfo *ri,
                                  PortableInterceptor::ForwardRequest &exc
                                  ACE_ENV_ARG_DECL);

  private:

    /// List of registered interceptors.
    ClientRequestInterceptor_List interceptor_list_;
  };
}

#if defined (__ACE_INLINE__)
#include "ClientRequestInterceptor_Adapter_Impl.inl"
#endif  /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif /* TAO_CLIENT_REQUEST_INTERCEPTOR_ADAPTER_IMPL_H */
