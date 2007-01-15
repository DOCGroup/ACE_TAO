// $Id$
#include "tao/CSD_Framework/CSD_ORBInitializer.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

ACE_RCSID (CSD_Framework,
           CSD_ORBInitializer,
           "$Id$")

#include "tao/CSD_Framework/CSD_Object_Adapter_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_CSD_ORBInitializer::TAO_CSD_ORBInitializer ()
{
}

void
TAO_CSD_ORBInitializer::pre_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_CSD_ORBInitializer::post_init (
    PortableInterceptor::ORBInitInfo_ptr
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
