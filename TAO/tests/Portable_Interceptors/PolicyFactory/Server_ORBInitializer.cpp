// -*- C++ -*-

#include "Server_ORBInitializer.h"
#include "IORInterceptor.h"
#include "PolicyFactory.h"
#include "testC.h"

#include "tao/ORB_Constants.h"

#include "ace/OS_NS_string.h"

void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    )
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  PortableInterceptor::PolicyFactory_ptr p;
  ACE_NEW_THROW_EX (p,
                    PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::PolicyFactory_var policy_factory (p);

  info->register_policy_factory (Test::POLICY_TYPE,
                                 policy_factory.in ());

  PortableInterceptor::IORInterceptor_ptr i;
  ACE_NEW_THROW_EX (i,
                    IORInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  PortableInterceptor::IORInterceptor_var ior_interceptor (i);

  info->add_ior_interceptor (ior_interceptor.in ());
}
