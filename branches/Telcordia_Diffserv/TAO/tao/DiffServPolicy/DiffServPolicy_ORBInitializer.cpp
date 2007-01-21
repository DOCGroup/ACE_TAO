// $Id$

#include "tao/DiffServPolicy/DiffServPolicy_ORBInitializer.h"
#include "tao/DiffServPolicy/DiffServPolicyC.h"
#include "tao/DiffServPolicy/DiffServPolicy_Factory.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"

ACE_RCSID (DiffServPolicy,
           DiffServPolicy_ORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_DiffServPolicy_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_DiffServPolicy_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_DiffServPolicy_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  TAO_ORBInitInfo * local_info = dynamic_cast <TAO_ORBInitInfo *> (info);
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
  ACE_CHECK;


  PortableInterceptor::PolicyFactory_var policy_factory =
    policy_factory_ptr;

  ACE_TRY
    {
      info->register_policy_factory (EndpointPolicy::ENDPOINT_POLICY_TYPE,
                                     policy_factory.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
    {
      if (ex.minor () == (CORBA::OMGVMCID | 16))
        {
          // The factory is already there, it happens because the
          // magic initializer in Messaging.cpp registers with the
          // ORB multiple times.  This is an indication that we
          // should do no more work in this ORBInitializer.
          return;
        }
      ACE_RE_THROW;
    }
  ACE_CATCHANY
    {
      // Rethrow any other exceptions...
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK;
}

TAO_END_VERSIONED_NAMESPACE_DECL
