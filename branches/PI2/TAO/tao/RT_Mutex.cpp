// $Id$
#include "tao/RT_Mutex.h"

#if ! defined (__ACE_INLINE__)
#include "tao/RT_Mutex.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(TAO, RT_Mutex, "$Id$")

#if (TAO_HAS_RT_CORBA == 1)

TAO_RT_Mutex::TAO_RT_Mutex (void)
{
}

TAO_RT_Mutex::~TAO_RT_Mutex (void)
{
}

void
TAO_RT_Mutex::lock (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

void
TAO_RT_Mutex::unlock (CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
}

CORBA::Boolean
TAO_RT_Mutex::try_lock (TimeBase::TimeT,
                        CORBA::Environment &)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return 0;
}

#endif /* TAO_HAS_RT_CORBA == 1 */

////////////////////////////////////////////////////////////////////////////////

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
