// $Id$

#include "tao/orbconf.h"

#if (TAO_HAS_RT_CORBA == 1)

#include "tao/Priority_Mapping.h"

#if !defined (__ACE_INLINE__)
# include "tao/Priority_Mapping.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Priority_Mapping, "$Id$")

TAO_Priority_Mapping::TAO_Priority_Mapping (void)
{
}

TAO_Priority_Mapping::~TAO_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Priority_Mapping::to_native (RTCORBA::Priority,
                                 RTCORBA::NativePriority &)
{
  return 0;
}

CORBA::Boolean
TAO_Priority_Mapping::to_CORBA (RTCORBA::NativePriority,
                                RTCORBA::Priority &)
{
  return 0;
}

#endif /* TAO_HAS_RT_CORBA == 1 */
