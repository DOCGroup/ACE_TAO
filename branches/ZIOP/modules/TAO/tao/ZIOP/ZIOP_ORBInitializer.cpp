// $Id: ZIOP_ORBInitializer.cpp 72363 2006-04-26 14:18:54Z mesnier_p $

#include "tao/ZIOP/ZIOP_ORBInitializer.h"

#include "tao/ZIOP/ZIOP.h"
#include "tao/ZIOP/Compression_Manager.h"
#include "tao/ZIOP/ZIOP_PolicyFactory.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"

ACE_RCSID (ZIOP,
           ZIOP_ORBInitializer,
           "$Id: ZIOP_ORBInitializer.cpp 72363 2006-04-26 14:18:54Z mesnier_p $")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_ZIOP_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_ZIOP_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  this->register_policy_factories (info
                                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_ZIOP_ORBInitializer::register_policy_factories (
  PortableInterceptor::ORBInitInfo_ptr info
  ACE_ENV_ARG_DECL)
{
  ::ZIOP::CompressionManager_ptr compression_manager_ptr;
  ACE_NEW_THROW_EX (compression_manager_ptr,
                    ::TAO::ZIOP::CompressionManager,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  ::ZIOP::CompressionManager_var manager = compression_manager_ptr;
  info->register_initial_reference ("CompressionManager", manager.in ());

  // Register the ZIOP policy factories.
  PortableInterceptor::PolicyFactory_ptr policy_factory_ptr;
  ACE_NEW_THROW_EX (policy_factory_ptr,
                    TAO_ZIOP_PolicyFactory,
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
