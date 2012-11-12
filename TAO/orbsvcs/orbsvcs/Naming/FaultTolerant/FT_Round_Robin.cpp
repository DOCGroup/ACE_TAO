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

  // No locations exist, so we cant get the next one
  if (len == 0)
    return false;

  TAO_FT_Location_Index_Map::ENTRY * entry;
  if (this->location_index_map_.find (id, entry) == 0)
    {
      CORBA::ULong & i = entry->int_id_;

      // The suggested location from previous next_member call
      // based on the previous location list. It may be invalid
      // if the list of locations are changed since then.
      // We need verify if the suggested location is still exist.
      // if it does, use it. Otherwise we need look the previous
      // location backwards one by one until we find an existing
      // one in current list then use the next location. If not
      // find a match, then start with 0 index.
      bool found = false;

      for (CORBA::ULong k = i; k > 0 && !found; --k)
      {
        for (CORBA::ULong j = 0; j < len && !found; ++j)
        {
          if (ACE_OS::strcmp (this->last_locations_[k][0].id.in (),
                              locations[j][0].id.in ()) == 0)
          {
            if (k == i)
            {
              i = j;
            }
            else
            {
              i = j + 1;
            }

            found = true;
          }
        }
      }

      if (!found)
        i = 0;

      if (len <= i)
        i = 0;  // Reset, i.e. wrap around

      // No need to release the lock since the LoadManager is
      // collocated.
      CORBA::Object_var member =
        naming_manager->get_member_ref (object_group,
                                      locations[i]);

      // Increment index to point to next location.
      i++;

      // Keep a copy of location list for use in next next_member call.
      this->copy_locations (locations);
      location = locations[i];
      return true;
    }

  // The first time through this method there will be no map entry for 
  // this object group. We will set up for the next time
  // around, and return the location residing at the first
  // position in the "locations of members" sequence.

  // Note that it is safe to set the next_index below to 1 even if the
  // length of the sequence is 1 since the above code handles the
  // boundary case correctly by wrapping around.

  const CORBA::ULong index = 0;
  if (this->location_index_map_.bind (id, index + 1) != 0)
  { // The location was already bound or some failure occured. Should not happen.
    
    throw CORBA::INTERNAL ();
  }

  this->copy_locations (locations);
  location = locations[index];
  return true;
}


void
TAO_FT_Round_Robin::copy_locations (PortableGroup::Locations_var& locations)
{
  this->last_locations_.clear ();
  CORBA::ULong len = locations->length ();
  for (CORBA::ULong j = 0; j < len; ++j)
    this->last_locations_.push_back (locations[j]);
}


TAO_END_VERSIONED_NAMESPACE_DECL
