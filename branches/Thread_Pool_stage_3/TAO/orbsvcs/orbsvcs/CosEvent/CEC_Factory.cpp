// $Id$

#include "CEC_Factory.h"
#include "ace/Dynamic_Service.h"

#if ! defined (__ACE_INLINE__)
#include "CEC_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(CosEvent, CEC_Factory, "$Id$")

TAO_CEC_Factory::~TAO_CEC_Factory (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_CEC_Factory>;

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_CEC_Factory>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
