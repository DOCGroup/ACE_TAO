// $Id$

#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "tao/AnyTypeCode/DynamicC.h"

#include "server_interceptor.h"
#include "testC.h"

#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (void)
  : myname_ ("Echo_Server_Interceptor")
{
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor ()
{
}

char *
Echo_Server_Request_Interceptor::name (void)
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy (void)
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr)
{
}

void
Echo_Server_Request_Interceptor::receive_request (
  PortableInterceptor::ServerRequestInfo_ptr ri)
{
  bool catched_exception = false;
  try
    {
      PortableInterceptor::ReplyStatus rstatus = ri->reply_status ();
      ACE_UNUSED_ARG (rstatus);
    }
  catch (const ::CORBA::BAD_INV_ORDER&)
    {
      catched_exception = true;
    }

  if (!catched_exception)
    {
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR, no exception when getting reply status\n"));
    }

  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::receive_request from \"%C\"\n",
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

      i = 1;
      CORBA::TypeCode_var second_typecode = paramlist[i].argument.type ();
      if (second_typecode->kind () != CORBA::tk_null)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in receive_request while checking "
                      "the type of the extracted out"
                      "argument\n"));
        }
     }

  CORBA::String_var tmdi =
    ri->target_most_derived_interface ();

  ACE_DEBUG ((LM_DEBUG,
              "Target most derived interface: %C\n",
              tmdi.in ()));
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_reply from \"%C\"\n",
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

  if (ACE_OS::strcmp (op.in (), "calculate") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments ();

      CORBA::Long param1, param2, result = 0;
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
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{

  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_exception "
              "from \"%C\"\n",
              op.in ()));


  CORBA::Any_var any =
    ri->sending_exception ();

  CORBA::TypeCode_var type = any->type ();

  const char *exception_id = type->id ();

  ACE_DEBUG ((LM_DEBUG,
              "Exception ID = %C\n",
              exception_id));
}

void
Echo_Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}
