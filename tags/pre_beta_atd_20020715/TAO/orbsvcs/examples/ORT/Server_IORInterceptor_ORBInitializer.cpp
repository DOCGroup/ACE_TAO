// $Id$

#include "Server_IORInterceptor_ORBInitializer.h"
#include "Server_IORInterceptor.h"

#include "GatewayC.h"

#include "tao/ORBInitInfo.h"
#include "tao/ORB_Core.h"

ACE_RCSID (ORT,
           Server_IORInterceptor_ORBInitializer,
           "$Id: ")

void
Server_IORInterceptor_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_IORInterceptor_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CORBA::Object_var obj =
    info->resolve_initial_references ("Gateway_Object_Factory"
                                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  /// Narrow it down correctly.
  Gateway::Object_Factory_var gateway_object_factory =
    Gateway::Object_Factory::_narrow (obj.in () ACE_ENV_ARG_PARAMETER);
  ACE_TRY_CHECK;

  /// Check for nil reference
  if (CORBA::is_nil (gateway_object_factory.in ()))
    ACE_ERROR ((LM_ERROR,
                "Unable to obtain reference to Gateway::Object_Factory "
                "object.\n"));

  PortableInterceptor::IORInterceptor_ptr gateway;
  ACE_NEW_THROW_EX (gateway,
                    Server_IORInterceptor (gateway_object_factory.in ()),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::IORInterceptor_var ior_interceptor =
    gateway;

  info->add_ior_interceptor (ior_interceptor.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
