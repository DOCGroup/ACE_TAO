//
// $Id$
//

#include "tao/RT_Current.h"

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/Stub.h"
#include "tao/ORB_Core.h"
#include "tao/Priority_Mapping.h"

#include "ace/Thread.h"

#if !defined (__ACE_INLINE__)
#include "tao/RT_Current.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(tao, RT_Current, "$Id$")

TAO_RT_Current::TAO_RT_Current (TAO_ORB_Core *orb_core)
  : orb_core_ (orb_core)
{
}

TAO_RT_Current::~TAO_RT_Current (void)
{
}

RTCORBA::Priority
TAO_RT_Current::the_priority (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  RTCORBA::Priority priority;
  if (this->orb_core_->get_protocols_hooks ()->
      get_thread_priority (this->orb_core_,
                           priority,
                           ACE_TRY_ENV) 
      == -1)
    ACE_THROW_RETURN (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO), -1);

  return priority;
}

void
TAO_RT_Current::the_priority (RTCORBA::Priority the_priority,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->orb_core_->get_protocols_hooks ()
      ->set_thread_priority (this->orb_core_,
                             the_priority,
                             ACE_TRY_ENV) 
      == -1)
    ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));
}

#endif /* TAO_HAS_RT_CORBA == 1 */
