// @(#) $Id$

#include "tao/IORInterceptor_Adapter.h"
#include "ace/Dynamic_Service.h"

ACE_RCSID (tao, 
           IORInterceptor_Adapter, 
           "$Id$")

TAO_IORInterceptor_Adapter::~TAO_IORInterceptor_Adapter (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_IORInterceptor_Adapter>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_IORInterceptor_Adapter>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


