// -*- C++ -*-
// $Id$

#ifndef CLIENT_REQUEST_INTERCEPTOR_H
#define CLIENT_REQUEST_INTERCEPTOR_H

#include "ace/config-all.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"
#include "tao/CORBA_String.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/**
 * @class Client_Request_Interceptor
 */
class Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
public:

  /// Constructor.
  Client_Request_Interceptor (void);

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
  virtual char * name (void);

  virtual void destroy (void);

  virtual void send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void send_poll (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (
      PortableInterceptor::ClientRequestInfo_ptr ri);
  //@}

  CORBA::ULong request_count (void) const;

  CORBA::ULong receive_exception_count (void) const;

private:
  /// The number of requests intercepted by this interceptor.
  CORBA::ULong request_count_;

  CORBA::ULong receive_exception_count_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif  /* CLIENT_REQUEST_INTERCEPTOR_H */
