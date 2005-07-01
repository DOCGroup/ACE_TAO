// -*- C++ -*-

//=============================================================================
/**
 *  @file    ServerRequestInterceptor_Adapter.h
 *
 *  $Id$
 *
 *   This file an adapter class to simplify the support of
 *   interceptors.
 *
 *  @author  Johnny Willemsen  <jwillemsen@remedy.nl>
 */
//=============================================================================

#ifndef TAO_SERVER_REQUEST_INTERCEPTOR_ADAPTER_H
#define TAO_SERVER_REQUEST_INTERCEPTOR_ADAPTER_H

#include /**/ "ace/pre.h"

#include "TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/CORBA_macros.h"
#include "tao/SystemException.h"

namespace PortableInterceptor
{
  class ServerRequestInterceptor;
  typedef ServerRequestInterceptor *ServerRequestInterceptor_ptr;
}

class TAO_ServerRequest;

namespace TAO
{
  class ServerRequestInfo;

  /**
   * @class ServerRequestInterceptor_Adapter
   */
  class TAO_Export ServerRequestInterceptor_Adapter
  {
  public:
    virtual ~ServerRequestInterceptor_Adapter (void);

    /**
     * @name PortableInterceptor Server Side Interception Points
     *
     * Each of these methods corresponds to a client side interception
     * point.
     */
    //@{
#if TAO_HAS_EXTENDED_FT_INTERCEPTORS == 1
    /// This method implements the "starting" server side interception
    /// point. It will be used as the first interception point and it is
    /// proprietary to TAO.
    /// @@ Will go away once Bug 1369 is fixed
    virtual void tao_ft_interception_point (
        TAO_ServerRequest &server_request,
        TAO::ServerRequestInfo *ri ,
        CORBA::OctetSeq_out oc
        ACE_ENV_ARG_DECL) = 0;
#endif /*TAO_HAS_EXTENDED_FT_INTERCEPTORS*/

    /// This method implements the "intermediate" server side
    /// interception point if the above #ifdef is set to 1 and a
    /// starting intercetion point if it is not set to 1.
    ///
    /// @note This method should have been the "starting" interception
    ///       point according to the interceptor spec. This will be
    ///       fixed once Bug 1369 is completely done.
    virtual void receive_request_service_contexts (
        TAO_ServerRequest &server_request,
        TAO::ServerRequestInfo *ri
        ACE_ENV_ARG_DECL) = 0;

    /// This method an "intermediate" server side interception point.
    virtual void receive_request (
        TAO_ServerRequest &server_request,
        TAO::ServerRequestInfo *ri
        ACE_ENV_ARG_DECL) = 0;

    /// This method implements one of the "ending" server side
    /// interception points.
    virtual void send_reply (
        TAO_ServerRequest &server_request,
        TAO::ServerRequestInfo *ri
        ACE_ENV_ARG_DECL) = 0;

    /// This method implements one of the "ending" server side
    /// interception points.
    virtual void send_exception (
        TAO_ServerRequest &server_request,
        TAO::ServerRequestInfo * ri
        ACE_ENV_ARG_DECL) = 0;

    /// This method implements one of the "ending" server side
    /// interception points.
    virtual void send_other (
        TAO_ServerRequest &server_request,
        TAO::ServerRequestInfo * ri
        ACE_ENV_ARG_DECL) = 0;
    //@}

    /// Register an interceptor.
    virtual void add_interceptor (
      PortableInterceptor::ServerRequestInterceptor_ptr interceptor
      ACE_ENV_ARG_DECL) = 0;

    virtual void destroy_interceptors (ACE_ENV_SINGLE_ARG_DECL) = 0;
  };

}
#include /**/ "ace/post.h"

#endif /* TAO_SERVER_REQUEST_INTERCEPTOR_ADAPTER_H */
