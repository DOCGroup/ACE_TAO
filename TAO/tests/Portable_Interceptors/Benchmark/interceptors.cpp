// $Id$

#include "interceptors.h"

ACE_RCSID(Benchmark, interceptors, "$Id$")

const CORBA::ULong request_ctx_id = 0xdead;
//const CORBA::ULong reply_ctx_id = 0xbeef;   // Never used.

Vault_Client_Request_Interceptor::Vault_Client_Request_Interceptor (void)
  : myname_ ("Vault_Client_Interceptor")
{
}

Vault_Client_Request_Interceptor::~Vault_Client_Request_Interceptor ()
{
}

void
Vault_Client_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Client_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Client_Request_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_Interceptor::send_poll (
                PortableInterceptor::ClientRequestInfo_ptr
                TAO_ENV_ARG_DECL_NOT_USED
                )
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing
}

void
Vault_Client_Request_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  if (ACE_OS::strcmp (ri->operation (), "authenticate") == 0)
    {
      // MAke the context to send the context to the target
      IOP::ServiceContext sc;
      sc.context_id = request_ctx_id;

      const char passwd [20] = "root123";
      CORBA::ULong string_len = ACE_OS::strlen (passwd) + 1;
      CORBA::Octet *buf = 0;
      ACE_NEW (buf,
               CORBA::Octet [string_len]);
      ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), passwd);

      sc.context_data.replace (string_len, string_len, buf, 1);

      // Add this context to the service context list.
      ri->add_request_service_context (sc, 0);
    }

  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      Test_Interceptors::Secure_Vault::Record *record;
      CORBA::Long id;
      paramlist[0].argument >>= id;
      paramlist[1].argument >>= record;
    }

}

void
Vault_Client_Request_Interceptor::receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      CORBA::Long result;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;
    }

}

void 
Vault_Client_Request_Interceptor::receive_other (
               PortableInterceptor::ClientRequestInfo_ptr
               TAO_ENV_ARG_DECL_NOT_USED
               )
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Client_Request_Interceptor::receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr rinfo
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Any_var any = rinfo->received_exception (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TypeCode_var tc = any->type ();

  const char *id = tc->id (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var exception_id =
    rinfo->received_exception_id (ACE_TRY_ENV);

  // Check for exception id mismatch
  ACE_ASSERT (ACE_OS::strcmp (id, exception_id.in ()) == 0);
}


Vault_Server_Request_Interceptor::Vault_Server_Request_Interceptor (void)
  : myname_ ("Vault_Server_Interceptor")
{
}

Vault_Server_Request_Interceptor::~Vault_Server_Request_Interceptor ()
{
}

void
Vault_Server_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Server_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Server_Request_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Interceptor::receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  if (ACE_OS::strcmp (ri->operation (), "authenticate") == 0)
    {

     IOP::ServiceId id = request_ctx_id;
     IOP::ServiceContext *sc = ri->get_request_service_context (id);

     if (sc == 0)
       ACE_THROW (CORBA::NO_MEMORY ());

     const char *buf = ACE_reinterpret_cast (const char *,
                                             sc->context_data.get_buffer ());
     if (ACE_OS::strcmp (buf, "root123") !=0)
       ACE_THROW (Test_Interceptors::Invalid ());
    }

  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      Test_Interceptors::Secure_Vault::Record *record;
      CORBA::Long id;
      paramlist[0].argument >>= id;
      paramlist[1].argument >>= record;
    }


}

void 
Vault_Server_Request_Interceptor::receive_request_service_contexts (
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Server_Request_Interceptor::send_reply (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      CORBA::Long result;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;
    }
}

void
Vault_Server_Request_Interceptor::send_exception (
      PortableInterceptor::ServerRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}

void 
Vault_Server_Request_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}


//////////////////////////////////  Context /////////////////////////

Vault_Client_Request_Context_Interceptor::Vault_Client_Request_Context_Interceptor (void)
  : myname_ ("Vault_Client_Context_Interceptor")
{
}

Vault_Client_Request_Context_Interceptor::~Vault_Client_Request_Context_Interceptor ()
{
}

void
Vault_Client_Request_Context_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Client_Request_Context_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Client_Request_Context_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_Context_Interceptor::send_poll (
                PortableInterceptor::ClientRequestInfo_ptr
                TAO_ENV_ARG_DECL_NOT_USED
                )
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing
}

void
Vault_Client_Request_Context_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // MAke the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = request_ctx_id;

  const char passwd [20] = "root123";
  CORBA::ULong string_len = ACE_OS::strlen (passwd) + 1;
  CORBA::Octet *buf = 0;
  ACE_NEW (buf,
           CORBA::Octet [string_len]);
  ACE_OS::strcpy (ACE_reinterpret_cast (char *, buf), passwd);

  sc.context_data.replace (string_len, string_len, buf, 1);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, 0);

}

void 
Vault_Client_Request_Context_Interceptor::receive_other (
               PortableInterceptor::ClientRequestInfo_ptr
               TAO_ENV_ARG_DECL_NOT_USED
               )
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Client_Request_Context_Interceptor::receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Vault_Client_Request_Context_Interceptor::receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr rinfo
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Any_var any = rinfo->received_exception (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TypeCode_var tc = any->type ();

  const char *id = tc->id (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var exception_id =
    rinfo->received_exception_id (ACE_TRY_ENV);

  // Check for exception id mismatch
  ACE_ASSERT (ACE_OS::strcmp (id, exception_id.in ()) == 0);
}


Vault_Server_Request_Context_Interceptor::Vault_Server_Request_Context_Interceptor (void)
  : myname_ ("Vault_Server_Context_Interceptor")
{
}

Vault_Server_Request_Context_Interceptor::~Vault_Server_Request_Context_Interceptor ()
{
}

void
Vault_Server_Request_Context_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Server_Request_Context_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Server_Request_Context_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Context_Interceptor::receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  IOP::ServiceId id = request_ctx_id;
  IOP::ServiceContext *sc = ri->get_request_service_context (id);

  if (sc == 0)
    ACE_THROW (CORBA::NO_MEMORY ());

  const char *buf = ACE_reinterpret_cast (const char *,
                                          sc->context_data.get_buffer ());
  if (ACE_OS::strcmp (buf, "root123") !=0)
    ACE_THROW (Test_Interceptors::Invalid ());
}

void
Vault_Server_Request_Context_Interceptor::receive_request_service_contexts(
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Server_Request_Context_Interceptor::send_reply (
      PortableInterceptor::ServerRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Vault_Server_Request_Context_Interceptor::send_exception (
      PortableInterceptor::ServerRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void 
Vault_Server_Request_Context_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}

///////////////////////////////////Dynamic ////////////////////////////////////

Vault_Client_Request_Dynamic_Interceptor::Vault_Client_Request_Dynamic_Interceptor (void)
  : myname_ ("Vault_Client_Dynamic_Interceptor")
{
}

Vault_Client_Request_Dynamic_Interceptor::~Vault_Client_Request_Dynamic_Interceptor ()
{
}

void
Vault_Client_Request_Dynamic_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Client_Request_Dynamic_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Client_Request_Dynamic_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_Dynamic_Interceptor::send_poll (
                PortableInterceptor::ClientRequestInfo_ptr
                TAO_ENV_ARG_DECL_NOT_USED
                )
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing
}

void
Vault_Client_Request_Dynamic_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
 if (ACE_OS::strcmp (ri->operation (), "authenticate") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      const char *user;
      paramlist[0].argument >>= user;
    }

 if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      Test_Interceptors::Secure_Vault::Record *record;
      CORBA::Long id;
      paramlist[0].argument >>= id;
      paramlist[1].argument >>= record;
    }

}

void
Vault_Client_Request_Dynamic_Interceptor::receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{

  if (ACE_OS::strcmp (ri->operation (), "ready") == 0)
    {
      CORBA::Short result;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;
    }

  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      CORBA::Long result;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;
    }

}

void 
Vault_Client_Request_Dynamic_Interceptor::receive_other (
               PortableInterceptor::ClientRequestInfo_ptr
               TAO_ENV_ARG_DECL_NOT_USED
               )
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Client_Request_Dynamic_Interceptor::receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr rinfo
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Any_var any = rinfo->received_exception (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TypeCode_var tc = any->type ();

  const char *id = tc->id (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var exception_id =
    rinfo->received_exception_id (ACE_TRY_ENV);

  // Check for exception id mismatch
  ACE_ASSERT (ACE_OS::strcmp (id, exception_id.in ()) == 0);
}


Vault_Server_Request_Dynamic_Interceptor::Vault_Server_Request_Dynamic_Interceptor (void)
  : myname_ ("Vault_Server_Dynamic_Interceptor")
{
}

Vault_Server_Request_Dynamic_Interceptor::~Vault_Server_Request_Dynamic_Interceptor ()
{
}

void
Vault_Server_Request_Dynamic_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Server_Request_Dynamic_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Server_Request_Dynamic_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Dynamic_Interceptor::receive_request (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
 if (ACE_OS::strcmp (ri->operation (), "authenticate") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      const char *user;
      paramlist[0].argument >>= user;
    }

 if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      Test_Interceptors::Secure_Vault::Record *record;
      CORBA::Long id;
      paramlist[0].argument >>= id;
      paramlist[1].argument >>= record;
    }
}

void
Vault_Server_Request_Dynamic_Interceptor::receive_request_service_contexts(
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Server_Request_Dynamic_Interceptor::send_reply (
      PortableInterceptor::ServerRequestInfo_ptr ri
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  if (ACE_OS::strcmp (ri->operation (), "ready") == 0)
    {
      CORBA::Short result;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;
    }

  if (ACE_OS::strcmp (ri->operation (), "update_records") == 0)
    {
      CORBA::Long result;
      CORBA_Any result_any = *(ri->result ());
      result_any >>= result;
    }
}

void
Vault_Server_Request_Dynamic_Interceptor::send_exception (
      PortableInterceptor::ServerRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void 
Vault_Server_Request_Dynamic_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}

//////////////////////////////NOOP///////////////////////////////////////

Vault_Client_Request_NOOP_Interceptor::Vault_Client_Request_NOOP_Interceptor (void)
  : myname_ ("Vault_Client_NOOP_Interceptor")
{
}

Vault_Client_Request_NOOP_Interceptor::~Vault_Client_Request_NOOP_Interceptor ()
{
}

void
Vault_Client_Request_NOOP_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Client_Request_NOOP_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Client_Request_NOOP_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_NOOP_Interceptor::send_poll (
                PortableInterceptor::ClientRequestInfo_ptr
                TAO_ENV_ARG_DECL_NOT_USED
                )
      ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing
}

void
Vault_Client_Request_NOOP_Interceptor::send_request (
      PortableInterceptor::ClientRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{

}

void 
Vault_Client_Request_NOOP_Interceptor::receive_other (
               PortableInterceptor::ClientRequestInfo_ptr
               TAO_ENV_ARG_DECL_NOT_USED
               )
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Client_Request_NOOP_Interceptor::receive_reply (
      PortableInterceptor::ClientRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
    ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Vault_Client_Request_NOOP_Interceptor::receive_exception (
      PortableInterceptor::ClientRequestInfo_ptr rinfo
      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::Any_var any = rinfo->received_exception (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TypeCode_var tc = any->type ();

  const char *id = tc->id (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::String_var exception_id =
    rinfo->received_exception_id (ACE_TRY_ENV);

  // Check for exception id mismatch
  ACE_ASSERT (ACE_OS::strcmp (id, exception_id.in ()) == 0);
}


Vault_Server_Request_NOOP_Interceptor::Vault_Server_Request_NOOP_Interceptor (void)
  : myname_ ("Vault_Server_NOOP_Interceptor")
{
}

Vault_Server_Request_NOOP_Interceptor::~Vault_Server_Request_NOOP_Interceptor ()
{
}

void
Vault_Server_Request_NOOP_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Vault_Server_Request_NOOP_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Vault_Server_Request_NOOP_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_NOOP_Interceptor::receive_request (
      PortableInterceptor::ServerRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

}

void
Vault_Server_Request_NOOP_Interceptor::receive_request_service_contexts(
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Server_Request_NOOP_Interceptor::send_reply (
      PortableInterceptor::ServerRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Vault_Server_Request_NOOP_Interceptor::send_exception (
      PortableInterceptor::ServerRequestInfo_ptr
      TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void 
Vault_Server_Request_NOOP_Interceptor::send_other (
             PortableInterceptor::ServerRequestInfo_ptr
             TAO_ENV_ARG_DECL_NOT_USED)
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}
