// -*- C++ -*-

#include "LB_LeastLoaded.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "tao/debug.h"

ACE_RCSID (LoadBalancing,
           LB_LeastLoaded,
           "$Id$")


#if !defined (__ACE_INLINE__)
#include "LB_LeastLoaded.inl"
#endif /* defined INLINE */


TAO_LB_LeastLoaded::TAO_LB_LeastLoaded (void)
  : load_map_ (TAO_PG_MAX_LOCATIONS),
    lock_ (),
    critical_threshold_ (0),
    reject_threshold_ (0),
    tolerance_ (1),
    dampening_ (0),
    per_balance_load_ (0)
{
}

TAO_LB_LeastLoaded::~TAO_LB_LeastLoaded (void)
{
}

char *
TAO_LB_LeastLoaded::name (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return CORBA::string_dup ("TAO_LB_LeastLoaded");
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
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::StrategyNotAdaptive))
{
  // Only the first load is used by this load balancing strategy.
  if (loads.length () == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  const CosLoadBalancing::Load & new_load = loads[0];

  TAO_LB_LoadMap::ENTRY * load;
  if (this->load_map_.find (the_location, load) == 0)
    {
      CosLoadBalancing::Load & previous_load = load->int_id_;

      if (previous_load.id != new_load.id)
        ACE_THROW (CORBA::BAD_PARAM ());  // Somebody switched LoadIds
                                          // on us!

      previous_load.value =
        this->effective_load (previous_load.value, new_load.value);
    }
  else
    {
      const CosLoadBalancing::Load eff_load =
        {
          new_load.id,
          this->effective_load (0, new_load.value);
        };

      if (this->load_map_.bind (the_location, eff_load) != 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "ERROR: TAO_LB_LeastLoaded - Unable to push loads\n"));

          ACE_THROW (CORBA::INTERNAL ());
        }
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
    this->get_location (locations.in (), location);

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

void
TAO_LB_LeastLoaded::analyze_loads (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  int send_load_advisory = 0;

  {
    ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);


    TAO_LB_Location_Map::iterator begin =
      location_map.begin ();

    TAO_LB_Location_Map::iterator end =
      location_map.end ();

    float s = 0;
    CORBA::ULong n = 0;
    TAO_LB_Location_Map::iterator i = begin;
    for ( ; i != end; ++i)
      {
        s += (*i)->int_id_.load_list[0].value;  // @@ Hard coded to
                                                //    get things
                                                //    going.
        n++;
      }

    float avg = (n == 0 ? s : s / n);
    float cl = proxy->current_load ();

    if (avg == 0)
      return;

    float relative_load = cl / avg;

    // @@ Ossama: Make the 1.5 factor adjustable, it is how much
    // dispersion we tolerate before starting to send advisories.
    if (relative_load > 1 + 1.5F / n)
      {
        proxy->has_high_load_ = 1;
        send_load_advisory = 2;  // 2 == Send high load advisory
      }

    if (send_load_advisory == 1 && relative_load < 1 + 0.9F / n)
      {
        proxy->has_high_load_ = 0;
        send_load_advisory = 1;  // 1 == Send nominal load advisory
      }
  }

  // @@ Ossama: no debug messages in production code, my fault....
  // ACE_DEBUG ((LM_DEBUG, "Load[%x] %f %f %f\n",
  //             proxy, cl, avg, relative_load));

  // @@ Ossama: Make the 1.5 factor adjustable, it is how much
  // dispersion we tolerate before starting to send advisories.
  if (send_load_advisory == 2)
    {
      proxy->control_->high_load_advisory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;  // We may not throw an exception, so explicitly return.
    }

  // @@ Ossama: notice that we wait until the load is signifcantly
  // lower before sending the nominal load advisory, it does not
  // matter that much because the replicas automatically restart after
  // rejecting one client....
  // @@ Ossama: make the 0.9 factor adjustable, at least at
  // construction time...
  if (send_load_advisory == 1)
    {
      proxy->control_->nominal_load_advisory (ACE_ENV_SINGLE_ARG_PARAMETE);
      ACE_CHECK;
    }

}

CORBA::Boolean
TAO_LB_LeastLoaded::get_location (
  const PortableGroup::Locations & locations,
  PortableGroup::Location & location)
{
  CORBA::Float min_load = 0;
  CORBA::ULong location_index = 0;
  CORBA::Boolean found_location = 0;

  const CORBA::ULong len = locations.length ();

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      0);

    for (CORBA::ULong i = 0; i < len; ++i)
      {
        // WRONG!!!  THIS IS LEAST LOADED, NOT MINIMUM DISPERSION!
        LoadMap::ENTRY * entry;
        if (this->load_map_.find (locations[i], entry) == 0
            && (i == 0 || entry.value < min_load))
          {
            min_load = entry->value;
            location_index = i;
            found_location = 1;
          }
      }
  }

  if (found_location)
    location = locations[location_index];

  return found_location;
}

CORBA::Object_ptr
TAO_LB_LeastLoaded::replica (
    TAO_LB_ObjectGroup_Map_Entry *entry
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  for ( ; ; )
    {
      ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                        guard,
                        entry->lock,
                        CORBA::Object::_nil ());

      if (entry->replica_infos.is_empty ())
        // @@ What do we do if the set is empty?
        ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                          CORBA::Object::_nil ());

      TAO_LB_ReplicaInfo_Set::iterator begin = entry->replica_infos.begin ();
      TAO_LB_ReplicaInfo_Set::iterator end = entry->replica_infos.end ();

      TAO_LB_ReplicaInfo_Set::iterator i = begin;
      TAO_LB_ReplicaInfo *replica_info = (*i);

      LoadBalancing::LoadList *d =
        replica_info->location_entry->load_list.ptr ();

      for (++i ; i != end; ++i)
        {
          LoadBalancing::LoadList *load =
            (*i)->location_entry->load_list.ptr ();

          // @@ Hardcode one load and don't bother checking the
          // LoadId, for now.  (just to get things going)
          if ((*d)[CORBA::ULong (0)].value > (*load)[CORBA::ULong (0)].value)
            {
              replica_info = *i;
              d = (*i)->location_entry->load_list.ptr ();
            }
        }

      // Before returning an object reference to the client
      // validate it first.
      CORBA::Object_ptr object = replica_info->replica.in ();

      {
        ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (entry->lock);

        ACE_GUARD_RETURN (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                          reverse_guard,
                          reverse_lock,
                          CORBA::Object::_nil ());

        // @@ Ossama: we should setup a timeout policy here...
        ACE_TRY
          {
            CORBA::Boolean non_existent =
              object->_non_existent (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK;
            if (!non_existent)
              {
                return CORBA::Object::_duplicate (object);
              }
          }
        ACE_CATCHANY
          {
            // @@ HACK!  Do the right thing!
            return CORBA::Object::_duplicate (object);
          }
        ACE_ENDTRY;
      }
    }
}

void
TAO_LB_LeastLoaded::analyze_loads (
  TAO_LB_Location_Map &location_map
  ACE_ENV_ARG_DECL)
{
  int send_load_advisory = 0;

  {
    ACE_MT (ACE_GUARD (TAO_SYNCH_MUTEX,
                       guard,
                       this->lock_));


    TAO_LB_Location_Map::iterator begin =
      location_map.begin ();

    TAO_LB_Location_Map::iterator end =
      location_map.end ();

    float s = 0;
    CORBA::ULong n = 0;
    TAO_LB_Location_Map::iterator i = begin;
    for ( ; i != end; ++i)
      {
        s += (*i)->int_id_.load_list[0].value;  // @@ Hard coded to
                                                //    get things
                                                //    going.
        n++;
      }

    float avg = (n == 0 ? s : s / n);
    float cl = proxy->current_load ();

    if (avg == 0)
      return;

    float relative_load = cl / avg;

    // @@ Ossama: Make the 1.5 factor adjustable, it is how much
    // dispersion we tolerate before starting to send advisories.
    if (relative_load > 1 + 1.5F / n)
      {
        proxy->has_high_load_ = 1;
        send_load_advisory = 2;  // 2 == Send high load advisory
      }

    if (send_load_advisory == 1 && relative_load < 1 + 0.9F / n)
      {
        proxy->has_high_load_ = 0;
        send_load_advisory = 1;  // 1 == Send nominal load advisory
      }
  }

  // @@ Ossama: no debug messages in production code, my fault....
  // ACE_DEBUG ((LM_DEBUG, "Load[%x] %f %f %f\n",
  //             proxy, cl, avg, relative_load));

  // @@ Ossama: Make the 1.5 factor adjustable, it is how much
  // dispersion we tolerate before starting to send advisories.
  if (send_load_advisory == 2)
    {
      proxy->control_->high_load_advisory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      return;  // We may not throw an exception, so explicitly return.
    }

  // @@ Ossama: notice that we wait until the load is signifcantly
  // lower before sending the nominal load advisory, it does not
  // matter that much because the replicas automatically restart after
  // rejecting one client....
  // @@ Ossama: make the 0.9 factor adjustable, at least at
  // construction time...
  if (send_load_advisory == 1)
    {
      proxy->control_->nominal_load_advisory (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}
