// $Id$

#include "client_interceptors.h"

#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/OS_NS_string.h"
#include "ace/Log_Msg.h"

const CORBA::ULong request_ctx_id = 0xdead;
//const CORBA::ULong reply_ctx_id = 0xbeef;   // Never used.

Vault_Client_Request_Interceptor::Vault_Client_Request_Interceptor (void)
  : myname_ ("Vault_Client_Interceptor")
{
}

Vault_Client_Request_Interceptor::~Vault_Client_Request_Interceptor ()
{
}

char *
Vault_Client_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Vault_Client_Request_Interceptor::destroy (void)
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

  CORBA::String_var op = ri->operation ();

  if (ACE_OS::strcmp (op.in (), "authenticate") == 0)
    {
      // Make the context to send the context to the target.
      IOP::ServiceContext sc;
      sc.context_id = request_ctx_id;

      const char passwd[] = "root123";
      CORBA::ULong string_len = sizeof (passwd) + 1;
      CORBA::Octet *buf = 0;
      ACE_NEW (buf,
               CORBA::Octet [string_len]);
      ACE_OS::strcpy (reinterpret_cast<char *> (buf), passwd);

      sc.context_data.replace (string_len, string_len, buf, 1);

      // Add this context to the service context list.
      ri->add_request_service_context (sc, 0);
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
Vault_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
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

  CORBA::TypeCode_var tc = any->type ();

  const char *id = tc->id ();

  CORBA::String_var exception_id =
    ri->received_exception_id ();

  if (ACE_OS::strcmp (id, exception_id.in ()) != 0)
    ACE_ERROR ((LM_ERROR,
                "Mismatched exception IDs: %s != %s\n",
                id,
                exception_id.in ()));
}


//////////////////////////////////  Context /////////////////////////

Vault_Client_Request_Context_Interceptor::Vault_Client_Request_Context_Interceptor (void)
  : myname_ ("Vault_Client_Context_Interceptor")
{
}

Vault_Client_Request_Context_Interceptor::~Vault_Client_Request_Context_Interceptor ()
{
}

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

  // MAke the context to send the context to the target
  IOP::ServiceContext sc;
  sc.context_id = request_ctx_id;

  const char passwd[] = "root123";
  CORBA::ULong string_len = sizeof (passwd) + 1;
  CORBA::Octet *buf = 0;
  ACE_NEW_THROW_EX (buf,
                    CORBA::Octet [string_len],
                    CORBA::NO_MEMORY ());

  ACE_OS::strcpy (reinterpret_cast<char *> (buf), passwd);

  sc.context_data.replace (string_len, string_len, buf, 1);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, 0);
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

  CORBA::TypeCode_var tc = any->type ();

  const char *id = tc->id ();

  CORBA::String_var exception_id =
    ri->received_exception_id ();

  if (ACE_OS::strcmp (id, exception_id.in ()) != 0)
    ACE_ERROR ((LM_ERROR,
                "Mismatched exception IDs %s != %s\n",
                id,
                exception_id.in ()));
}


///////////////////////////////////Dynamic ////////////////////////////////////

Vault_Client_Request_Dynamic_Interceptor::Vault_Client_Request_Dynamic_Interceptor (void)
  : myname_ ("Vault_Client_Dynamic_Interceptor")
{
}

Vault_Client_Request_Dynamic_Interceptor::~Vault_Client_Request_Dynamic_Interceptor ()
{
}

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
Vault_Client_Request_Dynamic_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
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

  CORBA::TypeCode_var tc = any->type ();

  const char *id = tc->id ();

  CORBA::String_var exception_id =
    ri->received_exception_id ();

  if (ACE_OS::strcmp (id, exception_id.in ()) != 0)
    ACE_ERROR ((LM_ERROR,
                "Mismatched exception IDs %s != %s\n",
                id,
                exception_id.in ()));
}


//////////////////////////////NOOP///////////////////////////////////////

Vault_Client_Request_NOOP_Interceptor::Vault_Client_Request_NOOP_Interceptor (void)
  : myname_ ("Vault_Client_NOOP_Interceptor")
{
}

Vault_Client_Request_NOOP_Interceptor::~Vault_Client_Request_NOOP_Interceptor ()
{
}

char *
Vault_Client_Request_NOOP_Interceptor::name (void)
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

  CORBA::TypeCode_var tc = any->type ();

  const char *id = tc->id ();

  CORBA::String_var exception_id =
    ri->received_exception_id ();

  if (ACE_OS::strcmp (id, exception_id.in ()) != 0)
    ACE_ERROR ((LM_ERROR,
                "Mismatched exception IDs %s != %s\n",
                id,
                exception_id.in ()));
}


