// $Id$

#include "PolicyFactory.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           PolicyFactory,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    PolicyFactory::~PolicyFactory (void)
    {
    }

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<PolicyFactory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<PolicyFactory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

  } /* namespace Portable_Server */
} /* namespace TAO */
