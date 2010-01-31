// $Id$

// local headers
#include "Stock_PriorityMapping.h"

// TAO headers
#include "tao/RTCORBA/Priority_Mapping_Manager.h"

//
// Stock::Priority_Mapping
//
Stock::Priority_Mapping::Priority_Mapping (void)
  : TAO_Linear_Priority_Mapping (ACE_SCHED_OTHER)
{
}

//
// ~Stock::Priority_Mapping
//
Stock::Priority_Mapping::~Priority_Mapping (void)
{
}

//
// register_mapping
//
void Stock::Priority_Mapping::register_mapping (CORBA::ORB_ptr orb)
{
  // Get the PriorityMappingManager.
  CORBA::Object_var obj = orb->resolve_initial_references ("PriorityMappingManager");
  TAO_Priority_Mapping_Manager_var manager = TAO_Priority_Mapping_Manager::_narrow (obj.in ());

  // Create an instance of my mapping and install it.
  RTCORBA::PriorityMapping *my_mapping = new Stock::Priority_Mapping;
  manager->mapping (my_mapping);
}
