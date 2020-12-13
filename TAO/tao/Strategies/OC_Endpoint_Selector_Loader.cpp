// =================================================================
/**
 * @file OC_Endpoint_Selector_Loader.cpp
 *
 * @author Phil Mesnier <mesnier_p@ociweb.com>
 */
// =================================================================

#include "OC_Endpoint_Selector_Loader.h"
#include "OC_Endpoint_Selector_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_OC_Endpoint_Selector_Loader::init (void)
{
  return  ACE_Service_Config::process_directive (ace_svc_desc_TAO_OC_Endpoint_Selector_Factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL
