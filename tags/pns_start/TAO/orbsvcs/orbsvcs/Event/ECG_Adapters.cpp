// $Id$

#include "ECG_Adapters.h"

ACE_RCSID(Event, ECG_Adapters, "$Id$")

TAO_ECG_Handler_Shutdown::~TAO_ECG_Handler_Shutdown (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Refcounted_Auto_Ptr<TAO_ECG_Handler_Shutdown, ACE_Null_Mutex>;
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Refcounted_Auto_Ptr<TAO_ECG_Handler_Shutdown, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */

