// $Id$

#include "Priority_Mapping.h"



TAO_Notify_Tests_Priority_Mapping::~TAO_Notify_Tests_Priority_Mapping (void)
{
}

CORBA::Boolean
TAO_Notify_Tests_Priority_Mapping::to_native (CORBA::Short corba_priority,
                                 CORBA::Short &native_priority)
{
  native_priority = corba_priority;
  return 1;
}

CORBA::Boolean
TAO_Notify_Tests_Priority_Mapping::to_CORBA (CORBA::Short native_priority,
                                CORBA::Short &corba_priority)
{
  corba_priority = native_priority;
  return 1;
}
