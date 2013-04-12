// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/LoadBalancing/LB_LeastLoaded.h"
#include "orbsvcs/LoadBalancing/LB_LoadMap.h"
#include "orbsvcs/LoadBalancing/LB_Random.h"

#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/LoadBalancing/LB_LeastLoaded.inl"
#endif /* defined INLINE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

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
  if (!ACE::is_equal (this->dampening_, 0.0f))
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
TAO_LB_LeastLoaded::name (void)
{
  return CORBA::string_dup ("LeastLoaded");
}

CosLoadBalancing::Properties *
TAO_LB_LeastLoaded::get_properties (void)
{
  CosLoadBalancing::Properties * props = 0;
  ACE_NEW_THROW_EX (props,
                    CosLoadBalancing::Properties (this->properties_),
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO::VMCID,
                        ENOMEM),
                      CORBA::COMPLETED_NO));

  return props;
}

void
TAO_LB_LeastLoaded::push_loads (
    const PortableGroup::Location & the_location,
    const CosLoadBalancing::LoadList & loads)
{
  // Only the first load is used by this load balancing strategy.
  if (loads.length () == 0)
    throw CORBA::BAD_PARAM ();

  CosLoadBalancing::Load load;  // Unused

  this->push_loads (the_location,
                    loads,
                    load);
}

void
TAO_LB_LeastLoaded::push_loads (
    const PortableGroup::Location & the_location,
    const CosLoadBalancing::LoadList & loads,
    CosLoadBalancing::Load & load)
{
  if (loads.length () == 0)
    throw CORBA::BAD_PARAM ();

  // Only the first load is used by this load balancing strategy.
  const CosLoadBalancing::Load & new_load = loads[0];

  if (this->load_map_ != 0)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, *this->lock_);

      TAO_LB_LoadMap::ENTRY * entry = 0;
      if (this->load_map_->find (the_location, entry) == 0)
        {
          CosLoadBalancing::Load & previous_load = entry->int_id_;

          if (previous_load.id != new_load.id)
            throw CORBA::BAD_PARAM ();  // Somebody switched
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
                ORBSVCS_ERROR ((LM_ERROR,
                            "ERROR: TAO_LB_LeastLoaded - "
                            "Unable to push loads\n"));

              throw CORBA::INTERNAL ();
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
                               const PortableGroup::Location & the_location)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  CosLoadBalancing::LoadList_var loads =
    load_manager->get_loads (the_location);

  this->push_loads (the_location,
                    loads.in (),
                    loads.inout ()[0]);

  return loads._retn ();
}


CORBA::Object_ptr
TAO_LB_LeastLoaded::next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group);

  if (locations->length () == 0)
    throw CORBA::TRANSIENT ();

  // @@ RACE CONDITION.  OBJECT GROUP MEMBERSHIP MAY CHANGE AFTER
  //    RETRIEVING LOCATIONS!  HOW DO WE HANDLE THAT?

  PortableGroup::Location location;
  CORBA::Boolean found_location =
    this->get_location (load_manager,
                        locations.in (),
                        location);

  if (found_location)
    {
//       ORBSVCS_DEBUG ((LM_DEBUG,
//                   "RETURNING REFERENCE FOR LOCATION \"%s\"\n",
//                   location[0].id.in ()));

      return load_manager->get_member_ref (object_group,
                                           location);
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
                                              locations.in ());
    }
}

void
TAO_LB_LeastLoaded::analyze_loads (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group);

  if (locations->length () == 0)
    throw CORBA::TRANSIENT ();

  const CORBA::ULong len = locations->length ();

  // Iterate through the entire location list to determine which
  // locations require load to be shed.
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      try
        {
          const PortableGroup::Location & loc = locations[i];

          // Retrieve the load list for the location from the
          // LoadManager and push it to this Strategy's load
          // processor.
          CosLoadBalancing::LoadList_var current_loads =
            load_manager->get_loads (loc);

          CosLoadBalancing::Load load;
          this->push_loads (loc,
                            current_loads.in (),
                            load);
/*
           ORBSVCS_DEBUG ((LM_DEBUG,
                       "EFFECTIVE_LOAD == %f\n"
                       "CRITICAL       == %f\n"
                       "REJECT         == %f\n"
                       "DAMPENING      == %f\n",
                       load.value,
                       this->critical_threshold_,
                       this->reject_threshold_,
                       this->dampening_));
*/
          // Perform load rebalancing only if the critical threshold
          // was  set.
          if (!ACE::is_equal (this->critical_threshold_, 0.0f))
            {
              if (load.value > this->critical_threshold_)
                {
/*
                   ORBSVCS_DEBUG ((LM_DEBUG,
                               "%P --- ALERTING LOCATION %u\n",
                               i));
*/

                  // The location is overloaded.  Perform load
                  // shedding by informing the LoadAlert object
                  // associated with the member at that location it
                  // should redirect client requests back to the
                  // LoadManager.
                  load_manager->enable_alert (loc);
                }
              else
                {
                  // The location is not overloaded.  Disable load
                  // shedding at given location.
                  load_manager->disable_alert (loc);
                }
            }
        }
      catch (const CosLoadBalancing::LocationNotFound&)
        {
          // No load available for the requested location.  Try the
          // next location.
        }
    }
}

PortableServer::POA_ptr
TAO_LB_LeastLoaded::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CORBA::Boolean
TAO_LB_LeastLoaded::get_location (
  CosLoadBalancing::LoadManager_ptr load_manager,
  const PortableGroup::Locations & locations,
  PortableGroup::Location & location)
{
  CORBA::Float min_load = FLT_MAX;  // Start out with the largest
                                    // positive value.

  CORBA::ULong location_index = 0;
  CORBA::Boolean found_location = 0;
  CORBA::Boolean found_load = 0;

  const CORBA::ULong len = locations.length ();

  // Iterate through the entire location list to find the least loaded
  // of them.
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      try
        {
          const PortableGroup::Location & loc = locations[i];

          // Retrieve the load list for the location from the LoadManager
          // and push it to this Strategy's load processor.
          CosLoadBalancing::LoadList_var current_loads =
            load_manager->get_loads (loc);

          found_load = 1;

          CosLoadBalancing::Load load;
          this->push_loads (loc,
                            current_loads.in (),
                            load);
/*
           ORBSVCS_DEBUG ((LM_DEBUG,
                       "LOC = %u"
                       "\tC = %d"
                       "\treject = %f"
                       "\tload = %f\n"
                       "\tmin_load = %f\n",
                       i,
                       (this->reject_threshold_ == 0
                        || load.value < this->reject_threshold_),
                       this->reject_threshold_,
                       load.value,
                       min_load));
*/
          if ((ACE::is_equal (this->reject_threshold_, 0.0f)
               || load.value < this->reject_threshold_)
              && load.value < min_load)
            {
//               ORBSVCS_DEBUG ((LM_DEBUG,
//                           "**** LOAD == %f\n",
//                           load.value));

              if (i > 0 && !ACE::is_equal (load.value, 0.0f))
                {
                  /*
                    percent difference =
                      (min_load - load.value) / load.value
                      == (min_load / load.value) - 1

                      The latter form is used to avoid a potential
                      arithmetic overflow problem, such as when
                      (min_load - load.value) > FLT_MAX, assuming that
                      either load.value is negative and min_load is
                      positive, or vice versa.
                  */
                  const CORBA::Float percent_diff =
                    (min_load / load.value) - 1;

                  /*
                    A "thundering herd" phenomenon may occur when
                    location loads are basically the same (e.g. only
                    differ by a very small amount), where one object
                    group member ends up receiving the majority of
                    requests from different clients.  In order to
                    prevent a single object group member from
                    receiving such request bursts, one of two equally
                    loaded locations is chosen at random.  Thanks to
                    Carlos, Marina and Jody at ATD for coming up with
                    this solution to this form of the thundering herd
                    problem.

                    See the documentation for
                    TAO_LB::LL_DEFAULT_LOAD_PERCENT_DIFF_CUTOFF in
                    LB_LeastLoaded.h for additional information.
                  */
                  if (percent_diff <= TAO_LB::LL_DEFAULT_LOAD_PERCENT_DIFF_CUTOFF)
                    {
                      // Prevent integer arithmetic overflow.
                      const CORBA::Float NUM_MEMBERS = 2;

                      // n == 0:  Use previously selected location.
                      // n == 1:  Use current location.
                      const CORBA::ULong n =
                        static_cast<CORBA::ULong> (NUM_MEMBERS * ACE_OS::rand ()
                                         / (RAND_MAX + 1.0));

                      ACE_ASSERT (n == 0 || n == 1);

                      if (n == 1)
                        {
                          min_load = load.value;
                          location_index = i;
                          found_location = 1;

//                           ORBSVCS_DEBUG ((LM_DEBUG,
//                                       "** NEW MIN_LOAD == %f\n",
//                                       min_load));
                        }

//                       if (n == 0)
//                         ORBSVCS_DEBUG ((LM_DEBUG, "^^^^^ PREVIOUS LOCATION\n"));
//                       else
//                         ORBSVCS_DEBUG ((LM_DEBUG, "^^^^^ CURRENT LOCATION\n"));

                    }
                  else
                    {
                      min_load = load.value;
                      location_index = i;
                      found_location = 1;

//                       ORBSVCS_DEBUG ((LM_DEBUG,
//                                   "***** NEW MIN_LOAD == %f\n",
//                                   min_load));
                    }
                }
              else
                {
                  min_load = load.value;
                  location_index = i;
                  found_location = 1;

//                   ORBSVCS_DEBUG ((LM_DEBUG,
//                               "NEW MIN_LOAD == %f\n",
//                               min_load));
                }
            }

          // ORBSVCS_DEBUG ((LM_DEBUG, "NEW MIN_LOAD == %f\n", min_load));
        }
      catch (const CosLoadBalancing::LocationNotFound&)
        {
          // No load available for the requested location.  Try the
          // next location.
        }
    }

//   ORBSVCS_DEBUG ((LM_DEBUG,
//               "FOUND_LOAD     == %u\n"
//               "FOUND_LOCATION == %u\n",
//               found_load,
//               found_location));

  // If no loads were found, return without an exception to allow this
  // strategy to select a member using an alternative method
  // (e.g. random selection).
  if (found_load)
    {
      if (found_location)
      {
        //ORBSVCS_DEBUG ((LM_DEBUG, "LOCATED = %u\n", location_index));
        location = locations[location_index];
      }
      else if (!ACE::is_equal (this->reject_threshold_, 0.0f))
        throw CORBA::TRANSIENT ();

//       ORBSVCS_DEBUG ((LM_DEBUG, "LOCATION ID == %s\n", location[0].id.in ()));
    }

  return found_location;
}

void
TAO_LB_LeastLoaded::init (const PortableGroup::Properties & props)
{
  CORBA::Float critical_threshold =
    TAO_LB::LL_DEFAULT_CRITICAL_THRESHOLD;
  CORBA::Float reject_threshold = TAO_LB::LL_DEFAULT_REJECT_THRESHOLD;
  CORBA::Float tolerance = TAO_LB::LL_DEFAULT_TOLERANCE;
  CORBA::Float dampening = TAO_LB::LL_DEFAULT_DAMPENING;
  CORBA::Float per_balance_load = TAO_LB::LL_DEFAULT_PER_BALANCE_LOAD;

  const PortableGroup::Property * ct = 0;  // critical threshold property

  const CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Property & property = props[i];
      if (ACE_OS::strcmp (property.nam[0].id.in (),
                          "org.omg.CosLoadBalancing.Strategy.LeastLoaded.CriticalThreshold") == 0)
        {
          this->extract_float_property (property,
                                        critical_threshold);

          ct = &property;
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LeastLoaded.RejectThreshold") == 0)
        {
          this->extract_float_property (property,
                                        reject_threshold);
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LeastLoaded.Tolerance") == 0)
        {
          this->extract_float_property (property,
                                        tolerance);

          // Valid tolerance values are greater than or equal to one.
          if (tolerance < 1)
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LeastLoaded.Dampening") == 0)
        {
          this->extract_float_property (property,
                                        dampening);

          // Dampening range is [0,1).
          if (dampening < 0 || dampening >= 1)
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LeastLoaded.PerBalanceLoad") == 0)
        {
          this->extract_float_property (property,
                                        per_balance_load);
        }
    }

  if (!ACE::is_equal (critical_threshold, 0.0f)
      && !ACE::is_equal (reject_threshold, 0.0f)
      && critical_threshold <= reject_threshold)
    throw PortableGroup::InvalidProperty (ct->nam, ct->val);

  this->properties_ = props;

  this->critical_threshold_ = critical_threshold;
  this->reject_threshold_   = reject_threshold;
  this->tolerance_          = tolerance;
  this->dampening_          = dampening;
  this->per_balance_load_   = per_balance_load;
/*
   ORBSVCS_DEBUG ((LM_DEBUG,
               "--------------------------------\n"
               "critical_threshold = %f\n"
               "reject_threshold   = %f\n"
               "tolerance          = %f\n"
               "dampening          = %f\n"
               "per_balance_load   = %f\n"
               "--------------------------------\n",
               critical_threshold,
               reject_threshold,
               tolerance,
               dampening,
               per_balance_load));
*/
}

void
TAO_LB_LeastLoaded::extract_float_property (
  const PortableGroup::Property & property,
  CORBA::Float & value)
{
  if (!(property.val >>= value))
    throw PortableGroup::InvalidProperty (property.nam, property.val);
}

TAO_END_VERSIONED_NAMESPACE_DECL
