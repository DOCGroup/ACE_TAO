// $Id$

#include "server_interceptors.h"

#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

ACE_RCSID (Benchmark,
           server_interceptors,
           "$Id$")

const CORBA::ULong request_ctx_id = 0xdead;
//const CORBA::ULong reply_ctx_id = 0xbeef;   // Never used.

Vault_Server_Request_Interceptor::Vault_Server_Request_Interceptor (void)
  : myname_ ("Vault_Server_Interceptor")
{
}

Vault_Server_Request_Interceptor::~Vault_Server_Request_Interceptor ()
{
}

char *
Vault_Server_Request_Interceptor::name (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Interceptor::destroy (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Vault_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "authenticate") == 0)
    {
      IOP::ServiceId id = request_ctx_id;
      IOP::ServiceContext_var sc =
        ri->get_request_service_context (id);

      const char *buf =
        reinterpret_cast<const char *> (sc->context_data.get_buffer ());

      if (ACE_OS::strcmp (buf, "root123") != 0)
        throw CORBA::NO_PERMISSION ();
    }

  if (ACE_OS::strcmp (op.in (), "update_records") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

      Test_Interceptors::Secure_Vault::Record *record;
      CORBA::Long id;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i++].argument >>= id;
      paramlist[i].argument >>= record;
    }
}

void
Vault_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "update_records") == 0)
    {
      CORBA::Long result;
      CORBA::Any_var result_any = ri->result ();

      (result_any.in ()) >>= result;
    }
}

void
Vault_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}

void
Vault_Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}


//////////////////////////////////  Context /////////////////////////

Vault_Server_Request_Context_Interceptor::Vault_Server_Request_Context_Interceptor (void)
  : myname_ ("Vault_Server_Context_Interceptor")
{
}

Vault_Server_Request_Context_Interceptor::~Vault_Server_Request_Context_Interceptor ()
{
}

char *
Vault_Server_Request_Context_Interceptor::name ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Context_Interceptor::destroy ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Vault_Server_Request_Context_Interceptor::receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Server_Request_Context_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  IOP::ServiceId id = request_ctx_id;
  IOP::ServiceContext_var sc =
    ri->get_request_service_context (id);

  const char *buf = reinterpret_cast<const char *> (sc->context_data.get_buffer ());

  if (ACE_OS::strcmp (buf, "root123") !=0)
    throw CORBA::NO_PERMISSION ();
}

void
Vault_Server_Request_Context_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Vault_Server_Request_Context_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Vault_Server_Request_Context_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}

///////////////////////////////////Dynamic ////////////////////////////////////

Vault_Server_Request_Dynamic_Interceptor::Vault_Server_Request_Dynamic_Interceptor (void)
  : myname_ ("Vault_Server_Dynamic_Interceptor")
{
}

Vault_Server_Request_Dynamic_Interceptor::~Vault_Server_Request_Dynamic_Interceptor ()
{
}

char *
Vault_Server_Request_Dynamic_Interceptor::name ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Dynamic_Interceptor::destroy ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Vault_Server_Request_Dynamic_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "authenticate") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

      const char *user;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i].argument >>= user;
    }

  if (ACE_OS::strcmp (op.in (), "update_records") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

      Test_Interceptors::Secure_Vault::Record *record;
      CORBA::Long id;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i++].argument >>= id;
      paramlist[i].argument >>= record;
    }
}

void
Vault_Server_Request_Dynamic_Interceptor::receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Server_Request_Dynamic_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "ready") == 0)
    {
      CORBA::Short result;
      CORBA::Any_var result_any = ri->result ();

      (result_any.in ()) >>= result;
    }

  if (ACE_OS::strcmp (op.in (), "update_records") == 0)
    {
      CORBA::Long result;
      CORBA::Any_var result_any = ri->result ();

      (result_any.in ()) >>= result;
    }
}

void
Vault_Server_Request_Dynamic_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Vault_Server_Request_Dynamic_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}

//////////////////////////////NOOP///////////////////////////////////////

Vault_Server_Request_NOOP_Interceptor::Vault_Server_Request_NOOP_Interceptor (void)
  : myname_ ("Vault_Server_NOOP_Interceptor")
{
}

Vault_Server_Request_NOOP_Interceptor::~Vault_Server_Request_NOOP_Interceptor ()
{
}

char *
Vault_Server_Request_NOOP_Interceptor::name (void)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_NOOP_Interceptor::destroy ()
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Vault_Server_Request_NOOP_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Vault_Server_Request_NOOP_Interceptor::receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Vault_Server_Request_NOOP_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

}

void
Vault_Server_Request_NOOP_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Vault_Server_Request_NOOP_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}
