/* -*- C++ -*- */

#include "BiDirGIOP.h"
#include "BiDir_ORBInitializer.h"
#include "tao/ORB_Core.h"

ACE_RCSID(BiDir_GIOP, BiDirGIOP, "$Id$")

TAO_BiDirGIOP_Loader::TAO_BiDirGIOP_Loader (void)
{
}

int
TAO_BiDirGIOP_Loader::activate (CORBA::ORB_ptr,
                                int,
                                char *[]
                                TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (TAO_DEF_GIOP_MINOR >= 2)
    {
      PortableInterceptor::ORBInitializer_ptr tmp_orb_initializer =
        PortableInterceptor::ORBInitializer::_nil ();
      PortableInterceptor::ORBInitializer_var bidir_orb_initializer;

      /// Register the BiDir ORBInitializer.
      ACE_NEW_THROW_EX (tmp_orb_initializer,
                        TAO_BiDir_ORBInitializer,
                        CORBA::NO_MEMORY (
                            CORBA_SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                ENOMEM),
                            CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (-1);

      bidir_orb_initializer = tmp_orb_initializer;

      PortableInterceptor::register_orb_initializer (bidir_orb_initializer.in ()
                                                     TAO_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (-1);
    }

  return 0;
}

int
TAO_BiDirGIOP_Loader::parse_policy (TAO_ORB_Core *orb_core,
                                    CORBA::Policy_ptr policy
                                    TAO_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  BiDirPolicy::BidirectionalPolicy_var bidir_policy
    = BiDirPolicy::BidirectionalPolicy::_narrow (policy
                                                 TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  // Bidirectional policy. If we have a BiDirectional policy, we set a
  // flag in the ORB_Core for use by the ORB
  if (!CORBA::is_nil (bidir_policy.in ()))
    {
      // Set the flag in the ORB_Core
      if (bidir_policy->value () == BiDirPolicy::BOTH)
        orb_core->bidir_giop_policy (1);
      return 1;
    }

  return 0;
}


int
TAO_BiDirGIOP_Loader::Initializer (void)
{
  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_BiDirGIOP_Loader);
}

ACE_STATIC_SVC_DEFINE (TAO_BiDirGIOP_Loader,
                       ACE_TEXT ("BiDirGIOP_Loader"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_BiDirGIOP_Loader),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_BiDirGIOP, TAO_BiDirGIOP_Loader)
