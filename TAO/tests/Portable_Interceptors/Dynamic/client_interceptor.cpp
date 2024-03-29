#include "client_interceptor.h"
#include "testC.h"

#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"

#include "ace/Log_Msg.h"
#include <cstring>

Echo_Client_Request_Interceptor::Echo_Client_Request_Interceptor (int& result)
  : result_ (result)
{
}

char *
Echo_Client_Request_Interceptor::name ()
{
  return CORBA::string_dup ("Echo_Client_Interceptor");
}

void
Echo_Client_Request_Interceptor::destroy ()
{
}

void
Echo_Client_Request_Interceptor::send_poll (PortableInterceptor::ClientRequestInfo_ptr)
{
  // Do nothing
}

void
Echo_Client_Request_Interceptor::send_request (PortableInterceptor::ClientRequestInfo_ptr ri)
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
              "Echo_Client_Request_Interceptor::send_request from "
              "\"%C\"\n",
              op.in ()));

  // For the "normal" operation, get the argument list.
  if (std::strcmp (op.in (), "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist = ri->arguments ();

      if (paramlist->length () != 2)
        {
          ++this->result_;
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) All parameters not available\n"));
        }

      CORBA::ULong first = 0, second = 1; // If you dont understand
                                          // why this is  done, then
                                          // try changing it.
      if (paramlist[first].mode != CORBA::PARAM_IN ||
          paramlist[second].mode != CORBA::PARAM_OUT)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in the extracted argument list\n"));
        }

      CORBA::Long param = 0;
      paramlist[first].argument >>= param;

      if (param != 10)
        {
          ++this->result_;
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in send_request while checking "
                      "the value of the extracted "
                      "arguments\n"));
        }

      CORBA::TypeCode_var second_typecode = paramlist[second].argument.type ();
      if (second_typecode->kind () != CORBA::tk_null)
        {
          ++this->result_;
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in send_request while checking "
                      "the type of the extracted out"
                      "argument\n"));
        }
    }
}

void
Echo_Client_Request_Interceptor::receive_other (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_other "
              "from \"%C\"\n",
              op.in ()));
}

void
Echo_Client_Request_Interceptor::receive_reply (PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_reply "
              "from \"%C\"\n",
              op.in ()));

    // For the "normal" operation, get the argument list.
  if (std::strcmp (op.in (), "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist = ri->arguments ();

      if (paramlist->length () != 2)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) All parameters not available\n"));
        }

      CORBA::ULong first = 0, second = 1; // If you dont understand
                                          // why this is  done, then
                                          // try changing it.
      if (paramlist[first].mode != CORBA::PARAM_IN ||
          paramlist[second].mode != CORBA::PARAM_OUT)
        {
          ++this->result_;
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in the extracted argument list\n"));
        }

      CORBA::Long param = 0;
      paramlist[first].argument >>= param;

      if (param != 10)
        {
          ++this->result_;
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in send_request while checking "
                      "the value of the extracted "
                      "arguments\n"));
        }

      const char *str = 0;

      paramlist[second].argument >>= str;

      CORBA::String_var transfer (str);

      if (std::strcmp (str, "DO_NOT_INSULT_MY_INTELLIGENCE") != 0)
        {
          ++this->result_;
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in send_request while checking "
                      "the value of the extracted "
                      "out arguments\n"));
        }
    }

  if (std::strcmp (op.in (), "calculate") == 0)
    {
      Dynamic::ParameterList_var paramlist = ri->arguments ();

      CORBA::Long param1, param2, result;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i++].argument >>= param1;
      paramlist[i].argument >>= param2;

      CORBA::Any_var result_any = ri->result ();

      (result_any.in ()) >>= result;

      ACE_DEBUG ((LM_DEBUG,
                  "The result of calculate() is %d + %d = %d\n",
                  param1,
                  param2,
                  result));
    }

  if (std::strcmp (op.in (), "_get_the_structure") == 0)
    {
      CORBA::Any_var a = ri->result ();

      const Test_Interceptors::Visual::VarLenStruct * v = nullptr;

      (a.in ()) >>= v;

      ACE_DEBUG ((LM_DEBUG,
                  "The result of the_structure() is:\n"
                  "  flag    = %d\n"
                  "  message = %C\n",
                  v->flag,
                  v->message.in ()));
    }
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri)
{
  CORBA::String_var op = ri->operation ();

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::received_exception "
              "from \"%C\"\n",
              op.in ()));

  CORBA::String_var exception_id = ri->received_exception_id ();

  ACE_DEBUG ((LM_DEBUG,
              "Exception ID = %C\n",
              exception_id.in ()));
}


