// -*- C++ -*-
//
// $Id$

#ifndef TAO_INTERCEPTORS_H
#define TAO_INTERCEPTORS_H

#include "tao/PortableInterceptorC.h"
#include "tao/LocalObject.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#pragma warning(push)
#pragma warning(disable:4250)
#endif /* _MSC_VER */

class Echo_Client_Request_Interceptor
  : public virtual PortableInterceptor::ClientRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
  // = Client-side echo interceptor.  For checking interceptor visually only.
public:

  Echo_Client_Request_Interceptor (void);
  // ctor.

  virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Canonical name of the interceptor.

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_poll (
        PortableInterceptor::ClientRequestInfo_ptr
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException
        ));

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri
                             ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_other (
        PortableInterceptor::ClientRequestInfo_ptr
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        PortableInterceptor::ForwardRequest
        ));

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri
                              ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr ri
      ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

protected:

  virtual ~Echo_Client_Request_Interceptor (void);
  // dtor.

private:
  const char *myname_;

};

class Echo_Server_Request_Interceptor
  : public PortableInterceptor::ServerRequestInterceptor,
    public virtual TAO_Local_RefCounted_Object
{
  // = Server-side echo interceptor.  For checking interceptor visually only.
public:
  Echo_Server_Request_Interceptor (void);
  // cotr.

  virtual char * name (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Canonical name of the interceptor.

  virtual void destroy (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
        )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableInterceptor::ForwardRequest
      ));

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri
                                ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri
                           ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri
                               ACE_ENV_ARG_DECL_WITH_DEFAULTS)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (
        PortableInterceptor::ServerRequestInfo_ptr
        ACE_ENV_ARG_DECL_WITH_DEFAULTS
        )
    ACE_THROW_SPEC ((
      CORBA::SystemException,
      PortableInterceptor::ForwardRequest
      ));

protected:

  virtual ~Echo_Server_Request_Interceptor (void);
  // dotr.

private:
  const char *myname_;

};

#if defined(_MSC_VER)
#pragma warning(pop)
#endif /* _MSC_VER */

#endif /* TAO_INTERCEPTORS_H */
