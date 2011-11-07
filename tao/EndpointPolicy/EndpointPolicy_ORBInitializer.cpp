// $Id$

#include "tao/EndpointPolicy/EndpointPolicy_ORBInitializer.h"

#include "tao/EndpointPolicy/EndpointPolicyC.h"
#include "tao/EndpointPolicy/EndpointPolicy_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_EndpointPolicy_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
TAO_EndpointPolicy_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    )
{
  this->register_policy_factories (info);
}

void
TAO_EndpointPolicy_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  TAO_ORBInitInfo * local_info = dynamic_cast <TAO_ORBInitInfo *> (info);

  if (!local_info)
    throw CORBA::INTERNAL ();

  TAO_ORB_Core * the_orb_core = local_info->orb_core ();

  // Register the EndpointPolicy policy factories.
  PortableInterceptor::PolicyFactory_ptr policy_factory_ptr;
  ACE_NEW_THROW_EX (policy_factory_ptr,
                    TAO_EndpointPolicy_Factory(the_orb_core),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));


  PortableInterceptor::PolicyFactory_var policy_factory =
    policy_factory_ptr;

  try
    {
      info->register_policy_factory (EndpointPolicy::ENDPOINT_POLICY_TYPE,
                                     policy_factory.in ());
    }
  catch (const ::CORBA::BAD_INV_ORDER& ex)
    {
      if (ex.minor () == (CORBA::OMGVMCID | 16))
        {
          // The factory is already there, it happens because the
          // magic initializer in Messaging.cpp registers with the
          // ORB multiple times.  This is an indication that we
          // should do no more work in this ORBInitializer.
          return;
        }
      throw;
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
