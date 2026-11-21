#include "client_interceptor.h"
#include "tao/OctetSeqC.h"
#include "ace/Log_Msg.h"
#include "tao/ORB_Constants.h"
#include <cstring>

constexpr IOP::ServiceId service_id = 0x08154711;
constexpr char request_msg[] = "TEST 0123456789 TEST";

char *
Echo_Client_Request_Interceptor::name ()
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy ()
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do Nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const operation =
    ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%C.send_request from %C\n",
              this->myname_,
              operation.in ()));

  // Make the context to send to the target
  IOP::ServiceContext sc;
  sc.context_id = ::service_id;

  CORBA::ULong const string_len = sizeof (request_msg);
  sc.context_data.length (string_len);
  std::memcpy (sc.context_data.get_buffer (), request_msg, string_len);

  // Add this context to the service context list.
  ri->add_request_service_context (sc, false);
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const operation =
    ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "%C.receive_reply from %C\n",
              this->myname_,
              operation.in ()));
}

void
Echo_Client_Request_Interceptor::receive_other (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const operation = ri->operation ();
  ACE_DEBUG ((LM_DEBUG,
              "%C.receive_other from %C\n",
              this->myname_,
              operation.in ()));
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var const operation = ri->operation ();
  ACE_DEBUG ((LM_DEBUG,
              "%C.receive_exception from %C\n",
              this->myname_,
              operation.in ()));
}

