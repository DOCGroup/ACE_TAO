// $Id$

#include "Object_Loader.h"
#include "ace/Dynamic_Service.h"

#if !defined(__ACE_INLINE__)
#include "Object_Loader.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, Object_Loader, "$Id$")

TAO_Object_Loader::~TAO_Object_Loader (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Object_Loader>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Object_Loader>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
