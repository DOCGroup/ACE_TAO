// $Id$

#include "tao/DiffServPolicy/DiffServPolicy_ORBInitializer.h"
#include "tao/DiffServPolicy/DiffServPolicy.h"
#include "tao/DiffServPolicy/DiffServPolicy_Factory.h"
#include "tao/DiffServPolicy/DiffServ_Protocols_Hooks.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"

ACE_RCSID (DiffServPolicy,
           DiffServPolicy_ORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_DiffServPolicy_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
TAO_DiffServPolicy_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);
}

void
TAO_DiffServPolicy_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  if (CORBA::is_nil (this->policy_factory_.in ()))
    {
      PortableInterceptor::PolicyFactory_ptr policy_factory;
      ACE_NEW_THROW_EX (policy_factory,
                        TAO_DiffServ_PolicyFactory,
                          CORBA::NO_MEMORY (
                            CORBA::SystemException::_tao_minor_code (
                              TAO::VMCID,
                              ENOMEM),
                            CORBA::COMPLETED_NO));

      this->policy_factory_ = policy_factory;
    }

  // Bind the same policy factory to all DiffServ related policy
  // types since a single policy factory is used to create each of
  // the different types of DiffServ policies.
  static CORBA::PolicyType const type[] = {
    TAO::CLIENT_NETWORK_PRIORITY_TYPE,
    TAO::NETWORK_PRIORITY_TYPE
  };

  const CORBA::PolicyType *end =
    type + sizeof (type) / sizeof (type[0]);

  for (CORBA::PolicyType const * i = type;
       i != end;
       ++i)
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
              // magic initializer in PortableServer.cpp registers
              // with the ORB multiple times.  This is an indication
              // that we should do no more work in this
              // ORBInitializer.
              return;
            }
          throw;
        }
      catch ( ::CORBA::Exception&)
        {
          // Rethrow any other exceptions...
          throw;
        }
    }
}

TAO_END_VERSIONED_NAMESPACE_DECL
