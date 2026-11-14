// -*- C++ -*-
#ifndef TAO_SERVER_INTERCEPTOR_H
#define TAO_SERVER_INTERCEPTOR_H

#include "testC.h"

#include "tao/PI_Server/PI_Server.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Vault_Server_Request_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Server-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Server_Request_Interceptor () = default;
  ~Vault_Server_Request_Interceptor () override = default;
  Vault_Server_Request_Interceptor (Vault_Server_Request_Interceptor const &) = delete;
  Vault_Server_Request_Interceptor (Vault_Server_Request_Interceptor &&) = delete;
  Vault_Server_Request_Interceptor &operator= (Vault_Server_Request_Interceptor const &) = delete;
  Vault_Server_Request_Interceptor &operator= (Vault_Server_Request_Interceptor &&) = delete;

  char * name () override;
  // Canonical name of the interceptor.

  void destroy () override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr) override;

private:
  char const *const myname_ { "Vault_Server_Interceptor" };
};

class Vault_Server_Request_Context_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Server-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Server_Request_Context_Interceptor () = default;
  ~Vault_Server_Request_Context_Interceptor () override = default;
  Vault_Server_Request_Context_Interceptor (Vault_Server_Request_Context_Interceptor const &) = delete;
  Vault_Server_Request_Context_Interceptor (Vault_Server_Request_Context_Interceptor &&) = delete;
  Vault_Server_Request_Context_Interceptor &operator= (Vault_Server_Request_Context_Interceptor const &) = delete;
  Vault_Server_Request_Context_Interceptor &operator= (Vault_Server_Request_Context_Interceptor &&) = delete;

  char * name () override;
  // Canonical name of the interceptor.

  void destroy () override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr) override;

private:
  char const *const myname_ { "Vault_Server_Context_Interceptor" };
};

class Vault_Server_Request_Dynamic_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Server-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Server_Request_Dynamic_Interceptor () = default;
  ~Vault_Server_Request_Dynamic_Interceptor () override = default;
  Vault_Server_Request_Dynamic_Interceptor (Vault_Server_Request_Dynamic_Interceptor const &) = delete;
  Vault_Server_Request_Dynamic_Interceptor (Vault_Server_Request_Dynamic_Interceptor &&) = delete;
  Vault_Server_Request_Dynamic_Interceptor &operator= (Vault_Server_Request_Dynamic_Interceptor const &) = delete;
  Vault_Server_Request_Dynamic_Interceptor &operator= (Vault_Server_Request_Dynamic_Interceptor &&) = delete;

  char * name () override;
  // Canonical name of the interceptor.

  void destroy () override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr) override;

private:
  char const *const myname_ { "Vault_Server_Dynamic_Interceptor" };
};

class Vault_Server_Request_NOOP_Interceptor
  : public virtual PortableInterceptor::ServerRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Server-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Server_Request_NOOP_Interceptor () = default;
  ~Vault_Server_Request_NOOP_Interceptor () override = default;
  Vault_Server_Request_NOOP_Interceptor (Vault_Server_Request_NOOP_Interceptor const &) = delete;
  Vault_Server_Request_NOOP_Interceptor (Vault_Server_Request_NOOP_Interceptor &&) = delete;
  Vault_Server_Request_NOOP_Interceptor &operator= (Vault_Server_Request_NOOP_Interceptor const &) = delete;
  Vault_Server_Request_NOOP_Interceptor &operator= (Vault_Server_Request_NOOP_Interceptor &&) = delete;

  char * name () override;
  // Canonical name of the interceptor.

  void destroy () override;

  void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void receive_request_service_contexts (PortableInterceptor::ServerRequestInfo_ptr) override;

  void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri) override;

  void send_other (PortableInterceptor::ServerRequestInfo_ptr) override;

private:
  char const *const myname_ { "Vault_Server_NOOP_Interceptor" };
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_SERVER_INTERCEPTOR_H */
