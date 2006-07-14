// $Id$

// local headers
#include "Stock_PriorityMapping.h"

// TAO headers
#include <tao/RTCORBA/Priority_Mapping_Manager.h>

//
// Stock_PriorityMapping
//
Stock_PriorityMapping::Stock_PriorityMapping (void)
  : TAO_Continuous_Priority_Mapping (5)
{
}

//
// ~Stock_PriorityMapping
//
Stock_PriorityMapping::~Stock_PriorityMapping (void)
{
}

//
// register_mapping
//
void Stock_PriorityMapping::register_mapping (CORBA::ORB_ptr orb)
{
  // Get the PriorityMappingManager.
  CORBA::Object_var obj = orb->resolve_initial_references ("PriorityMappingManager");
  TAO_Priority_Mapping_Manager_var manager = TAO_Priority_Mapping_Manager::_narrow (obj);

  // Create an instance of my mapping and install it.
  RTCORBA::PriorityMapping *my_mapping = new Stock_PriorityMapping;
  manager->mapping (my_mapping);
}
