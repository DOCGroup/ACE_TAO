// @(#) $Id$

#include "tao/IFR_Client_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID(tao, IFR_Client_Adapter, "$Id$")

TAO_IFR_Client_Adapter::~TAO_IFR_Client_Adapter (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_IFR_Client_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_IFR_Client_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


