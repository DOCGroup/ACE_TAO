// -*- C++ -*-

#include "LB_LeastLoaded.h"
#include "LB_LoadMap.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "tao/debug.h"

ACE_RCSID (LoadBalancing,
           LB_LeastLoaded,
           "$Id$")


#if !defined (__ACE_INLINE__)
#include "LB_LeastLoaded.inl"
#endif /* defined INLINE */


TAO_LB_LeastLoaded::TAO_LB_LeastLoaded (CORBA::Float critical_threshold,
                                        CORBA::Float reject_threshold,
                                        CORBA::Float tolerance,
                                        CORBA::Float dampening,
                                        CORBA::Float per_balance_load)
  : load_map_ (0),
    lock_ (0),
    critical_threshold_ (critical_threshold),
    reject_threshold_ (reject_threshold),
    tolerance_ (tolerance == 0 ? 1 : tolerance),
    dampening_ (dampening),
    per_balance_load_ (per_balance_load)
{
  // A load map that retains previous load values at a given location
  // and lock are only needed if dampening is enabled, i.e. non-zero.
  if (this->dampening_ != 0)
    {
      ACE_NEW (this->load_map_, TAO_LB_LoadMap (TAO_PG_MAX_LOCATIONS));

      ACE_NEW (this->lock_, TAO_SYNCH_MUTEX);
    }
}

TAO_LB_LeastLoaded::~TAO_LB_LeastLoaded (void)
{
  delete this->load_map_;
  delete this->lock_;
}

char *
TAO_LB_LeastLoaded::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("LeastLoaded");
}
    
CosLoadBalancing::Properties *
TAO_LB_LeastLoaded::get_properties (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosLoadBalancing::Properties * props = 0;
  ACE_NEW_THROW_EX (props,
                    CosLoadBalancing::Properties,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (props);

  return props;
}
    
void
TAO_LB_LeastLoaded::push_loads (
    const PortableGroup::Location & the_location,
    const CosLoadBalancing::LoadList & loads
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // Only the first load is used by this load balancing strategy.
  if (loads.length () == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  CosLoadBalancing::Load load;  // Unused

  this->push_loads (the_location,
                    loads,
                    load
                    ACE_ENV_ARG_PARAMETER);
}

void
TAO_LB_LeastLoaded::push_loads (
    const PortableGroup::Location & the_location,
    const CosLoadBalancing::LoadList & loads,
    CosLoadBalancing::Load & load
    ACE_ENV_ARG_DECL)
{
  if (loads.length () == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  // Only the first load is used by this load balancing strategy.
  const CosLoadBalancing::Load & new_load = loads[0];

  if (this->load_map_ != 0)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, *this->lock_);

      TAO_LB_LoadMap::ENTRY * entry;
      if (this->load_map_->find (the_location, entry) == 0)
        {
          CosLoadBalancing::Load & previous_load = entry->int_id_;

          if (previous_load.id != new_load.id)
            ACE_THROW (CORBA::BAD_PARAM ());  // Somebody switched
                                              // LoadIds on us!

          previous_load.value =
            this->effective_load (previous_load.value, new_load.value);

          load = previous_load;
        }
      else
        {
          const CosLoadBalancing::Load eff_load =
            {
              new_load.id,
              this->effective_load (0, new_load.value)
            };

          if (this->load_map_->bind (the_location, eff_load) != 0)
            {
              if (TAO_debug_level > 0)
                ACE_ERROR ((LM_ERROR,
                            "ERROR: TAO_LB_LeastLoaded - "
                            "Unable to push loads\n"));

              ACE_THROW (CORBA::INTERNAL ());
            }

          load = eff_load;
        }
    }
  else
    {
      load.id = new_load.id;
      load.value = this->effective_load (0, new_load.value);      
    }
}

CORBA::Object_ptr
TAO_LB_LeastLoaded::next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  if (CORBA::is_nil (load_manager))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (),
                      CORBA::Object::_nil ());

  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (locations->length () == 0)
    ACE_THROW_RETURN (CORBA::TRANSIENT (),
                      CORBA::Object::_nil ());

  // @@ RACE CONDITION.  OBJECT GROUP MEMBERSHIP MAY CHANGE AFTER
  //    RETRIEVING LOCATIONS!  HOW DO WE HANDLE THAT?

  PortableGroup::Location location;
  CORBA::Boolean found_location =
    this->get_location (object_group,
                        load_manager,
                        locations.in (),
                        location
                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  if (found_location)
    {
      return load_manager->get_member_ref (object_group,
                                           location
                                           ACE_ENV_ARG_PARAMETER);
    }
  else
    {
      // This should never occur.
      ACE_THROW_RETURN (CORBA::INTERNAL (),
                        CORBA::Object::_nil ());
    }
}

CORBA::Boolean
TAO_LB_LeastLoaded::get_location (
  PortableGroup::ObjectGroup_ptr object_group,
  const CosLoadBalancing::LoadManager_ptr load_manager,
  const PortableGroup::Locations & locations,
  PortableGroup::Location & location
  ACE_ENV_ARG_DECL)
{
  CORBA::Float min_load = 0;
  CORBA::ULong location_index = 0;
  CORBA::Boolean found_location = 0;

  const CORBA::ULong len = locations.length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Location & loc = locations[i];

      // Retrieve the load list for the location from the LoadManager
      // and push it to this Strategy's load processor.
      CosLoadBalancing::LoadList_var current_loads =
        load_manager->get_loads (loc
                                 ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      CosLoadBalancing::Load load;
      this->push_loads (loc,
                        current_loads.in (),
                        load
                        ACE_ENV_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      if (load.value < this->reject_threshold_
          && (i == 0 || load.value < min_load))
        {
          min_load = load.value;
          location_index = i;
          found_location = 1;
        }
      else if (load.value > this->critical_threshold_)
        {
          // The location is overloaded.  Perform load shedding by
          // informing the LoadAlert object associated with the member
          // at that location it should redirect client requests back
          // to the LoadManager.
          //
          // AMI is used to improve member selection times and overall
          // throughput since the LoadAlert object need not be alerted
          // synchronously.  In particular, the load alert can and
          // should be performed in parallel to the member selection.
          load_manager->enable_alert (object_group,
                                      loc
                                      ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
      else if (load.value <= this->critical_threshold_)
        {
          // The location is not overloaded
          load_manager->disable_alert (object_group,
                                       loc
                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (0);
        }
    }

  if (found_location)
    location = locations[location_index];

  return found_location;
}
