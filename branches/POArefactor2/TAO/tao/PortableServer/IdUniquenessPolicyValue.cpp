#include "IdUniquenessPolicyValue.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (PortableServer,
           IdUniquenessPolicyValue,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {
    IdUniquenessPolicyValue::~IdUniquenessPolicyValue (void)
    {
    }

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

    template class ACE_Dynamic_Service<IdUniquenessPolicyValue>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<IdUniquenessPolicyValue>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}
