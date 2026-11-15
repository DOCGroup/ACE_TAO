// -*- C++ -*-
#ifndef INTERCEPTORS_H
#define INTERCEPTORS_H

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Client-side interceptor that verifies RTCORBA priority context in replies.
class Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:
  explicit Client_Request_Interceptor (char const *orb_id);
  Client_Request_Interceptor (Client_Request_Interceptor const &) = delete;
  Client_Request_Interceptor (Client_Request_Interceptor &&) = delete;
  Client_Request_Interceptor &operator= (Client_Request_Interceptor const &) = delete;
  Client_Request_Interceptor &operator= (Client_Request_Interceptor &&) = delete;

  ~Client_Request_Interceptor () override = default;

  /// Returns the canonical interceptor name.
  char * name () override;

  void destroy () override;

  void send_poll (PortableInterceptor::ClientRequestInfo_ptr) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr) override;

  void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri) override;

private:
  char const * const myname_;

  /// ID of the ORB where this interceptor was created, usually from ORBInitInfo.
  CORBA::String_var const orb_id_;

  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_INTERCEPTORS_H */
