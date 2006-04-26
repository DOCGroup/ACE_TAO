// $Id$

#include "tao/Strategies/OC_Endpoint_Selector_Loader.h"
#include "tao/Strategies/OC_Endpoint_Selector_Factory.h"

ACE_RCSID (tao,
           OC_Endpoint_Selector_Loader,
           "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_OC_Endpoint_Selector_Loader::TAO_OC_Endpoint_Selector_Loader (void)
{
#if defined (TAO_AS_STATIC_LIBS)
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_OC_Endpoint_Selector_Factory);
#endif /* TAO_AS_STATIC_LIBS */
}

TAO_END_VERSIONED_NAMESPACE_DECL
