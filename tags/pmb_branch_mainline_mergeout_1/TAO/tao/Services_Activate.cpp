#include "tao/Services_Activate.h"
#include "ace/Dynamic_Service.h"


ACE_RCSID(tao, Services_Activate, "$Id$")

TAO_Services_Activate::~TAO_Services_Activate (void)
{
  //no-op
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Services_Activate>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Services_Activate>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
