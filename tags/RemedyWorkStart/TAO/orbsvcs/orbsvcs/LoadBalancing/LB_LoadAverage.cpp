// $Id$

#include "orbsvcs/LoadBalancing/LB_LoadAverage.h"
#include "orbsvcs/LoadBalancing/LB_LoadMap.h"
#include "orbsvcs/LoadBalancing/LB_Random.h"

#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/debug.h"
#include "tao/ORB_Constants.h"

#include "ace/Null_Mutex.h"
#include "ace/OS_NS_string.h"

#if !defined (__ACE_INLINE__)
#include "orbsvcs/LoadBalancing/LB_LoadAverage.inl"
#endif /* defined INLINE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_LB_LoadAverage::TAO_LB_LoadAverage (PortableServer::POA_ptr poa)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    load_map_ (0),
    lock_ (0),
    properties_ (),
    tolerance_ (TAO_LB::LA_DEFAULT_TOLERANCE),
    dampening_ (TAO_LB::LA_DEFAULT_DAMPENING),
    per_balance_load_ (TAO_LB::LA_DEFAULT_DAMPENING)
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

TAO_LB_LoadAverage::~TAO_LB_LoadAverage (void)
{
  delete this->load_map_;
  delete this->lock_;
}

char *
TAO_LB_LoadAverage::name (void)
{
  return CORBA::string_dup ("LoadAverage");
}

CosLoadBalancing::Properties *
TAO_LB_LoadAverage::get_properties (void)
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
TAO_LB_LoadAverage::push_loads (
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
TAO_LB_LoadAverage::push_loads (
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
                ACE_ERROR ((LM_ERROR,
                            "ERROR: TAO_LB_LoadAverage - "
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
TAO_LB_LoadAverage::get_loads (CosLoadBalancing::LoadManager_ptr load_manager,
                               const PortableGroup::Location & the_location)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  CosLoadBalancing::LoadList_var loads =
    load_manager->get_loads (the_location);

  this->push_loads (the_location,
                    loads.in (),
                    loads[0]);

  return loads._retn ();
}


CORBA::Object_ptr
TAO_LB_LoadAverage::next_member (
    PortableGroup::ObjectGroup_ptr object_group,
    CosLoadBalancing::LoadManager_ptr load_manager)
{
  if (CORBA::is_nil (load_manager))
    throw CORBA::BAD_PARAM ();

  PortableGroup::Locations_var locations =
    load_manager->locations_of_members (object_group);

  if (locations->length () == 0)
    throw CORBA::TRANSIENT ();

  // @note The Random load balancing strategy is used since it is
  //       very lightweight and stateless.

  return TAO_LB_Random::_tao_next_member (object_group,
                                          load_manager,
                                          locations.in ());
}

void
TAO_LB_LoadAverage::analyze_loads (
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

  CosLoadBalancing::Load total_load = { 0, 0 };
  CosLoadBalancing::Load avg_load  = { 0, 0 };

  CosLoadBalancing::LoadList tmp (len);
  tmp.length (1);

  // Iterate through the entire location list to determine
  // the average load of all the locations
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

          // @@ Jai, please use the compound "+=" operator here.  It
          //    is more efficient in this case.
          total_load.value = total_load.value + load.value;

          tmp[i] = load;

          /*
          ACE_DEBUG ((LM_DEBUG,
                       "TOTAL == %f\n",
                       total_load.value));
          */
        }
      catch (const CosLoadBalancing::LocationNotFound&)
        {
          // no location found
          //
        }
    }

  avg_load.value = total_load.value / len;
/*
  ACE_DEBUG ((LM_DEBUG,
              "AVERAGE LOAD == %f\n",
              avg_load.value));
*/

  // Iterate through the entire location list to determine
  // the location where the load has to be shed.
  for (CORBA::ULong j = 0; j < len; ++j)
    {
      try
        {
          const PortableGroup::Location & loc = locations[j];

         /*
          ACE_DEBUG ((LM_DEBUG,
                       "EFFECTIVE_LOAD == %f\n"
                       "AVERAGE       == %f\n",
                       tmp[i].value,
                       avg_load.value));
         */

          if (tmp[j].value <= avg_load.value)
            {
              load_manager->disable_alert (loc);
            }
          else
            {
              CORBA::Float percent_diff =
                  (tmp[j].value / avg_load.value) - 1;

              if (ACE::is_equal (tmp[j].value, avg_load.value))
              {
                percent_diff = 0;
              }
/*
              ACE_DEBUG ((LM_DEBUG,
                          "ALERT LOC == %u"
                          "\tAVG LOAD == %f\n"
                          "\tLOAD == %f\n"
                          "\tPERCENT == %f\n",
                          i,
                          avg_load.value,
                          tmp[i].value,
                          percent_diff));
*/

              // @@ Jai, please correct the indentation on the below
              // conditional blocks so that they conform to the ACE
              // coding style.
              if (percent_diff <= TAO_LB::LA_DEFAULT_DIFF_AVERAGE_CUTOFF)
              {
                load_manager->disable_alert (loc);
              }
              else if ((percent_diff > TAO_LB::LA_DEFAULT_DIFF_AVERAGE_CUTOFF)
                       && (percent_diff < 1))
              {
/*
                ACE_DEBUG ((LM_DEBUG,
                            "%P --- ALERTING LOCATION %u\n",
                            i));
*/

                load_manager->enable_alert (loc);

              }

            }
        }
      catch (const CosLoadBalancing::LocationNotFound&)
        {
          // no location found
          //
        }
    }

}

PortableServer::POA_ptr
TAO_LB_LoadAverage::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

void
TAO_LB_LoadAverage::init (const PortableGroup::Properties & props)
{
  CORBA::Float tolerance = TAO_LB::LA_DEFAULT_TOLERANCE;
  CORBA::Float dampening = TAO_LB::LA_DEFAULT_DAMPENING;
  CORBA::Float per_balance_load = TAO_LB::LA_DEFAULT_PER_BALANCE_LOAD;

  const CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Property & property = props[i];
      if (ACE_OS::strcmp (property.nam[0].id.in (),
                          "org.omg.CosLoadBalancing.Strategy.LoadAverage.Tolerance") == 0)
        {
          this->extract_float_property (property,
                                        tolerance);

          // Valid tolerance values are greater than or equal to one.
          if (tolerance < 1)
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LoadAverage.Dampening") == 0)
        {
          this->extract_float_property (property,
                                        dampening);

          // Dampening range is [0,1).
          if (dampening < 0 || dampening >= 1)
            throw PortableGroup::InvalidProperty (property.nam, property.val);
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy.LoadAverage.PerBalanceLoad") == 0)
        {
          this->extract_float_property (property,
                                        per_balance_load);
        }
    }

  this->properties_ = props;

  this->tolerance_          = tolerance;
  this->dampening_          = dampening;
  this->per_balance_load_   = per_balance_load;

  /*
   ACE_DEBUG ((LM_DEBUG,
               "--------------------------------\n"
               "tolerance          = %f\n"
               "dampening          = %f\n"
               "per_balance_load   = %f\n"
               "--------------------------------\n",
               tolerance,
               dampening,
               per_balance_load));
  */
}

void
TAO_LB_LoadAverage::extract_float_property (
  const PortableGroup::Property & property,
  CORBA::Float & value)
{
  if (!(property.val >>= value))
    throw PortableGroup::InvalidProperty (property.nam, property.val);
}

TAO_END_VERSIONED_NAMESPACE_DECL
