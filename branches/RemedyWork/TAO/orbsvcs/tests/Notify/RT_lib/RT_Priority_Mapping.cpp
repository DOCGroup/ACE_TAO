// $Id$

#include "RT_Priority_Mapping.h"



#include "../lib/LookupManager.h"

TAO_Notify_Tests_RT_Priority_Mapping::~TAO_Notify_Tests_RT_Priority_Mapping (void)
{
}

void
TAO_Notify_Tests_RT_Priority_Mapping::init (CORBA::ORB_ptr orb)
{
  CORBA::Object_var object = orb->resolve_initial_references ("PriorityMappingManager");

  RTCORBA::PriorityMappingManager_var mapping_manager =
    RTCORBA::PriorityMappingManager::_narrow (object.in ());

  this->priority_mapping_ = mapping_manager->mapping ();
}

CORBA::Boolean
TAO_Notify_Tests_RT_Priority_Mapping::to_native (RTCORBA::Priority corba_priority,
                                    RTCORBA::NativePriority &native_priority)
{
  return this->priority_mapping_->to_native (corba_priority, native_priority);
}

CORBA::Boolean
TAO_Notify_Tests_RT_Priority_Mapping::to_CORBA (RTCORBA::NativePriority native_priority,
                                RTCORBA::Priority &corba_priority)
{
  return this->priority_mapping_->to_CORBA (native_priority, corba_priority);
}
