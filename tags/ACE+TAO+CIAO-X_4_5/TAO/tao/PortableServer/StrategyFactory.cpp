// $Id$

#include "StrategyFactory.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           StrategyFactory,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    StrategyFactory::~StrategyFactory (void)
    {
    }

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<StrategyFactory>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<StrategyFactory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

  } /* namespace Portable_Server */
} /* namespace TAO */
