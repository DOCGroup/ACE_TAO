// $Id$

#include "tao/Thread_Lane_Resources_Manager.h"

ACE_RCSID(tao, Thread_Lane_Resources_Manager, "$Id$")

#if !defined (__ACE_INLINE__)
# include "tao/Thread_Lane_Resources_Manager.i"
#endif /* ! __ACE_INLINE__ */

TAO_Thread_Lane_Resources_Manager::~TAO_Thread_Lane_Resources_Manager (void)
{
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Dynamic_Service<TAO_Thread_Lane_Resources_Manager>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Dynamic_Service<TAO_Thread_Lane_Resources_Manager>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
