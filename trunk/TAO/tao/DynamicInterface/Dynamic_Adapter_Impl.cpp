// $Id$

#include "Dynamic_Adapter_Impl.h"

ACE_RCSID(DynamicInterface, TAO_Dynamic_Adapter_Impl, "$Id$")

#include "Request.h"
#include "Server_Request.h"
#include "tao/Invocation.h"
#include "tao/ORB_Core.h"
#include "tao/Exception.h"

TAO_Dynamic_Adapter_Impl::TAO_Dynamic_Adapter_Impl (void)
{
}

TAO_Dynamic_Adapter_Impl::~TAO_Dynamic_Adapter_Impl (void)
{
}

void
TAO_Dynamic_Adapter_Impl::create_request (
    CORBA::Object_ptr obj,
    CORBA::ORB_ptr orb,
    const char *operation,
    CORBA::NVList_ptr arg_list,
    CORBA::NamedValue_ptr result,
    CORBA::ExceptionList_ptr exceptions,
    CORBA::Request_ptr &request,
    CORBA::Flags req_flags
    TAO_ENV_ARG_DECL
  )
{
  ACE_NEW_THROW_EX (request,
                    CORBA::Request (obj,
                                    orb,
                                    operation,
                                    arg_list,
                                    result,
                                    req_flags,
                                    exceptions
                                    TAO_ENV_ARG_PARAMETER),
                    CORBA::NO_MEMORY (
                        CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM
                          ),
                        CORBA::COMPLETED_MAYBE
                      ));
}

CORBA::Request_ptr
TAO_Dynamic_Adapter_Impl::request (CORBA::Object_ptr obj,
                                   CORBA::ORB_ptr orb,
                                   const char *operation
                                   TAO_ENV_ARG_DECL)
{
  CORBA::Request_ptr req = CORBA::Request::_nil ();
  ACE_NEW_THROW_EX (req,
                    CORBA::Request (obj,
                                    orb,
                                    operation
                                    TAO_ENV_ARG_PARAMETER),
                    CORBA::NO_MEMORY (
                        CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM
                          ),
                        CORBA::COMPLETED_MAYBE
                      ));
  ACE_CHECK_RETURN (CORBA::Request::_nil ());

  return req;
}

CORBA::Boolean
TAO_Dynamic_Adapter_Impl::context_is_nil (CORBA::Context_ptr ctx)
{
  return ctx == 0;
}

void
TAO_Dynamic_Adapter_Impl::context_release (CORBA::Context_ptr ctx)
{
  if (ctx != 0)
    {
      ctx->_decr_refcnt ();
    }
}

CORBA::Boolean
TAO_Dynamic_Adapter_Impl::request_is_nil (CORBA::Request_ptr req)
{
  return req == 0;
}

void
TAO_Dynamic_Adapter_Impl::request_release (CORBA::Request_ptr req)
{
  if (req != 0)
    {
      req->_decr_refcnt ();
    }
}

CORBA::Boolean
TAO_Dynamic_Adapter_Impl::server_request_is_nil (CORBA::ServerRequest_ptr req)
{
  return req == 0;
}

void
TAO_Dynamic_Adapter_Impl::server_request_release (CORBA::ServerRequest_ptr req)
{
  if (req != 0)
    {
      req->_decr_refcnt ();
    }
}

void
TAO_Dynamic_Adapter_Impl::create_exception_list (
    CORBA::ExceptionList_ptr &list
    TAO_ENV_ARG_DECL
  )
{
  ACE_NEW_THROW_EX (list,
                    CORBA::ExceptionList,
                    CORBA::NO_MEMORY (
                        CORBA_SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM
                          ),
                        CORBA::COMPLETED_NO
                      ));
}

int
TAO_Dynamic_Adapter_Impl::Initializer (void)
{
  TAO_ORB_Core::dynamic_adapter_name ("Concrete_Dynamic_Adapter");
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Dynamic_Adapter_Impl);
}

ACE_STATIC_SVC_DEFINE (
    TAO_Dynamic_Adapter_Impl,
    ACE_TEXT ("Concrete_Dynamic_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_Dynamic_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_DynamicInterface, TAO_Dynamic_Adapter_Impl)
