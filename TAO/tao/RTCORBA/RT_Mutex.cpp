// $Id$
#include "RT_Mutex.h"
#include "RT_ORB.h"

ACE_RCSID(TAO, RT_Mutex, "$Id$")

TAO_RT_Mutex::TAO_RT_Mutex (void)
{
}

TAO_RT_Mutex::~TAO_RT_Mutex (void)
{
}

void
TAO_RT_Mutex::lock (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->mu_.acquire () != 0)
    ACE_THROW (CORBA::INTERNAL ());
}

void
TAO_RT_Mutex::unlock (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (this->mu_.release () != 0)
    ACE_THROW (CORBA::INTERNAL ());
}

CORBA::Boolean
TAO_RT_Mutex::try_lock (TimeBase::TimeT wait_time
                        ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int result;

  if (wait_time == 0)
    // No wait.
    result = this->mu_.tryacquire ();
  else
    {
      // Wait for the specified amount of time before giving up.
      // (wait_time units are 100ns.  See TimeBase.pidl)
      TimeBase::TimeT seconds = wait_time / 10000000u;
      TimeBase::TimeT microseconds = (wait_time % 10000000u) / 10;

      ACE_Time_Value relative_time (ACE_U64_TO_U32 (seconds),
                                    ACE_U64_TO_U32 (microseconds));

      ACE_Time_Value absolute_time =
        relative_time +
        ACE_OS::gettimeofday ();

      result = this->mu_.acquire (absolute_time);
    }

  if (result == 0)
    return 1;
  else if (result == -1 &&
           (errno == ETIME ||
            errno == EBUSY))
    return 0;
  else
    // Some really bad error.
    ACE_THROW_RETURN (CORBA::INTERNAL (), 0);
}

const char *
TAO_RT_Mutex::name (void) const
{
  return 0;
}

////////////////////////////////////////////////////////////////////////////////
#if (TAO_HAS_NAMED_RT_MUTEXES == 1)
TAO_Named_RT_Mutex::TAO_Named_RT_Mutex (const char *name)
  : name_ (name)
{
}

const char *
TAO_Named_RT_Mutex::name (void) const
{
  return this->name_.c_str ();
}
#endif /* TAO_HAS_NAMED_RT_MUTEXES == 1 */

////////////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
