// $Id$

#include "POA_Policy.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           POA_Policy,
           "$Id$")

namespace TAO
{
  POA_Policy::~POA_Policy (void)
  {
  }

  #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

  template class ACE_Dynamic_Service<POA_Policy>;

  #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

  #pragma instantiate ACE_Dynamic_Service<POA_Policy>

  #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
}

