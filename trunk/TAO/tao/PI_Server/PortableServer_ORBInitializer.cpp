// -*- C++ -*-

#include "tao/PI_Server/PortableServer_ORBInitializer.h"
#include "tao/PI_Server/PortableServer_PolicyFactory.h"
#include "tao/debug.h"
#include "tao/ORB_Constants.h"
#include "tao/PortableServer/PortableServer.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/ORB_Core.h"

ACE_RCSID (PI_Server,
           PortableServer_ORBInitializer,
           "$Id$")


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_PortableServer_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_PortableServer_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_PortableServer_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
#if !defined (CORBA_E_MICRO)
  // Register the PortableServer policy factories.
  PortableInterceptor::PolicyFactory_ptr tmp;
  ACE_NEW_THROW_EX (tmp,
                    TAO_PortableServer_PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  PortableInterceptor::PolicyFactory_var policy_factory = tmp;

  // Bind the same policy factory to all PortableServer related policy
  // types since a single policy factory is used to create each of the
  // different types of PortableServer policies.
  static CORBA::PolicyType const type[] = {
#if (TAO_HAS_MINIMUM_POA == 0)
    PortableServer::THREAD_POLICY_ID,
    PortableServer::IMPLICIT_ACTIVATION_POLICY_ID,
    PortableServer::SERVANT_RETENTION_POLICY_ID,
    PortableServer::REQUEST_PROCESSING_POLICY_ID,
#endif /* TAO_HAS_MINIMUM_POA == 0 */
    PortableServer::LIFESPAN_POLICY_ID,
    PortableServer::ID_UNIQUENESS_POLICY_ID,
    PortableServer::ID_ASSIGNMENT_POLICY_ID
  };

  CORBA::PolicyType const * end =
    type + sizeof (type) / sizeof (type[0]);

  for (CORBA::PolicyType const *i = type;
       i != end;
       ++i)
    {
      ACE_TRY
        {
          info->register_policy_factory (*i,
                                         policy_factory.in ()
                                         ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::BAD_INV_ORDER, ex)
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
#else
  ACE_UNUSED_ARG (info);
#endif
}

TAO_END_VERSIONED_NAMESPACE_DECL
