// -*- C++ -*-

//=============================================================================
/**
 * @file Client_Request_Interceptor.h
 *
 * $Id$
 *
 * Implementation header for the client request interceptor for the
 * PortableInterceptor::ForwardRequest test.
 *
 * @author Ossama Othman <ossama@uci.edu>
 */
//=============================================================================

#ifndef CLIENT_REQUEST_INTERCEPTOR_H
#define CLIENT_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"
#include "tao/CORBA_String.h"

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class Client_Request_Interceptor
 *
 * @brief Client request interceptor that exercises
 * PortableInterceptor::ForwardRequest support.
 *
 * This client request interceptor forwards a request to a secondary
 * object by throwing the PortableInterceptor::ForwardRequest
 * exception from within the send_request() interception point.  It
 * only forwards the initial request.  See the README file for
 * details.
 */
class Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  Client_Request_Interceptor (const char *orb_id,
                              const char *forward_str);

  /**
   * @name Methods Required by the Client Request Interceptor
   * Interface
   *
   * These are methods that must be implemented since they are pure
   * virtual in the abstract base class.  They are the canonical
   * methods required for all client request interceptors.
   */
  //@{
  /// Return the name of this ClientRequestinterceptor.
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

private:

  ///The ID of the ORB this interceptor is registered with.
  CORBA::String_var orb_id_;

  /// Pseudo-reference to the ORB this interceptor is registered
  /// with.
  CORBA::ORB_var orb_;

  /// The number of requests intercepted by this interceptor.
  CORBA::ULong request_count_;

  /// Stringified reference to the object the initial request will be
  /// forwarded to by this interceptor.
  CORBA::String_var forward_str_;

};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* CLIENT_REQUEST_INTERCEPTOR_H */
