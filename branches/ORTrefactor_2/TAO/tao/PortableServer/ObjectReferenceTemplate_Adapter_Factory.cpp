// $Id$

#include "tao/PortableServer/ObjectReferenceTemplate_Adapter_Factory.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           ObjectReferenceTemplate_Adapter_Factory,
           "$Id$")

TAO_ObjectReferenceTemplate_Adapter_Factory::~TAO_ObjectReferenceTemplate_Adapter_Factory (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_ObjectReferenceTemplate_Adapter_Factory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_ObjectReferenceTemplate_Adapter_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


