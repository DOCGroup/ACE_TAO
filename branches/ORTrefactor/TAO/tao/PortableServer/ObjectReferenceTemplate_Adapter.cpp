// $Id$

#include "tao/PortableServer/ObjectReferenceTemplate_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, ObjectReferenceTemplate_Adapter, "$Id$")


TAO_ObjectReferenceTemplate_Adapter::~TAO_ObjectReferenceTemplate_Adapter (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_ObjectReferenceTemplate_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_ObjectReferenceTemplate_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


