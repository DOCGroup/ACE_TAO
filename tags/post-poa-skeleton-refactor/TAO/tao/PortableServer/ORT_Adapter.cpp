// $Id$

#include "ORT_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(PortableServer,
          ORT_Adapter,
          "$Id$")

namespace TAO
{
  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<ORT_Adapter>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<ORT_Adapter>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}
