

// $Id$

#include "Protocols_Hooks.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, Protocols_Hooks, "$Id$")

/// destructor
TAO_Protocols_Hooks::~TAO_Protocols_Hooks (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Protocols_Hooks>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Protocols_Hooks>

#endif
