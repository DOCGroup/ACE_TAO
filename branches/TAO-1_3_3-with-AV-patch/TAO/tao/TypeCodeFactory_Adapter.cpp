// @(#) $Id$

#include "tao/TypeCodeFactory_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (tao, 
           TypeCodeFactory_Adapter, 
           "$Id$")

TAO_TypeCodeFactory_Adapter::~TAO_TypeCodeFactory_Adapter (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_TypeCodeFactory_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


