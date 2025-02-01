#include "tao/PortableServer/PortableServer.h"
#include "tao/PortableServer/Object_Adapter_Factory.h"
#include "tao/PortableServer/POA_Current_Factory.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

int
TAO_POA_Initializer::init ()
{
  ACE_Service_Config::process_directive (ace_svc_desc_TAO_POA_Current_Factory);

  return ACE_Service_Config::process_directive (ace_svc_desc_TAO_Object_Adapter_Factory);
}

TAO_END_VERSIONED_NAMESPACE_DECL
