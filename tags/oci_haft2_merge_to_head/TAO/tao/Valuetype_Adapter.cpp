// @(#) $Id$

#include "tao/Valuetype_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (tao, 
           Valuetype_Adapter, 
           "$Id$")

TAO_Valuetype_Adapter::~TAO_Valuetype_Adapter (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Valuetype_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Valuetype_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


