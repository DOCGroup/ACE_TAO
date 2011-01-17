// $Id$

#include "tao/DynamicInterface/Dynamic_Adapter_Impl.h"
#include "tao/DynamicInterface/Request.h"
#include "tao/DynamicInterface/Server_Request.h"
#include "tao/ORB_Core.h"
#include "tao/SystemException.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
    CORBA::Flags req_flags)
{
  ACE_NEW_THROW_EX (request,
                    CORBA::Request (obj,
                                    orb,
                                    operation,
                                    arg_list,
                                    result,
                                    req_flags,
                                    exceptions
                                   ),
                    CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM
                          ),
                        CORBA::COMPLETED_MAYBE
                      ));
}

CORBA::Request_ptr
TAO_Dynamic_Adapter_Impl::request (CORBA::Object_ptr obj,
                                   CORBA::ORB_ptr orb,
                                   const char *operation)
{
  CORBA::Request_ptr req = CORBA::Request::_nil ();
  ACE_NEW_THROW_EX (req,
                    CORBA::Request (obj,
                                    orb,
                                    operation
                                   ),
                    CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
                            ENOMEM
                          ),
                        CORBA::COMPLETED_MAYBE
                      ));

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
      ctx->_decr_refcount ();
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
      req->_decr_refcount ();
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
      req->_decr_refcount ();
    }
}

void
TAO_Dynamic_Adapter_Impl::create_exception_list (
    CORBA::ExceptionList_ptr &list)
{
  ACE_NEW_THROW_EX (list,
                    CORBA::ExceptionList,
                    CORBA::NO_MEMORY (
                        CORBA::SystemException::_tao_minor_code (
                            TAO::VMCID,
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

TAO_END_VERSIONED_NAMESPACE_DECL

ACE_STATIC_SVC_DEFINE (
    TAO_Dynamic_Adapter_Impl,
    ACE_TEXT ("Concrete_Dynamic_Adapter"),
    ACE_SVC_OBJ_T,
    &ACE_SVC_NAME (TAO_Dynamic_Adapter_Impl),
    ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
    0
  )

ACE_FACTORY_DEFINE (TAO_DynamicInterface, TAO_Dynamic_Adapter_Impl)
