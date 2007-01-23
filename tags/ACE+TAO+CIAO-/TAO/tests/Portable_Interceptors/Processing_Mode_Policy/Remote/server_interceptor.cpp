// $Id$

#include "server_interceptor.h"
#include "tao/AnyTypeCode/AnyTypeCode_Adapter_Impl.h"
#include "tao/AnyTypeCode/DynamicC.h"
#include "tao/AnyTypeCode/TypeCode.h"
#include "ace/Log_Msg.h"
#include "ace/OS_NS_string.h"

ACE_RCSID (Remote,
           server_interceptor,
           "$Id$")

const char *request_msg = "The Echo_Server_Request_Interceptor request message";
const char *reply_msg = "The Echo_Server_Request_Interceptor reply message";
CORBA::ULong Echo_Server_Request_Interceptor::server_interceptor_check_ = 0;

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
  ++server_interceptor_check_;

  CORBA::String_var op = ri->operation (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  PortableInterceptor::ObjectId_var test_oid =
    ri->object_id (ACE_ENV_SINGLE_ARG_PARAMETER);
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
  ++server_interceptor_check_;

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
  if (0 == type.in())
    {
      ACE_DEBUG ((LM_DEBUG,
                  "No TypeCode information available; perhaps the user IDL\n"
                  "  has been compiled with TAO_IDL \"-St -Sa\" options?\n"));
      ACE_THROW (CORBA::NO_IMPLEMENT ());
    }

  ++server_interceptor_check_;

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
  ++server_interceptor_check_;

  // Do Nothing
}
