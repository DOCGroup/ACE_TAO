// $Id$

#include "tao/ZIOP/ZIOP_ORBInitializer.h"

#include "tao/ZIOP/ZIOP.h"
#include "tao/ZIOP/ZIOP_PolicyFactory.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"

ACE_RCSID (ZIOP,
           ZIOP_ORBInitializer,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_ZIOP_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr)
{
}

void
TAO_ZIOP_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info)
{
  this->register_policy_factories (info);
}

void
TAO_ZIOP_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info)
{
  // Register the ZIOP policy factories.
  PortableInterceptor::PolicyFactory_ptr policy_factory_ptr;
  ACE_NEW_THROW_EX (policy_factory_ptr,
                    TAO_ZIOP_PolicyFactory,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));


  PortableInterceptor::PolicyFactory_var policy_factory =
    policy_factory_ptr;

  ACE_TRY
    {
      info->register_policy_factory (ZIOP::COMPRESSION_ENABLING_POLICY_ID,
                                     policy_factory.in ()
                                     ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      info->register_policy_factory (ZIOP::COMPRESSOR_ID_POLICY_ID,
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

