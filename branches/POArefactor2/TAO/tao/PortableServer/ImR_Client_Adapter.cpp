// @(#) $Id$

#include "ImR_Client_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (tao,
           ImR_Client_Adapter,
           "$Id$")

namespace TAO
{
  namespace Portable_Server
  {

    ImR_Client_Adapter::~ImR_Client_Adapter (void)
    {
    }

    #if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
    template class ACE_Dynamic_Service<ImR_Client_Adapter>;
    #elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
    #pragma instantiate ACE_Dynamic_Service<ImR_Client_Adapter>

    #endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
  }
}

