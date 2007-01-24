// $Id$

#include "orbsvcs/PortableGroup/PortableGroup_ORBInitializer.h"
#include "orbsvcs/PortableGroup/PortableGroup_Request_Dispatcher.h"

#include "tao/Exception.h"
#include "tao/ORB_Core.h"
#include "tao/PI/ORBInitInfo.h"
#include "tao/debug.h"

ACE_RCSID (PortableGroup,
           PortableGroup_ORBInitializer,
           "$Id$")

static const char pg_poa_factory_name[] = "TAO_GOA";
static const char pg_poa_factory_directive[] = ACE_DYNAMIC_SERVICE_DIRECTIVE(
  "TAO_GOA", "TAO_PortableGroup", "_make_TAO_PG_Object_Adapter_Factory", "");

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

void
TAO_PortableGroup_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr info
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Narrow to a TAO_ORBInitInfo object to get access to the
  // orb_core() TAO extension.
  TAO_ORBInitInfo_var tao_info = TAO_ORBInitInfo::_narrow (info
                                                           ACE_ENV_ARG_PARAMETER);

  if (CORBA::is_nil (tao_info.in ()))
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "(%P|%t) PortableGroup_ORBInitializer::pre_init:\n"
                    "(%P|%t)    Unable to narrow "
                    "\"PortableInterceptor::ORBInitInfo_ptr\" to\n"
                    "(%P|%t)   \"TAO_ORBInitInfo_ptr.\"\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }

  // Set a new request dispatcher in the ORB.
  PortableGroup_Request_Dispatcher *rd = 0;
  ACE_NEW_THROW_EX (rd,
                    PortableGroup_Request_Dispatcher (),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  tao_info->orb_core ()->request_dispatcher (rd);

  // If the application resolves the root POA, make sure we load the PG POA.
  TAO_ORB_Core::set_poa_factory (pg_poa_factory_name,
                                 pg_poa_factory_directive);
}

void
TAO_PortableGroup_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL
