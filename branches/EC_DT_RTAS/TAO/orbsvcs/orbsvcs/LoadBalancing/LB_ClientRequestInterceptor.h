// -*- C++ -*-

//=============================================================================
/**
 * @file LB_ClientRequestInterceptor.h
 *
 * $Id$
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================


#ifndef TAO_LB_CLIENT_REQUEST_INTERCEPTOR_H
#define TAO_LB_CLIENT_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */


/**
 * @class TAO_LB_ClientRequestInterceptor
 *
 * @brief Load balancer client request interceptor that injects an
 *        IOP::ServiceContext into a request's
 *        IOP::ServiceContextList.
 *
 * Load shedding requires that a specific ServiceContext exists
 * in the request's ServiceContextList.  The server side will attempt
 * to detect if that ServiceContext exists, and shed load if it does.
 *
 * @note This model introduces a Denial-of-Service vulnerability.
 *       Malicious or incorrectly configured clients can spoof the
 *       server into thinking that it is invoking requests on a
 *       non-load balanced object.  At that point, the server will let
 *       the request through, completely bypassing the load shedding
 *       mechanism!  A server side-only mechanism can correct this
 *       vulnerability, but at the potential cost of additional
 *       resource overhead.
 */
class TAO_LB_ClientRequestInterceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /**
   * @name Methods Required by the Client Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all client request interceptors.
   */
  //@{
  /// Return the name of this ClientRequestInterceptor.
  virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_poll (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_other (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));
  //@}

};


#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* TAO_LB_CLIENT_REQUEST_INTERCEPTOR_H */
