// $Id$

#include "interceptors.h"


ACE_RCSID(Interceptors, interceptors, "$Id$")

#if (TAO_HAS_INTERCEPTORS == 1)
  //const CORBA::ULong request_ctx_id = 0xdead;
  //const CORBA::ULong reply_ctx_id = 0xbeef;
const char *request_msg = "The Echo_Request_Interceptor request message";
const char *reply_msg = "The Echo_Request_Interceptor reply message";

Echo_Client_Request_Interceptor::Echo_Client_Request_Interceptor (void)
  : myname_ ("Echo_Client_Interceptor")
{
}

Echo_Client_Request_Interceptor::~Echo_Client_Request_Interceptor ()
{
}

void
Echo_Client_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Echo_Client_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Echo_Client_Request_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Client_Request_Interceptor::send_poll (
    PortableInterceptor::ClientRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED
    )
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Do nothing
}

void
Echo_Client_Request_Interceptor::send_request (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::send_request from "
              "\"%s\"\n",
              ri->operation ()));

  if (ACE_OS::strcmp (ri->operation (), "normal") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param;
      (paramlist)[0].argument >>= param;

      ACE_DEBUG ((LM_DEBUG,
                  "The arg is %d\n",
                  param));
    }
}

void
Echo_Client_Request_Interceptor::receive_other (
               PortableInterceptor::ClientRequestInfo_ptr
               TAO_ENV_ARG_DECL_NOT_USED
               )
      ACE_THROW_SPEC ((CORBA::SystemException,
                       PortableInterceptor::ForwardRequest))
{
  // Do nothing
}

void
Echo_Client_Request_Interceptor::receive_reply (
    PortableInterceptor::ClientRequestInfo_ptr ri
    TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::receive_reply "
              "from \"%s\"\n",
              ri->operation (TAO_ENV_SINGLE_ARG_PARAMETER)));
  if (ACE_OS::strcmp (ri->operation (), "normal") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param;
      (paramlist)[0].argument >>= param;

      ACE_DEBUG ((LM_DEBUG,
                  "The arg is %d\n",
                  param));
    }
  if (ACE_OS::strcmp (ri->operation (), "calculate") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param1, param2, result;
      (paramlist)[0].argument >>= param1;
      (paramlist)[1].argument >>= param2;
      CORBA::Any result_any = *(ri->result ());
      result_any >>= result;

      ACE_DEBUG ((LM_DEBUG,
                  "The result of calculate is %d + %d = %d\n",
                  param1,
                  param2,
                  result));
    }
}

void
Echo_Client_Request_Interceptor::receive_exception (
    PortableInterceptor::ClientRequestInfo_ptr rinfo
    TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     PortableInterceptor::ForwardRequest))
{
  CORBA::String_var operation =
    rinfo->operation (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::received_exception "
              "from \"%s\"\n",
              operation.in ()));


  CORBA::String_var exception_id =
    rinfo->received_exception_id (TAO_ENV_SINGLE_ARG_PARAMETER);
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

void
Echo_Server_Request_Interceptor::_add_ref (void)
{
  this->_incr_refcnt ();
  // This is a TAO specific implementation.
}

void
Echo_Server_Request_Interceptor::_remove_ref (void)
{
  this->_decr_refcnt ();
  // This is a TAO specific implementation.
}

char *
Echo_Server_Request_Interceptor::name (TAO_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup (this->myname_);
}

void
Echo_Server_Request_Interceptor::receive_request (
  PortableInterceptor::ServerRequestInfo_ptr ri
  TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::receive_request from \"%s\"\n",
              ri->operation ()));

  if (ACE_OS::strcmp (ri->operation (), "normal") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param;
      (paramlist)[0].argument >>= param;

      ACE_DEBUG ((LM_DEBUG,
                  "The arg is %d\n",
                  param));

     }


}

void
Echo_Server_Request_Interceptor::send_reply (
    PortableInterceptor::ServerRequestInfo_ptr ri
    TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_DEBUG ((LM_DEBUG,
              "Echo_Server_Request_Interceptor::send_reply from \"%s\"\n",
              ri->operation ()));

  if (ACE_OS::strcmp (ri->operation (), "normal") == 0)
    {
     Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param;
      (paramlist)[0].argument >>= param;
      ACE_DEBUG ((LM_DEBUG,
                  "The arg is %d\n",
                  param));
    }

  if (ACE_OS::strcmp (ri->operation (), "calculate") == 0)
    {
      Dynamic::ParameterList paramlist = *(ri->arguments ());
      CORBA::Long param1, param2, result;
      (paramlist)[0].argument >>= param1;
      (paramlist)[1].argument >>= param2;

      CORBA::Any result_any = *(ri->result ());
      result_any >>= result;

      ACE_DEBUG ((LM_DEBUG,
                  "The result of calculate  is %d + %d = %d\n",
                  param1,
                  param2,
                  result));
    }
}

void
Echo_Server_Request_Interceptor::send_exception (
    PortableInterceptor::ServerRequestInfo_ptr rinfo
    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableInterceptor::ForwardRequest))
{
  TAO_ENV_ARG_DEFN;

  CORBA::String_var operation =
    rinfo->operation (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Echo_Client_Request_Interceptor::received_exception "
              "from \"%s\"\n",
              operation.in ()));


  CORBA::Any_var any =
    rinfo->sending_exception (ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::TypeCode_var type = any->type ();

  const char *exception_id = type->id (ACE_TRY_ENV);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG,
              "Exception ID = %s\n",
              exception_id));
}

void
Echo_Server_Request_Interceptor::receive_request_service_contexts (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED
    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   PortableInterceptor::ForwardRequest
                   ))
{
  // Do nothing
}

void
Echo_Server_Request_Interceptor::send_other (
    PortableInterceptor::ServerRequestInfo_ptr
    TAO_ENV_ARG_DECL_NOT_USED
    )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   PortableInterceptor::ForwardRequest
                   ))
{
  // Do Nothing
}

#endif /* (TAO_HAS_INTERCEPTORS == 1) */
