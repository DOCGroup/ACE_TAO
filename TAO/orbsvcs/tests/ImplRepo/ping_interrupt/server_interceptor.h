// -*- C++ -*-
#ifndef TAO_SERVER_INTERCEPTOR_H
#define TAO_SERVER_INTERCEPTOR_H

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"
#include "tao/PortableServer/POAC.h"
#include "tao/LocalObject.h"
#include "tao/ORB.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

/// Server-side echo interceptor.  For checking interceptor visually only.
class Ping_Death_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
public:
  explicit Ping_Death_Request_Interceptor (int *counter);

  Ping_Death_Request_Interceptor (Ping_Death_Request_Interceptor const &) = delete;
  Ping_Death_Request_Interceptor (Ping_Death_Request_Interceptor &&) = delete;
  Ping_Death_Request_Interceptor &operator= (Ping_Death_Request_Interceptor const &) = delete;
  Ping_Death_Request_Interceptor &operator= (Ping_Death_Request_Interceptor &&) = delete;

  ~Ping_Death_Request_Interceptor () override = default;
  void set_poa (PortableServer::POA_ptr poa);

  /// Canonical name of the interceptor.
  char * name () override;

  void destroy () override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr) override;

private:
  /// Non-owning counter retained by the ORB initializer for the interceptor's lifetime.
  int * const counter_;
  PortableServer::POA_var poa_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_SERVER_INTERCEPTOR_H */
