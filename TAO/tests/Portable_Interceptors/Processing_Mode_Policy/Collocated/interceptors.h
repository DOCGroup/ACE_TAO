// -*- C++ -*-
#ifndef TAO_INTERCEPTORS_H
#define TAO_INTERCEPTORS_H
#include /**/ "ace/pre.h"

#include "tao/PI/PI.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/PI_Server/PI_Server.h"
#include "tao/LocalObject.h"

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Echo_Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Client-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Client_Request_Interceptor ();
  // ctor.
  Echo_Client_Request_Interceptor (Echo_Client_Request_Interceptor const &) = delete;
  Echo_Client_Request_Interceptor (Echo_Client_Request_Interceptor &&) = delete;
  Echo_Client_Request_Interceptor &operator= (Echo_Client_Request_Interceptor const &) = delete;
  Echo_Client_Request_Interceptor &operator= (Echo_Client_Request_Interceptor &&) = delete;
  ~Echo_Client_Request_Interceptor () override = default;
  // dtor.

  char * name () override;
  // Canonical name of the interceptor.

  void destroy () override;

  void send_poll (
      PortableInterceptor::ClientRequestInfo_ptr) override;

  void send_request (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_other (PortableInterceptor::ClientRequestInfo_ptr) override;

  void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri) override;

  void receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr ri) override;

  // Some identifiers that are used for error checking
  static CORBA::ULong client_interceptor_check_;
private:
  char const *const myname_;
};

class Echo_Server_Request_Interceptor
  : public PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Server-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Server_Request_Interceptor ();
  // cotr.
  Echo_Server_Request_Interceptor (Echo_Server_Request_Interceptor const &) = delete;
  Echo_Server_Request_Interceptor (Echo_Server_Request_Interceptor &&) = delete;
  Echo_Server_Request_Interceptor &operator= (Echo_Server_Request_Interceptor const &) = delete;
  Echo_Server_Request_Interceptor &operator= (Echo_Server_Request_Interceptor &&) = delete;
  ~Echo_Server_Request_Interceptor () override = default;
  // dotr.

  char * name () override;
  // Canonical name of the interceptor.

  void destroy () override;

  void receive_request_service_contexts (
      PortableInterceptor::ServerRequestInfo_ptr) override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr) override;

  // Some identifiers that are used for error checking
  static CORBA::ULong server_interceptor_check_;

private:
  char const *const myname_;
};

#if defined (_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#include /**/ "ace/post.h"
#endif /* TAO_INTERCEPTORS_H */
