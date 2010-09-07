// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Direct_Priority_Mapping.h"
#include "ace/Sched_Params.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  return true;
}

CORBA::Boolean
TAO_Direct_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
                                       RTCORBA::Priority &corba_priority)
{
  corba_priority = native_priority;
  return true;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
