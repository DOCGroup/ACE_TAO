// $Id$
#include "ObjectReferenceTemplate_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(PortableServer,
          ObjectReferenceTemplate_Adapter,
          "$Id$")

namespace TAO
{
  ObjectReferenceTemplate_Adapter::~ObjectReferenceTemplate_Adapter (void)
  {
  }
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<ObjectReferenceTemplate_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<ObjectReferenceTemplate_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
