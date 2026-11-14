#include "server_interceptors.h"

#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/Log_Msg.h"
#include <cstring>

constexpr CORBA::ULong request_ctx_id { 0xdead };

char *
Vault_Server_Request_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Interceptor::destroy ()
{
}

void
Vault_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Server_Request_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var const op = ri->operation ();

  if (std::strcmp (op.in (), "authenticate") == 0)
    {
      IOP::ServiceId const id = request_ctx_id;
      IOP::ServiceContext_var const sc =
        ri->get_request_service_context (id);

      char const *const buf =
        reinterpret_cast<char const *> (sc->context_data.get_buffer ());

      if (std::strcmp (buf, "root123") != 0)
        throw CORBA::NO_PERMISSION ();
    }

  if (std::strcmp (op.in (), "update_records") == 0)
    {
      Dynamic::ParameterList_var const paramlist =
        ri->arguments ();

      Test_Interceptors::Secure_Vault::Record const *record {};
      CORBA::Long id {};
      CORBA::ULong i {};  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i++].argument >>= id;
      paramlist[i].argument >>= record;
    }
}

void
Vault_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var const op = ri->operation ();

  if (std::strcmp (op.in (), "update_records") == 0)
    {
      CORBA::Long result {};
      CORBA::Any_var const result_any = ri->result ();

      (result_any.in ()) >>= result;
    }
}

void
Vault_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}

void
Vault_Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}

//////////////////////////////////  Context /////////////////////////

char *
Vault_Server_Request_Context_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Context_Interceptor::destroy ()
{
}

void
Vault_Server_Request_Context_Interceptor::receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Server_Request_Context_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  IOP::ServiceId const id = request_ctx_id;
  IOP::ServiceContext_var const sc =
    ri->get_request_service_context (id);

  char const *const buf = reinterpret_cast<char const *> (sc->context_data.get_buffer ());

  if (std::strcmp (buf, "root123") != 0)
    throw CORBA::NO_PERMISSION ();
}

void
Vault_Server_Request_Context_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Vault_Server_Request_Context_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Vault_Server_Request_Context_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}

///////////////////////////////////Dynamic ////////////////////////////////////

char *
Vault_Server_Request_Dynamic_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_Dynamic_Interceptor::destroy ()
{
}

void
Vault_Server_Request_Dynamic_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var const op = ri->operation ();

  if (std::strcmp (op.in (), "authenticate") == 0)
    {
      Dynamic::ParameterList_var const paramlist =
        ri->arguments ();

      char const *user {};
      CORBA::ULong i {};  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i].argument >>= user;
    }

  if (std::strcmp (op.in (), "update_records") == 0)
    {
      Dynamic::ParameterList_var const paramlist =
        ri->arguments ();

      Test_Interceptors::Secure_Vault::Record const *record {};
      CORBA::Long id {};
      CORBA::ULong i {};  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i++].argument >>= id;
      paramlist[i].argument >>= record;
    }
}

void
Vault_Server_Request_Dynamic_Interceptor::receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Server_Request_Dynamic_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var const op = ri->operation ();

  if (std::strcmp (op.in (), "ready") == 0)
    {
      CORBA::Short result {};
      CORBA::Any_var const result_any = ri->result ();

      (result_any.in ()) >>= result;
    }

  if (std::strcmp (op.in (), "update_records") == 0)
    {
      CORBA::Long result {};
      CORBA::Any_var const result_any = ri->result ();

      (result_any.in ()) >>= result;
    }
}

void
Vault_Server_Request_Dynamic_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Vault_Server_Request_Dynamic_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}

//////////////////////////////NOOP///////////////////////////////////////

char *
Vault_Server_Request_NOOP_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Server_Request_NOOP_Interceptor::destroy ()
{
}

void
Vault_Server_Request_NOOP_Interceptor::receive_request (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Vault_Server_Request_NOOP_Interceptor::receive_request_service_contexts(
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Server_Request_NOOP_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Vault_Server_Request_NOOP_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Vault_Server_Request_NOOP_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}
