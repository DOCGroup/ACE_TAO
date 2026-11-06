#include "RT_Priority_Mapping.h"

#include "../lib/LookupManager.h"

void
TAO_Notify_Tests_RT_Priority_Mapping::init (CORBA::ORB_ptr orb)
{
  CORBA::Object_var const object = orb->resolve_initial_references ("PriorityMappingManager");

  RTCORBA::PriorityMappingManager_var const mapping_manager =
    RTCORBA::PriorityMappingManager::_narrow (object.in ());

  this->priority_mapping_ = mapping_manager->mapping ();
}

CORBA::Boolean
TAO_Notify_Tests_RT_Priority_Mapping::to_native (CORBA::Short corba_priority,
                                                 CORBA::Short& native_priority)
{
  return this->priority_mapping_->to_native (corba_priority, native_priority);
}

CORBA::Boolean
TAO_Notify_Tests_RT_Priority_Mapping::to_CORBA (CORBA::Short native_priority,
                                                CORBA::Short& corba_priority)
{
  return this->priority_mapping_->to_CORBA (native_priority, corba_priority);
}
