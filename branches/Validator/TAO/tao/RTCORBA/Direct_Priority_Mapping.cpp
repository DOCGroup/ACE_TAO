// $Id$

#include "tao/orbconf.h"

#include "Direct_Priority_Mapping.h"
#include "ace/Sched_Params.h"

#if !defined (__ACE_INLINE__)
# include "Direct_Priority_Mapping.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(Strategies, Direct_Priority_Mapping, "$Id$")

TAO_Direct_Priority_Mapping::TAO_Direct_Priority_Mapping (long)
{
}

TAO_Direct_Priority_Mapping::~TAO_Direct_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Direct_Priority_Mapping::to_native (RTCORBA::Priority corba_priority,
                                        RTCORBA::NativePriority &native_priority)
{
  native_priority = corba_priority;
  return 1;
}

CORBA::Boolean
TAO_Direct_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
                                       RTCORBA::Priority &corba_priority)
{
  corba_priority = native_priority;
  return 1;
}
