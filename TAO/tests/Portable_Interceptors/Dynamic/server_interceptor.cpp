#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "tao/AnyTypeCode/DynamicC.h"

#include "server_interceptor.h"
#include "testC.h"

#include "ace/Log_Msg.h"
#include <cstring>

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (int& result)
  : result_ (result)
{
}

char *
Echo_Server_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Echo_Server_Interceptor");
}

void
Echo_Server_Request_Interceptor::destroy ()
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr ri)
{
  bool catched_exception = false;
  try
    {
      CORBA::String_var tmdi = ri->target_most_derived_interface ();
    }
  catch (const ::CORBA::BAD_INV_ORDER& ex)
    {
      // BAD_INV_ORDER should be thrown with minor code 14
      catched_exception = (ex.minor () == (CORBA::OMGVMCID | 14));
    }

  if (!catched_exception)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR, no exception when calling target_most_derived_interface from receive_request_service_contexts\n"));
    }

  catched_exception = false;
  try
    {
      CORBA::Boolean const tmdi = ri->target_is_a ("FOO");
      ACE_UNUSED_ARG(tmdi);
    }
  catch (const ::CORBA::BAD_INV_ORDER& ex)
    {
      // BAD_INV_ORDER should be thrown with minor code 14
      catched_exception = (ex.minor () == (CORBA::OMGVMCID | 14));
    }

  if (!catched_exception)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR, no exception when calling target_is_a from receive_request_service_contexts\n"));
    }

  catched_exception = false;
  try
    {
      PortableInterceptor::ReplyStatus const tmdi = ri->reply_status ();
      ACE_UNUSED_ARG(tmdi);
    }
  catch (const ::CORBA::BAD_INV_ORDER& ex)
    {
      // BAD_INV_ORDER should be thrown with minor code 14
      catched_exception = (ex.minor () == (CORBA::OMGVMCID | 14));
    }

  if (!catched_exception)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR, no exception when calling reply_status from receive_request_service_contexts\n"));
    }

  catched_exception = false;
  try
    {
      Dynamic::ParameterList_var paramlist = ri->arguments ();
    }
  catch (const ::CORBA::BAD_INV_ORDER& ex)
    {
      // BAD_INV_ORDER should be thrown with minor code 14
      catched_exception = (ex.minor () == (CORBA::OMGVMCID | 14));
    }

  if (!catched_exception)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR, no exception when calling arguments from receive_request_service_contexts\n"));
    }
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
  catch (const ::CORBA::BAD_INV_ORDER& ex)
    {
      // BAD_INV_ORDER should be thrown with minor code 14
      catched_exception = (ex.minor () == (CORBA::OMGVMCID | 14));
    }

  if (!catched_exception)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR, no exception when getting reply status\n"));
    }

  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::receive_request from \"%C\"\n",
              op.in ()));

  CORBA::Boolean targetisa = ri->target_is_a ("FOO");
  if (targetisa)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR in receive_request while checking "
                  "target_is_a, it is not a FOO\n"));
    }
  targetisa = ri->target_is_a ("IDL:Test_Interceptors/Visual:1.0");
  if (!targetisa)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR in receive_request while checking "
                  "target_is_a, it should be IDL:Test_Interceptors/Visual:1.0\n"));
    }

  if (std::strcmp (op.in (), "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist = ri->arguments ();

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
          ++this->result_;
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in receive_request while checking "
                      "the type of the extracted out"
                      "argument\n"));
        }
     }

  CORBA::String_var tmdi = ri->target_most_derived_interface ();

  ACE_DEBUG ((LM_DEBUG,
              "Target most derived interface: %C\n",
              tmdi.in ()));

  if (std::strcmp (tmdi.in (), "IDL:Test_Interceptors/Visual:1.0") != 0)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR in receive_request while checking "
                  "target_most_derived_interface\n"));
    }

  catched_exception = false;
  try
    {
      PortableInterceptor::ReplyStatus const tmdi = ri->reply_status ();
      ACE_UNUSED_ARG(tmdi);
    }
  catch (const ::CORBA::BAD_INV_ORDER& ex)
    {
      // BAD_INV_ORDER should be thrown with minor code 14
      catched_exception = (ex.minor () == (CORBA::OMGVMCID | 14));
    }

  if (!catched_exception)
    {
      ++this->result_;
      ACE_ERROR ((LM_ERROR,
                  "(%P|%t) ERROR, no exception when calling reply_status from receive_request\n"));
    }
}

void
Echo_Server_Request_Interceptor::send_reply (PortableInterceptor::ServerRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_reply from \"%C\"\n",
              op.in ()));

  if (std::strcmp (op.in (), "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist = ri->arguments ();

      CORBA::Long param;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i].argument >>= param;
      ACE_DEBUG ((LM_DEBUG,
                  "The arg is %d\n",
                  param));
    }

  if (std::strcmp (op.in (), "calculate") == 0)
    {
      Dynamic::ParameterList_var paramlist = ri->arguments ();

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

  CORBA::Any_var any = ri->sending_exception ();

  CORBA::TypeCode_var type = any->type ();

  CORBA::String_var exception_id = type->id ();

  ACE_DEBUG ((LM_DEBUG,
              "Exception ID = %C\n",
              exception_id.in ()));
}

void
Echo_Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr)
{
  // Do Nothing
}
