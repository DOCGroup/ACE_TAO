// -*- C++ -*-

#include "tao/PI/PI_ORBInitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/PI_PolicyFactory.h"
#include "tao/PI/ProcessingModePolicyC.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfoC.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

ACE_RCSID (tao,
           PI_ORBInitializer,
           "$Id$")

void
TAO_PI_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr info
                                 ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_UNUSED_ARG (info);
}


void
TAO_PI_ORBInitializer::post_init (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // The PI policy factory is stateless and reentrant, so share a
  // single instance between all ORBs.
  if (CORBA::is_nil (this->policy_factory_.in ()))
    {
      PortableInterceptor::PolicyFactory_ptr policy_factory;
      ACE_NEW_THROW_EX (policy_factory,
                        TAO_PI_PolicyFactory,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                            CORBA::COMPLETED_NO));
      ACE_CHECK;

      this->policy_factory_ = policy_factory;
    }


  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}


void
TAO_PI_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  // Register the PI policy factory.

  // Bind the same policy factory to all PortableInterceptor related policy
  // types since a single policy factory is used to create each of the
  // different types of PortableInterceptor policies.
  CORBA::PolicyType type[] = {
    PortableInterceptor::PROCESSING_MODE_POLICY_TYPE
  };

  const CORBA::PolicyType *end = type + sizeof (type) / sizeof (type[0]);

  for (CORBA::PolicyType *i = type; i != end; ++i)
    {
      ACE_TRY
        {
          info->register_policy_factory (*i,
                                         this->policy_factory_.in ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
        {
          if (ex.minor () == (CORBA::OMGVMCID | 16))
            {
              // The factory is already there, it happens because the
              // magic initializer in PI_Initializer.cpp registers
              // with the ORB multiple times.  This is an indication
              // that we should do no more work in this
              // ORBInitializer.
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
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
