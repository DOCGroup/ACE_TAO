// $Id$

#ifndef TAO_INTERCEPTORS_H
#define TAO_INTERCEPTORS_H

#include "testC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if defined(_MSC_VER)
#if (_MSC_VER >= 1200)
#pragma warning(push)
#endif /* _MSC_VER >= 1200 */
#pragma warning(disable:4250)
#endif /* _MSC_VER */

#if (TAO_HAS_INTERCEPTORS == 1)

class Vault_Client_Request_Interceptor
:  public PortableInterceptor::ClientRequestInterceptor//,
   //  public CORBA::LocalObject
{
  // = Client-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Client_Request_Interceptor (CORBA::ORB_ptr orb);
  // ctor.
  virtual ~Vault_Client_Request_Interceptor ();
  // dtor.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Canonical name of the interceptor.

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                             CORBA::Environment &ACE_TRY_ENVV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr
                              TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                                  CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                    PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;

};

class Vault_Server_Request_Interceptor
: public PortableInterceptor::ServerRequestInterceptor//,
//    public CORBA::LocalObject
{
  // = Server-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Server_Request_Interceptor (CORBA::ORB_ptr orb);
  // cotr.
  ~Vault_Server_Request_Interceptor ();
  // dotr.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Canonical name of the interceptor.

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr
        TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                           CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr
                           TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;
};

class Vault_Client_Request_Context_Interceptor
:  public PortableInterceptor::ClientRequestInterceptor//,
   //  public CORBA::LocalObject
{
  // = Client-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Client_Request_Context_Interceptor (CORBA::ORB_ptr orb);
  // ctor.
  virtual ~Vault_Client_Request_Context_Interceptor ();
  // dtor.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Canonical name of the interceptor.

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                             CORBA::Environment &ACE_TRY_ENVV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr
                              TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                                  CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                    PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;

};

class Vault_Server_Request_Context_Interceptor
: public PortableInterceptor::ServerRequestInterceptor//,
//    public CORBA::LocalObject
{
  // = Server-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Server_Request_Context_Interceptor (CORBA::ORB_ptr orb);
  // cotr.
  ~Vault_Server_Request_Context_Interceptor ();
  // dotr.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Canonical name of the interceptor.

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr
        TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                           CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));
  
  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr
                           TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;
};


class Vault_Client_Request_Dynamic_Interceptor
:  public PortableInterceptor::ClientRequestInterceptor//,
   //  public CORBA::LocalObject
{
  // = Client-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Client_Request_Dynamic_Interceptor (CORBA::ORB_ptr orb);
  // ctor.
  virtual ~Vault_Client_Request_Dynamic_Interceptor ();
  // dtor.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Canonical name of the interceptor.

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                             CORBA::Environment &ACE_TRY_ENVV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr
                              TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                                  CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                    PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;

};

class Vault_Server_Request_Dynamic_Interceptor
: public PortableInterceptor::ServerRequestInterceptor//,
//    public CORBA::LocalObject
{
  // = Server-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Server_Request_Dynamic_Interceptor (CORBA::ORB_ptr orb);
  // cotr.
  ~Vault_Server_Request_Dynamic_Interceptor ();
  // dotr.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Canonical name of the interceptor.

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr
        TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                           CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr
                           TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;
};


class Vault_Client_Request_NOOP_Interceptor
:  public PortableInterceptor::ClientRequestInterceptor//,
   //  public CORBA::LocalObject
{
  // = Client-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Client_Request_NOOP_Interceptor (CORBA::ORB_ptr orb);
  // ctor.
  virtual ~Vault_Client_Request_NOOP_Interceptor ();
  // dtor.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Canonical name of the interceptor.

  virtual void send_poll (PortableInterceptor::ClientRequestInfo_ptr
                          TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_request (PortableInterceptor::ClientRequestInfo_ptr ri,
                             CORBA::Environment &ACE_TRY_ENVV =
                             TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri,
                              CORBA::Environment &ACE_TRY_ENV =
                              TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void receive_other (PortableInterceptor::ClientRequestInfo_ptr
                              TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void receive_exception (PortableInterceptor::ClientRequestInfo_ptr ri,
                                  CORBA::Environment &ACE_TRY_ENV =
                                  TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                    PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;

};

class Vault_Server_Request_NOOP_Interceptor
: public PortableInterceptor::ServerRequestInterceptor//,
//    public CORBA::LocalObject
{
  // = Server-side Vault interceptor.  For checking interceptor visually only.
public:
  Vault_Server_Request_NOOP_Interceptor (CORBA::ORB_ptr orb);
  // cotr.
  ~Vault_Server_Request_NOOP_Interceptor ();
  // dotr.

  virtual void _add_ref (void);
  // Increment the reference count.

  virtual void _remove_ref (void);
  // Decrement the reference count.

  virtual char * name (CORBA::Environment &ACE_TRY_ENV = TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Canonical name of the interceptor.

  virtual void receive_request (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void receive_request_service_contexts (
        PortableInterceptor::ServerRequestInfo_ptr
        TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

  virtual void send_reply (PortableInterceptor::ServerRequestInfo_ptr ri,
                           CORBA::Environment &ACE_TRY_ENV =
                           TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException));

  virtual void send_exception (PortableInterceptor::ServerRequestInfo_ptr ri,
                                CORBA::Environment &ACE_TRY_ENV =
                                TAO_default_environment ())
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest));

  virtual void send_other (PortableInterceptor::ServerRequestInfo_ptr
                           TAO_ENV_ARG_DECL_WITH_DEFAULTS)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest));

private:
  const char *myname_;

  CORBA::ORB_var orb_;
};

#if defined(_MSC_VER) && (_MSC_VER >= 1200)
#pragma warning(pop)
#endif /* _MSC_VER */
#endif /* (TAO_HAS_INTERCEPTORS == 1) */
#endif /* TAO_INTERCEPTORS_H */
