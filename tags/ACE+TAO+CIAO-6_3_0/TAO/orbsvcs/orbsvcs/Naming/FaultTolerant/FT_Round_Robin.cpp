// -*- C++ -*-
// $Id$

#include "orbsvcs/Naming/FaultTolerant/FT_Round_Robin.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"

#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FT_Round_Robin::TAO_FT_Round_Robin (void)
  : lock_ (),
    location_index_map_ (TAO_PG_MAX_OBJECT_GROUPS)
{
}

TAO_FT_Round_Robin::~TAO_FT_Round_Robin (void)
{
}


bool
TAO_FT_Round_Robin::next_location (
      PortableGroup::ObjectGroup_ptr object_group,
      TAO_FT_Naming_Manager *naming_manager,
      PortableGroup::Location& location)
{
  const PortableGroup::ObjectGroupId id =
    naming_manager->get_object_group_id (object_group);

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    monitor,
                    this->lock_,
                    0);

  PortableGroup::Locations_var locations =
    naming_manager->locations_of_members (object_group);

  const CORBA::ULong len = locations->length ();

  // No locations exist, so we can't get the next one
  if (len == 0)
    return false;

  TAO_FT_Location_Index_Map::ENTRY * entry;
  if (this->location_index_map_.find (id, entry) == 0)
    {
      CORBA::ULong & i = entry->int_id_;

      if (len <= i)
        i = 0;  // Reset, i.e. wrap around

      location = locations[i];

      // Increment index to point to next location.
      ++i;

      return true;
    }

  // Could not find an entry
  // Create an entry at location 0
  CORBA::ULong start = 0;
  location = locations[start++];
  if (this->location_index_map_.bind (id, start) != 0)
  { // The location was already bound or some failure occured. Should not happen.
    throw CORBA::INTERNAL ();
  }
  return true;
}


TAO_END_VERSIONED_NAMESPACE_DECL
