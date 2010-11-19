// -*- C++ -*-

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

class Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual CORBA::LocalObject
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

#endif  /* CLIENT_REQUEST_INTERCEPTOR_H */
