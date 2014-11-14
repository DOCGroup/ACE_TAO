// $Id$

#include "client_interceptor.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

const char *request_msg = "The Echo_Client_Request_Interceptor request message";
const char *reply_msg = "The Echo_Client_Request_Interceptor reply message";
CORBA::ULong Echo_Client_Request_Interceptor::client_interceptor_check_ = 0;

Echo_Client_Request_Interceptor::Echo_Client_Request_Interceptor (void)
  : myname_ ("Echo_Client_Interceptor")
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor ()
{
}

char *
Echo_Client_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy (void)
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    )
{
  // Do nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  client_interceptor_check_++;

  CORBA::String_var op =
    ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::send_request\n"));

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::send_request from "
              "\"%C\"\n",
              op.in ()));

  if (ACE_OS::strcmp (op.in (), "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

      CORBA::Long param;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i].argument >>= param;

      ACE_DEBUG ((LM_DEBUG,
                  "The arg is %d\n",
                  param));
    }
}

void
Echo_Client_Request_Interceptor::receive_other (
  PortableInterceptor::ClientRequestInfo_ptr ri)
{
  client_interceptor_check_++;

  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_other "
              "from \"%C\"\n",
              op.in ()));
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  client_interceptor_check_++;

  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_reply "
              "from \"%C\"\n",
              op.in ()));

  if (ACE_OS::strcmp (op.in (), "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

      CORBA::Long param;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i].argument >>= param;

    }
  else if (ACE_OS::strcmp (op.in (), "calculate") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

      CORBA::Long param1, param2, result;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i++].argument >>= param1;
      paramlist[i].argument >>= param2;

      CORBA::Any_var result_any = ri->result ();

      (result_any.in ()) >>= result;

      ACE_DEBUG ((LM_DEBUG,
                  "The result of calculate is %d + %d = %d\n",
                  param1,
                  param2,
                  result));
    }
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  client_interceptor_check_++;

  CORBA::String_var op = ri->operation ();

  CORBA::String_var exception_id =
    ri->received_exception_id ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::received_exception "
              "from \"%C\"\n",
              op.in ()));

  ACE_DEBUG ((LM_DEBUG,
              "Exception ID = %C\n",
              exception_id.in ()));
}

