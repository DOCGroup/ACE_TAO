// $Id$

#include "interceptors.h"
#include "testC.h"

ACE_RCSID (Dynamic,
           interceptors,
           "$Id$")

  //const CORBA::ULong request_ctx_id = 0xdead;
  //const CORBA::ULong reply_ctx_id = 0xbeef;
static const char request_msg[] = "The Echo_Request_Interceptor request message";
static const char reply_msg[] = "The Echo_Request_Interceptor reply message";

Echo_Client_Request_Interceptor::Echo_Client_Request_Interceptor (void)
  : myname_ ("Echo_Client_Interceptor")
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor ()
{
}

char *
Echo_Client_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED
    )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::send_request from "
              "\"%s\"\n",
              op.in ()));

  // For the "normal" operation, get the argument list.
  if (ACE_OS::strcmp (op.in (),
                      "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (paramlist->length () != 2)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) All parameters not available \n"));

        }

      CORBA::ULong first = 0, second = 1; // If you dont understand
                                          // why this is  done, then
                                          // try changing it.
      if (paramlist[first].mode != CORBA::PARAM_IN ||
          paramlist[second].mode != CORBA::PARAM_OUT)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in the extracted argument list \n"));
        }

      CORBA::Long param = 0;
      paramlist[first].argument >>= param;

      if (param != 10)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in send_request while checking ",
                      "the value of the extracted ",
                      "arguments \n"));
        }
    }
}

void
Echo_Client_Request_Interceptor::receive_other (
  PortableInterceptor::ClientRequestInfo_ptr ri
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_other "
              "from \"%s\"\n",
              op.in ()));
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_reply "
              "from \"%s\"\n",
              op.in ()));

    // For the "normal" operation, get the argument list.
  if (ACE_OS::strcmp (op.in (),
                      "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      if (paramlist->length () != 2)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) All parameters not available \n"));

        }

      CORBA::ULong first = 0, second = 1; // If you dont understand
                                          // why this is  done, then
                                          // try changing it.
      if (paramlist[first].mode != CORBA::PARAM_IN ||
          paramlist[second].mode != CORBA::PARAM_OUT)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in the extracted argument list \n"));
        }

      CORBA::Long param = 0;
      paramlist[first].argument >>= param;

      if (param != 10)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in send_request while checking ",
                      "the value of the extracted ",
                      "arguments \n"));
        }

      const char *str = 0;

      paramlist[second].argument >>= str;

      CORBA::String_var transfer (str);

      if (ACE_OS::strcmp (str,
                          "DO_NOT_INSULT_MY_INTELLIGENCE") != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "(%P|%t) ERROR in send_request while checking ",
                      "the value of the extracted ",
                      "out arguments \n"));
        }
    }

  if (ACE_OS::strcmp (op.in (), "calculate") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Long param1, param2, result;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i++].argument >>= param1;
      paramlist[i].argument >>= param2;

      CORBA::Any_var result_any = ri->result (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      (result_any.in ()) >>= result;

      ACE_DEBUG ((LM_DEBUG,
                  "The result of calculate() is %d + %d = %d\n",
                  param1,
                  param2,
                  result));
    }

  if (ACE_OS::strcmp (op.in (), "_get_the_structure") == 0)
    {
      CORBA::Any_var a = ri->result (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      Test_Interceptors::Visual::VarLenStruct * v;

      (a.in ()) >>= v;

      ACE_DEBUG ((LM_DEBUG,
                  "The result of the_structure() is:\n"
                  "  flag    = %d\n"
                  "  message = %s\n",
                  v->flag,
                  v->message.in ()));
    }
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::received_exception "
              "from \"%s\"\n",
              op.in ()));


  CORBA::String_var exception_id =
    ri->received_exception_id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Exception ID = %s\n",
              exception_id.in ()));
}

Echo_Server_Request_Interceptor::Echo_Server_Request_Interceptor (void)
  : myname_ ("Echo_Server_Interceptor")
{
}

Echo_Server_Request_Interceptor::~Echo_Server_Request_Interceptor ()
{
}

char *
Echo_Server_Request_Interceptor::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::destroy (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
}

void
Echo_Server_Request_Interceptor::receive_request (
  PortableInterceptor::ServerRequestInfo_ptr ri
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::receive_request from \"%s\"\n",
              op.in ()));

  if (ACE_OS::strcmp (op.in (), "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Long param;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i].argument >>= param;

      ACE_DEBUG ((LM_DEBUG,
                  "The arg is %d\n",
                  param));

     }

  CORBA::String_var tmdi =
    ri->target_most_derived_interface (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Target most derived interface: %s\n",
              tmdi.in ()));
}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_reply from \"%s\"\n",
              op.in ()));

  if (ACE_OS::strcmp (op.in (), "normal") == 0)
    {
      Dynamic::ParameterList_var paramlist =
        ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

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
        ri->arguments (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      CORBA::Long param1, param2, result = 0;
      CORBA::ULong i = 0;  // index -- explicitly used to avoid
                           // overloaded operator ambiguity.
      paramlist[i++].argument >>= param1;
      paramlist[i].argument >>= param2;

      CORBA::Any_var result_any = ri->result (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

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
    PortableInterceptor::ServerRequestInfo_ptr ri
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_exception "
              "from \"%s\"\n",
              op.in ()));


  CORBA::Any_var any =
    ri->sending_exception (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::TypeCode_var type = any->type ();

  const char *exception_id = type->id (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Exception ID = %s\n",
              exception_id));
}

void
Echo_Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  // Do Nothing
}
