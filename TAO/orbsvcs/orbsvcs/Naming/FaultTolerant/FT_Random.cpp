// -*- C++ -*-
// $Id$

#include "orbsvcs/Naming/FaultTolerant/FT_Random.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Manager.h"

#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_FT_Random::TAO_FT_Random (void)
{
}

TAO_FT_Random::~TAO_FT_Random (void)
{
}

bool
TAO_FT_Random::next_location (
      PortableGroup::ObjectGroup_ptr object_group,
      TAO_FT_Naming_Manager *naming_manager,
      PortableGroup::Location& location)
{
  PortableGroup::Locations_var locations =
    naming_manager->locations_of_members (object_group);

  const CORBA::ULong len = locations->length ();

  // No locations exist, so we can't get the next one
  if (len == 0)
    return false;

  CORBA::ULong i = 0;
  if (len > 1)
    {
      // Prevent integer arithmetic overflow.
      double flen = static_cast<double> (len);
      do
        {
          i = static_cast<CORBA::ULong> (flen * ACE_OS::rand () / (RAND_MAX + 1.0));
        }
      while (i == len);
    }
  location = locations[i];
  return true;
}


TAO_END_VERSIONED_NAMESPACE_DECL
