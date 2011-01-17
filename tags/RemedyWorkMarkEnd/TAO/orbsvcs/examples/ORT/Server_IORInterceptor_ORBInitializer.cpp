// $Id$

#include "Server_IORInterceptor_ORBInitializer.h"
#include "Server_IORInterceptor.h"
#include "tao/ORB_Constants.h"

#include "GatewayC.h"

void
Server_IORInterceptor_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    )
{
}

void
Server_IORInterceptor_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  CORBA::Object_var obj =
    info->resolve_initial_references ("Gateway_Object_Factory");

  /// Narrow it down correctly.
  Gateway::Object_Factory_var gateway_object_factory =
    Gateway::Object_Factory::_narrow (obj.in ());

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
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::IORInterceptor_var ior_interceptor = gateway;

  info->add_ior_interceptor (ior_interceptor.in ());
}
