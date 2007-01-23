// -*- C++ -*-

#include "Server_ORBInitializer.h"
#include "IORInterceptor.h"
#include "PolicyFactory.h"
#include "testC.h"

#include "tao/ORB_Constants.h"

#include "ace/OS_NS_string.h"


ACE_RCSID (PolicyFactory,
           Server_ORBInitializer,
           "$Id$")


void
Server_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr /* info */
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
Server_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  PortableInterceptor::PolicyFactory_ptr p;
  ACE_NEW_THROW_EX (p,
                    PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::PolicyFactory_var policy_factory (p);

  info->register_policy_factory (Test::POLICY_TYPE,
                                 policy_factory.in ()
                                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  PortableInterceptor::IORInterceptor_ptr i;
  ACE_NEW_THROW_EX (i,
                    IORInterceptor,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::IORInterceptor_var ior_interceptor (i);

  info->add_ior_interceptor (ior_interceptor.in ()
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
