// @(#) $Id$

#include "tao/Dynamic_Adapter.h"
#include "tao/ORB_Core.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (tao, 
           Dynamic_Adapter, 
           "$Id$")

TAO_Dynamic_Adapter::~TAO_Dynamic_Adapter (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Dynamic_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Dynamic_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
