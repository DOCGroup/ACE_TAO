#include "client_interceptors.h"

#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/Log_Msg.h"
#include <cstring>

constexpr CORBA::ULong request_ctx_id { 0xdead };

char *
Vault_Client_Request_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_Interceptor::destroy ()
{
}

void
Vault_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const op = ri->operation ();

  if (std::strcmp (op.in (), "authenticate") == 0)
    {
      // Make the context to send the context to the target.
      IOP::ServiceContext sc {};
      sc.context_id = request_ctx_id;

      char const passwd[] = "root123";
      CORBA::ULong const string_len = sizeof (passwd);
      // context_data.replace takes ownership of the allocated buffer.
      CORBA::Octet *const buf = CORBA::OctetSeq::allocbuf (string_len);
      std::strcpy (reinterpret_cast<char *> (buf), passwd);

      sc.context_data.replace (string_len, string_len, buf, true);

      // Add this context to the service context list.
      ri->add_request_service_context (sc, false);
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
Vault_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
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
Vault_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::Any_var any = ri->received_exception ();

  CORBA::TypeCode_var const tc = any->type ();

  char const *const id = tc->id ();

  CORBA::String_var const exception_id =
    ri->received_exception_id ();

  if (std::strcmp (id, exception_id.in ()) != 0)
    ACE_ERROR ((LM_ERROR,
                "Mismatched exception IDs: %s != %s\n",
                id,
                exception_id.in ()));
}


//////////////////////////////////  Context /////////////////////////

char *
Vault_Client_Request_Context_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_Context_Interceptor::destroy ()
{
}

void
Vault_Client_Request_Context_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Client_Request_Context_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  // Make the context to send the context to the target.
  IOP::ServiceContext sc {};
  sc.context_id = request_ctx_id;

  char const passwd[] = "root123";
  CORBA::ULong const string_len = sizeof (passwd);
  // context_data.replace takes ownership of the allocated buffer.
  CORBA::Octet *const buf = CORBA::OctetSeq::allocbuf (string_len);

  std::strcpy (reinterpret_cast<char *> (buf), passwd);

  sc.context_data.replace (string_len, string_len, buf, true);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, false);
}

void
Vault_Client_Request_Context_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Client_Request_Context_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Vault_Client_Request_Context_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::Any_var any = ri->received_exception ();

  CORBA::TypeCode_var const tc = any->type ();

  char const *const id = tc->id ();

  CORBA::String_var const exception_id =
    ri->received_exception_id ();

  if (std::strcmp (id, exception_id.in ()) != 0)
    ACE_ERROR ((LM_ERROR,
                "Mismatched exception IDs %s != %s\n",
                id,
                exception_id.in ()));
}


///////////////////////////////////Dynamic ////////////////////////////////////

char *
Vault_Client_Request_Dynamic_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_Dynamic_Interceptor::destroy ()
{
}

void
Vault_Client_Request_Dynamic_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Client_Request_Dynamic_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
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
Vault_Client_Request_Dynamic_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
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
Vault_Client_Request_Dynamic_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Client_Request_Dynamic_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::Any_var any = ri->received_exception ();

  CORBA::TypeCode_var const tc = any->type ();

  char const *const id = tc->id ();

  CORBA::String_var const exception_id =
    ri->received_exception_id ();

  if (std::strcmp (id, exception_id.in ()) != 0)
    ACE_ERROR ((LM_ERROR,
                "Mismatched exception IDs %s != %s\n",
                id,
                exception_id.in ()));
}


//////////////////////////////NOOP///////////////////////////////////////

char *
Vault_Client_Request_NOOP_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_NOOP_Interceptor::destroy ()
{
}

void
Vault_Client_Request_NOOP_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Client_Request_NOOP_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Vault_Client_Request_NOOP_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Vault_Client_Request_NOOP_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr)
{
}

void
Vault_Client_Request_NOOP_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::Any_var any = ri->received_exception ();

  CORBA::TypeCode_var const tc = any->type ();

  char const *const id = tc->id ();

  CORBA::String_var const exception_id =
    ri->received_exception_id ();

  if (std::strcmp (id, exception_id.in ()) != 0)
    ACE_ERROR ((LM_ERROR,
                "Mismatched exception IDs %s != %s\n",
                id,
                exception_id.in ()));
}


