// -*- C++ -*-
//
// $Id$

#ifndef TAO_CLIENT_INTERCEPTOR_H
#define TAO_CLIENT_INTERCEPTOR_H

#include "testC.h"

#include "tao/PI/PI.h"
#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Vault_Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Client-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Client_Request_Interceptor (void);
  // ctor.
  virtual ~Vault_Client_Request_Interceptor ();
  // dtor.

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

private:
  const char *myname_;
};

class Vault_Client_Request_Context_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Client-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Client_Request_Context_Interceptor (void);
  // ctor.
  virtual ~Vault_Client_Request_Context_Interceptor ();
  // dtor.

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

private:
  const char *myname_;
};

class Vault_Client_Request_Dynamic_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Client-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Client_Request_Dynamic_Interceptor (void);
  // ctor.
  virtual ~Vault_Client_Request_Dynamic_Interceptor ();
  // dtor.

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

private:
  const char *myname_;
};

class Vault_Client_Request_NOOP_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual ::CORBA::LocalObject
{
  // = Client-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Client_Request_NOOP_Interceptor (void);
  // ctor.
  virtual ~Vault_Client_Request_NOOP_Interceptor ();
  // dtor.

  virtual char * name (void);
  // Canonical name of the interceptor.

  virtual void destroy (void);

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri);

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr);

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri);

private:
  const char *myname_;
};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_CLIENT_INTERCEPTOR_H */
