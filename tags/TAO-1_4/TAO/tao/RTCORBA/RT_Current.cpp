//
// $Id$
//

#include "RT_Current.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "Priority_Mapping.h"

#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Protocols_Hooks.h"

#include "ace/Thread.h"

#if !defined (__ACE_INLINE__)
#include "RT_Current.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID (TAO,
           RT_Current,
           "$Id$")

TAO_RT_Current::TAO_RT_Current (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

TAO_RT_Current::~TAO_RT_Current (void)
{
}

RTCORBA::Priority
TAO_RT_Current::the_priority (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTCORBA::Priority priority;

  TAO_Protocols_Hooks *tph = this->orb_core_->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  int result = tph->get_thread_CORBA_priority (priority
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  if (result == -1)
    ACE_THROW_RETURN (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO), -1);

  return priority;
}

void
TAO_RT_Current::the_priority (RTCORBA::Priority the_priority
                              ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{

  TAO_Protocols_Hooks *tph = this->orb_core_->get_protocols_hooks (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  int result = tph->set_thread_CORBA_priority (the_priority
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (result == -1)
    ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));
}

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
