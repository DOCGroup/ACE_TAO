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

TAO_RT_Current::TAO_RT_Current (void)
{
}

TAO_RT_Current::~TAO_RT_Current (void)
{
}

RTCORBA::Priority
TAO_RT_Current::the_priority (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ORB_Core *orb_core =
    this->_stubobj ()->orb_core ();

  RTCORBA::Priority priority;
  if (orb_core->get_thread_priority (priority) == -1)
    ACE_THROW_RETURN (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO), -1);

  return 0;
}

void
TAO_RT_Current::the_priority (RTCORBA::Priority the_priority,
                              CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  TAO_ORB_Core *orb_core =
    this->_stubobj ()->orb_core ();

  if (orb_core->set_thread_priority (the_priority) == -1)
    ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));
}

#endif /* TAO_HAS_RT_CORBA == 1 */
