//=============================================================================
/**
 *  @file    RTPortableServer.cpp
 *
 *  $Id$
 *
 *  @author Frank Hunleth <fhunleth@cs.wustl.edu>
 */
//=============================================================================


#include "tao/RTPortableServer/RTPortableServer.h"
#include "tao/RTPortableServer/RT_Object_Adapter_Factory.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_RTPortableServer_Initializer::TAO_RTPortableServer_Initializer (void)
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_RT_Object_Adapter_Factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
