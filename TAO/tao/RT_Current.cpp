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

TAO_RT_Current_ptr
TAO_RT_Current::_duplicate (TAO_RT_Current_ptr obj)
{
  if (!CORBA::is_nil (obj))
    obj->_incr_refcnt ();

  return obj;
}

TAO_RT_Current_ptr TAO_RT_Current::_narrow (
    CORBA::Object_ptr obj,
    CORBA::Environment &ACE_TRY_ENV
  )
{
  if (CORBA::is_nil (obj))
    return TAO_RT_Current::_nil ();
  CORBA::Boolean check =
    !obj->_is_a ("IDL:omg.org/RTCORBA/Current:1.0", ACE_TRY_ENV);
  ACE_CHECK_RETURN (TAO_RT_Current::_nil ());
  if (check)
    return TAO_RT_Current::_nil ();

  TAO_Stub *stub = obj->_stubobj ();
  stub->_incr_refcnt ();

  TAO_RT_Current *current;
  ACE_NEW_RETURN (current,
                  TAO_RT_Current (stub),
                  0);

  return current;
}

CORBA::Boolean TAO_RT_Current::_is_a (const char *value,
                                      CORBA::Environment &)
{
  if (
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/RTCORBA/Current:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Current:1.0")) ||
    (!ACE_OS::strcmp ((char *)value, "IDL:omg.org/CORBA/Object:1.0")))
    return 1;
  return 0;
}

const char* TAO_RT_Current::_interface_repository_id (void) const
{
  return "IDL:omg.org/RTCORBA/Current:1.0";
}

RTCORBA::Priority
TAO_RT_Current::the_priority (
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  TAO_ORB_Core *orb_core =
    this->_stubobj ()->orb_core ();

  RTCORBA::Priority priority;
  if (orb_core->get_thread_priority (priority) == -1)
    ACE_THROW_RETURN (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO), -1);

  return 0;
}

void
TAO_RT_Current::the_priority (
      RTCORBA::Priority the_priority,
      CORBA::Environment &ACE_TRY_ENV)
    ACE_THROW_SPEC ((
      CORBA::SystemException
    ))
{
  TAO_ORB_Core *orb_core =
    this->_stubobj ()->orb_core ();

  if (orb_core->set_thread_priority (the_priority) == -1)
    ACE_THROW (CORBA::DATA_CONVERSION (1, CORBA::COMPLETED_NO));
}

#endif /* TAO_HAS_RT_CORBA == 1 */
