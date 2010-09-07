// -*- C++ -*-
// $Id$

#include "tao/PI/PI_ORBInitializer.h"

#if TAO_HAS_INTERCEPTORS == 1

#include "tao/PI/PI_PolicyFactory.h"
#include "tao/PI/ProcessingModePolicyC.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfoC.h"
#include "ace/CORBA_macros.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_PI_ORBInitializer::pre_init (PortableInterceptor::ORBInitInfo_ptr)
{
}


void
TAO_PI_ORBInitializer::post_init (PortableInterceptor::ORBInitInfo_ptr info)
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

      this->policy_factory_ = policy_factory;
    }

  this->register_policy_factories (info);
}


void
TAO_PI_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
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
      try
        {
          info->register_policy_factory (*i, this->policy_factory_.in ());
        }
      catch (const ::CORBA::BAD_INV_ORDER& ex)
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
          throw;
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif  /* TAO_HAS_INTERCEPTORS == 1 */
