// -*- C++ -*-

#include "LB_LeastLoaded.h"
#include "LB_LoadMap.h"
#include "LB_Random.h"
#include "orbsvcs/PortableGroup/PG_conf.h"
#include "tao/debug.h"


ACE_RCSID (LoadBalancing,
           LB_LeastLoaded,
           "$Id$")


#if !defined (__ACE_INLINE__)
#include "LB_LeastLoaded.inl"
#endif /* defined INLINE */


TAO_LB_LeastLoaded::TAO_LB_LeastLoaded (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    load_map_ (0),
    lock_ (0),
    properties_ (),
    critical_threshold_ (TAO_LB::LL_DEFAULT_CRITICAL_THRESHOLD),
    reject_threshold_ (TAO_LB::LL_DEFAULT_REJECT_THRESHOLD),
    tolerance_ (TAO_LB::LL_DEFAULT_TOLERANCE),
    dampening_ (TAO_LB::LL_DEFAULT_DAMPENING),
    per_balance_load_ (TAO_LB::LL_DEFAULT_DAMPENING)
{
  // A load map that retains previous load values at a given location
  // and lock are only needed if dampening is enabled, i.e. non-zero.
  if (this->dampening_ != 0)
    {
      ACE_NEW (this->load_map_, TAO_LB_LoadMap (TAO_PG_MAX_LOCATIONS));

      ACE_NEW (this->lock_, TAO_SYNCH_MUTEX);
    }

  // Initialize the random load balancing strategy.
  TAO_LB_Random::init ();
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
                    CosLoadBalancing::Properties (this->properties_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

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

CosLoadBalancing::LoadList *
TAO_LB_LeastLoaded::get_loads (CosLoadBalancing::LoadManager_ptr load_manager,
                               const PortableGroup::Location & the_location
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LocationNotFound))
{
  if (CORBA::is_nil (load_manager))
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  CosLoadBalancing::LoadList_var loads =
    load_manager->get_loads (the_location
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  this->push_loads (the_location,
                    loads.in (),
                    loads[0]
                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (0);

  return loads._retn ();
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
    this->get_location (load_manager,
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
      // No loads have been reported for any of the locations the
      // object group members reside at.  If no loads have been
      // reported to the LoadManager, adaptive load balancing
      // decisions cannot be made.  Fall back on a non-adaptive
      // strategy, such as the Random load balancing strategy,
      // instead.
      //
      // @note The Random load balancing strategy is used since it is
      //       very lightweight and stateless.

      return TAO_LB_Random::_tao_next_member (object_group,
                                              load_manager,
                                              locations.in ()
                                              ACE_ENV_ARG_PARAMETER);
    }
}

void
TAO_LB_LeastLoaded::analyze_loads (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (CORBA::is_nil (load_manager))
    ACE_THROW (CORBA::BAD_PARAM ());

  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group
                                        ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (locations->length () == 0)
    ACE_THROW (CORBA::TRANSIENT ());

  const CORBA::ULong len = locations->length ();

  // Iterate through the entire location list to determine which
  // locations require load to be shed.
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      ACE_TRY
        {
          const PortableGroup::Location & loc = locations[i];

          // Retrieve the load list for the location from the
          // LoadManager and push it to this Strategy's load
          // processor.
          CosLoadBalancing::LoadList_var current_loads =
            load_manager->get_loads (loc
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          CosLoadBalancing::Load load;
          this->push_loads (loc,
                            current_loads.in (),
                            load
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

//           ACE_DEBUG ((LM_DEBUG,
//                       "EFFECTIVE_LOAD == %f\n"
//                       "CRITICAL       == %f\n"
//                       "REJECT         == %f\n"
//                       "DAMPENING      == %f\n",
//                       load.value,
//                       this->critical_threshold_,
//                       this->reject_threshold_,
//                       this->dampening_));

          // Perform load rebalancing only if the critical threshold
          // was  set.
          if (this->critical_threshold_ != 0)
            {
              if (load.value > this->critical_threshold_)
                {
//                   ACE_DEBUG ((LM_DEBUG,
//                               "%P --- ALERTING LOCATION %u\n",
//                               i));

                  // The location is overloaded.  Perform load
                  // shedding by informing the LoadAlert object
                  // associated with the member at that location it
                  // should redirect client requests back to the
                  // LoadManager.
                  load_manager->enable_alert (loc
                                              ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
              else
                {
                  // The location is not overloaded.  Disable load
                  // shedding at given location.
                  load_manager->disable_alert (loc
                                               ACE_ENV_ARG_PARAMETER);
                  ACE_TRY_CHECK;
                }
            }
        }
      ACE_CATCH (CosLoadBalancing::LocationNotFound, ex)
        {
          // No load available for the requested location.  Try the
          // next location.
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

PortableServer::POA_ptr
TAO_LB_LeastLoaded::_default_POA (ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CORBA::Boolean
TAO_LB_LeastLoaded::get_location (
  CosLoadBalancing::LoadManager_ptr load_manager,
  const PortableGroup::Locations & locations,
  PortableGroup::Location & location
  ACE_ENV_ARG_DECL)
{
  CORBA::Float min_load = FLT_MAX;  // Start out with the largest
                                    // possible.

  CORBA::ULong location_index = 0;
  CORBA::Boolean found_location = 0;
  CORBA::Boolean found_load = 0;

  const CORBA::ULong len = locations.length ();

  // Iterate through the entire location list to find the least loaded
  // of them.
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      ACE_TRY
        {
          const PortableGroup::Location & loc = locations[i];

          // Retrieve the load list for the location from the LoadManager
          // and push it to this Strategy's load processor.
          CosLoadBalancing::LoadList_var current_loads =
            load_manager->get_loads (loc
                                     ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          found_load = 1;

          CosLoadBalancing::Load load;
          this->push_loads (loc,
                            current_loads.in (),
                            load
                            ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

//           ACE_DEBUG ((LM_DEBUG,
//                       "LOC = %u"
//                       "\tCOND = %d"
//                       "\treject = %f"
//                       "\tload = %f\n",
//                       i,
//                       (this->reject_threshold_ == 0
//                        || load.value < this->reject_threshold_),
//                       this->reject_threshold_,
//                       load.value));

          if ((this->reject_threshold_ == 0
               || load.value < this->reject_threshold_)
              && load.value < min_load)
            {
//               ACE_DEBUG ((LM_DEBUG,
//                           "**** LOAD == %f\n",
//                           load.value));

              min_load = load.value;
              location_index = i;
              found_location = 1;
            }
        }
      ACE_CATCH (CosLoadBalancing::LocationNotFound, ex)
        {
          // No load available for the requested location.  Try the
          // next location.
        }
      ACE_ENDTRY;
      ACE_CHECK_RETURN (0);
    }

//   ACE_DEBUG ((LM_DEBUG,
//               "FOUND_LOAD     == %u\n"
//               "FOUND_LOCATION = %u\n",
//               found_load,
//               found_location));

  // If no loads were found, return without an exception to allow this
  // strategy to select a member using an alternative method
  // (e.g. random selection).
  if (found_load)
    {
      if (found_location)
        location = locations[location_index];
      else if (this->reject_threshold_ != 0)
        ACE_THROW_RETURN (CORBA::TRANSIENT (), 0);
    }

//   ACE_DEBUG ((LM_DEBUG, "LOCATED = %u\n", location_index));

  return found_location;
}

void
TAO_LB_LeastLoaded::init (const PortableGroup::Properties & props
                          ACE_ENV_ARG_DECL)
{
  CORBA::Float critical_threshold =
    TAO_LB::LL_DEFAULT_CRITICAL_THRESHOLD;
  CORBA::Float reject_threshold = TAO_LB::LL_DEFAULT_REJECT_THRESHOLD;
  CORBA::Float tolerance = TAO_LB::LL_DEFAULT_TOLERANCE;
  CORBA::Float dampening = TAO_LB::LL_DEFAULT_DAMPENING;
  CORBA::Float per_balance_load = TAO_LB::LL_DEFAULT_PER_BALANCE_LOAD;

  const PortableGroup::Property * ct = 0;  // critical threshold property
  const PortableGroup::Property * rt = 0;  // reject   threshold property

  const CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Property & property = props[i];
      if (ACE_OS::strcmp (property.nam[0].id.in (),
                          "org.omg.CosLoadBalancing.Strategy.LeastLoaded.CriticalThreshold") == 0)
        {
          this->extract_float_property (property,
                                        critical_threshold
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          ct = &property;
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LeastLoaded.RejectThreshold") == 0)
        {
          this->extract_float_property (property,
                                        reject_threshold
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          rt = &property;
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LeastLoaded.Tolerance") == 0)
        {
          this->extract_float_property (property,
                                        tolerance
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Valid tolerance values are greater than or equal to one.
          if (tolerance < 1)
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LeastLoaded.Dampening") == 0)
        {
          this->extract_float_property (property,
                                        dampening
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Dampening range is [0,1).
          if (dampening < 0 || dampening >= 1)
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LeastLoaded.PerBalanceLoad") == 0)
        {
          this->extract_float_property (property,
                                        per_balance_load
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }
    }

  if (critical_threshold != 0 && reject_threshold != 0
      && critical_threshold <= reject_threshold)
    ACE_THROW (PortableGroup::InvalidProperty (ct->nam, ct->val));

  this->properties_ = props;

  this->critical_threshold_ = critical_threshold;
  this->reject_threshold_   = reject_threshold;
  this->tolerance_          = tolerance;
  this->dampening_          = dampening;
  this->per_balance_load_   = per_balance_load;

//   ACE_DEBUG ((LM_DEBUG,
//               "--------------------------------\n"
//               "critical_threshold = %f\n"
//               "reject_threshold   = %f\n"
//               "tolerance          = %f\n"
//               "dampening          = %f\n"
//               "per_balance_load   = %f\n"
//               "--------------------------------\n",
//               critical_threshold,
//               reject_threshold,
//               tolerance,
//               dampening,
//               per_balance_load));
}

void
TAO_LB_LeastLoaded::extract_float_property (
  const PortableGroup::Property & property,
  CORBA::Float & value
  ACE_ENV_ARG_DECL)
{
  if (!(property.val >>= value))
    ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                               property.val));
}
