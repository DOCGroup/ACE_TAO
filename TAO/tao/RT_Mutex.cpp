// $Id$
#include "tao/RT_Mutex.h"
#include "tao/RT_ORB.h"

ACE_RCSID(TAO, RT_Mutex, "$Id$")

#if (TAO_HAS_RT_CORBA == 1)

TAO_RT_Mutex::TAO_RT_Mutex (void)
{
}

TAO_RT_Mutex::~TAO_RT_Mutex (void)
{
}

void
TAO_RT_Mutex::lock (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->mu_.acquire () != 0)
    ACE_THROW (CORBA::INTERNAL ());
}

void
TAO_RT_Mutex::unlock (CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->mu_.release () != 0)
    ACE_THROW (CORBA::INTERNAL ());
}

CORBA::Boolean
TAO_RT_Mutex::try_lock (TimeBase::TimeT wait_time,
                        CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (wait_time == 0)
    // no wait
    return this->mu_.tryacquire () != -1;
  else
    {
      // Wait for the specified amount of time before giving up.
      // (wait_time units are 100ns.  See TimeBase.pidl)

      TimeBase::TimeT seconds = wait_time / 10000000u;
      TimeBase::TimeT microseconds = (wait_time % 10000000u) / 10;

      ACE_Time_Value relative_time (ACE_U64_TO_U32 (seconds),
                                    ACE_U64_TO_U32 (microseconds));

      ACE_Time_Value absolute_time = relative_time + ACE_OS::gettimeofday ();

      return this->mu_.acquire (absolute_time) != -1;
    }
}

TAO_Named_RT_Mutex::TAO_Named_RT_Mutex (const char *name,
                                        TAO_Named_RT_Mutex_Manager &mutex_mgr)
  : name_ (CORBA::string_dup (name)),
    mutex_mgr_ (mutex_mgr)
{
}

TAO_Named_RT_Mutex::~TAO_Named_RT_Mutex (void)
{
  // Since the last reference to this mutex is being destroyed,
  // unregister ourself from the mutex manager
  this->mutex_mgr_.unregister_mutex (name_);

  CORBA::string_free (name_);
}

#endif /* TAO_HAS_RT_CORBA == 1 */

////////////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
