#include "LB_LoadManager.h"
#include "LB_MemberLocator.h"
#include "LB_LoadAlert_Handler.h"
#include "LB_RoundRobin.h"
#include "LB_Random.h"
#include "LB_LeastLoaded.h"
#include "LB_conf.h"

#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/Messaging/Messaging.h"
#include "tao/debug.h"

#include "ace/Reactor.h"

ACE_RCSID (LoadBalancing,
           LB_LoadManager,
           "$Id$")


TAO_LB_LoadManager::TAO_LB_LoadManager (void)
  : reactor_ (0),
    poa_ (),
    root_poa_ (),
    monitor_lock_ (),
    load_lock_ (),
    load_alert_lock_ (),
    lock_ (),
    monitor_map_ (TAO_PG_MAX_LOCATIONS),
    load_map_ (TAO_PG_MAX_LOCATIONS),
    load_alert_map_ (TAO_PG_MAX_LOCATIONS),
    object_group_manager_ (),
    property_manager_ (object_group_manager_),
    generic_factory_ (object_group_manager_, property_manager_),
    pull_handler_ (),
    timer_id_ (-1),
    lm_ref_ (),
    round_robin_ (),
    random_ (),
    least_loaded_ (),
    built_in_balancing_strategy_info_name_ (1),
    built_in_balancing_strategy_name_ (1),
    custom_balancing_strategy_name_ (1)
{
  this->pull_handler_.initialize (&this->monitor_map_, this);

  // @note "this->init()" is not called here (in the constructor)
  //       since it may thrown an exception.  Throwing an exception in
  //       a constructor in an emulated exception environment is
  //       problematic since native exception semantics cannot be
  //       reproduced in such a case.  As such, init() must be called
  //       by whatever code instantiates this LoadManager.
}

TAO_LB_LoadManager::~TAO_LB_LoadManager (void)
{
}

void
TAO_LB_LoadManager::push_loads (
    const PortableGroup::Location & the_location,
    const CosLoadBalancing::LoadList & loads
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  if (loads.length () == 0)
    ACE_THROW (CORBA::BAD_PARAM ());

  {
    ACE_GUARD (TAO_SYNCH_MUTEX,
               guard,
               this->load_lock_);

    if (this->load_map_.rebind (the_location, loads) == -1)
      ACE_THROW (CORBA::INTERNAL ());
  }

  // Analyze loads for object groups that have members residing at the
  // given location.
  PortableGroup::ObjectGroups_var groups =
    this->object_group_manager_.groups_at_location (the_location
                                                    ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  const CORBA::ULong len = groups->length ();

  for (CORBA::ULong i = 0; i < len; ++i)
    {
      PortableGroup::ObjectGroup_ptr object_group =
        groups[i].in ();

      ACE_TRY
        {
          PortableGroup::Properties_var properties =
            this->get_properties (object_group
                                  ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          PortableGroup::Value value;
          CosLoadBalancing::Strategy_ptr strategy;

          if ((TAO_PG::get_property_value (
                 this->built_in_balancing_strategy_name_,
                 properties.in (),
                 value)
               || TAO_PG::get_property_value (
                    this->custom_balancing_strategy_name_,
                    properties.in (),
                    value))
              && (value >>= strategy)
              && !CORBA::is_nil (strategy))
            {
              strategy->analyze_loads (object_group,
                                       this->lm_ref_.in ()
                                       ACE_ENV_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }
        }
      ACE_CATCHANY
        {
          // Ignore all exceptions.
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

CosLoadBalancing::LoadList *
TAO_LB_LoadManager::get_loads (const PortableGroup::Location & the_location
                               ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LocationNotFound))
{
  CosLoadBalancing::LoadList * tmp;
  ACE_NEW_THROW_EX (tmp,
                    CosLoadBalancing::LoadList,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (0);

  CosLoadBalancing::LoadList_var loads = tmp;

  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->load_lock_,
                    0);

  if (this->load_map_.find (the_location, *tmp) == 0)
    return loads._retn ();
  else
    ACE_THROW_RETURN (CosLoadBalancing::LocationNotFound (), 0);
}

void
TAO_LB_LoadManager::enable_alert (const PortableGroup::Location & the_location
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CosLoadBalancing::LoadAlertNotFound))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->load_alert_lock_);

  TAO_LB_LoadAlertMap::ENTRY * entry;
  if (this->load_alert_map_.find (the_location, entry) == 0)
    {
      TAO_LB_LoadAlertInfo & info = entry->int_id_;

      // @note This could be problematic if the LoadAlert object is
      //       registered with more than LoadManager.

      if (info.alerted == 1)
        return;  // No need to set the alert status.  It has already
                 // been set.

      // Duplicate before releasing the LoadAlertMap lock to prevent a
      // race condition from occuring.  The LoadAlertInfo map may be
      // altered prior to invoking an operation on the LoadAlert
      // object.
      CosLoadBalancing::LoadAlert_var load_alert =
        CosLoadBalancing::LoadAlert::_duplicate (info.load_alert.in ());

      // The alert condition will be enabled.
      // @@ What happens if the below call fails?  This variable
      //    should be reset to zero!
      info.alerted = 1;

      {
        // Release the lock prior to making the below remote
        // invocation.
        ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (
          this->load_alert_lock_);
        ACE_GUARD (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                   reverse_guard,
                   reverse_lock);

        // Use AMI to make the following operation "non-blocking,"
        // allowing the caller to continue without being forced to
        // wait for a response.
        //
        // AMI is used to improve member selection times and overall
        // throughput since the LoadAlert object need not be alerted
        // synchronously.  In particular, the load alert can and
        // should be performed in parallel to other tasks, such as
        // member selection.
        load_alert->sendc_enable_alert (this->load_alert_handler_.in ()
                                        ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
    }
  else
    ACE_THROW (CosLoadBalancing::LoadAlertNotFound ());
}

void
TAO_LB_LoadManager::disable_alert (const PortableGroup::Location & the_location
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CosLoadBalancing::LoadAlertNotFound))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->load_alert_lock_);

  TAO_LB_LoadAlertMap::ENTRY * entry;
  if (this->load_alert_map_.find (the_location, entry) == 0)
    {
      TAO_LB_LoadAlertInfo & info = entry->int_id_;

      // @note This could be problematic if the LoadAlert object is
      //       registered with more than LoadManager.
      if (info.alerted == 0)
        return;  // No need to set the alert status.  It has already
                 // been set.

      // Duplicate before releasing the LoadAlertMap lock to prevent a
      // race condition from occuring.  The LoadAlertInfo map may be
      // altered prior to invoking an operation on the LoadAlert
      // object.
      CosLoadBalancing::LoadAlert_var load_alert =
        CosLoadBalancing::LoadAlert::_duplicate (info.load_alert.in ());

      // The alert condition will be disabled.
      // @@ What happens if the below call fails?  This variable
      //    should be reset to one!
      info.alerted = 0;

      {
        // Release the lock prior to making the below remote
        // invocation.
        ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (
          this->load_alert_lock_);
        ACE_GUARD (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                   reverse_guard,
                   reverse_lock);

        // Use AMI to make the following operation "non-blocking,"
        // allowing the caller to continue without being forced to
        // wait for a response.
        //
        // AMI is used to improve member selection times and overall
        // throughput since the LoadAlert object need not be alerted
        // synchronously.  In particular, the load alert can and
        // should be performed in parallel to other tasks, such as
        // member selection.
        load_alert->sendc_disable_alert (this->load_alert_handler_.in ()
                                         ACE_ENV_ARG_PARAMETER);
        ACE_CHECK;
      }
    }
  else
    ACE_THROW (CosLoadBalancing::LoadAlertNotFound ());
}

void
TAO_LB_LoadManager::register_load_alert (
    const PortableGroup::Location & the_location,
    CosLoadBalancing::LoadAlert_ptr load_alert
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LoadAlertAlreadyPresent,
                   CosLoadBalancing::LoadAlertNotAdded))
{
  if (CORBA::is_nil (load_alert))
    ACE_THROW (CORBA::BAD_PARAM ());

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->load_alert_lock_);

  TAO_LB_LoadAlertInfo info;
  info.load_alert = CosLoadBalancing::LoadAlert::_duplicate (load_alert);

  int result = this->load_alert_map_.bind (the_location, info);

  if (result == 1)
    {
      ACE_THROW (CosLoadBalancing::LoadAlertAlreadyPresent ());
    }
  else if (result == -1)
    {
      // Problems dude!
      ACE_THROW (CosLoadBalancing::LoadAlertNotAdded ());
    }
}

CosLoadBalancing::LoadAlert_ptr
TAO_LB_LoadManager::get_load_alert (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LoadAlertNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->load_alert_lock_,
                    CosLoadBalancing::LoadAlert::_nil ());

  TAO_LB_LoadAlertMap::ENTRY * entry;
  if (this->load_alert_map_.find (the_location, entry) == 0)
    {
      TAO_LB_LoadAlertInfo & info = entry->int_id_;

      return
        CosLoadBalancing::LoadAlert::_duplicate (info.load_alert.in ());
    }
  else
    {
      ACE_THROW_RETURN (CosLoadBalancing::LoadAlertNotFound (),
                        CosLoadBalancing::LoadAlert::_nil ());
    }
}

void
TAO_LB_LoadManager::remove_load_alert (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LoadAlertNotFound))
{
  // Disable the "alert" status on the LoadAlert object since it will
  // no longer be associated with the LoadManager.  In particular,
  // requests should be allowed through once again since there will be
  // no way to control the load shedding mechanism once the LoadAlert
  // object is no longer under the control of the LoadManager.
  this->disable_alert (the_location
                       ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->load_alert_lock_);

  if (this->load_alert_map_.unbind (the_location) != 0)
    {
      ACE_THROW (CosLoadBalancing::LoadAlertNotFound ());
    }
}

void
TAO_LB_LoadManager::register_load_monitor (
    const PortableGroup::Location & the_location,
    CosLoadBalancing::LoadMonitor_ptr load_monitor
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::MonitorAlreadyPresent))
{
  if (CORBA::is_nil (load_monitor))
    ACE_THROW (CORBA::BAD_PARAM ());

  const CosLoadBalancing::LoadMonitor_var the_monitor =
    CosLoadBalancing::LoadMonitor::_duplicate (load_monitor);

  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->monitor_lock_);

  int result = this->monitor_map_.bind (the_location, the_monitor);

  if (result == 0
      && this->monitor_map_.current_size () == 1)
    {
      // Register the "pull monitoring" event handler only after the
      // first load monitor is registered.  This is an optimization to
      // prevent unnecessary invocation of the "pull monitoring" event
      // handler.
      ACE_Time_Value interval (TAO_LB_PULL_HANDLER_INTERVAL, 0);
      ACE_Time_Value restart (TAO_LB_PULL_HANDLER_RESTART, 0);
      this->timer_id_ = this->reactor_->schedule_timer (&this->pull_handler_,
                                                        0,
                                                        interval,
                                                        restart);

      if (this->timer_id_ == -1)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "TAO_LB_LoadManager::register_load_monitor: "
                        "Unable to schedule timer.\n"));

          (void) this->monitor_map_.unbind (the_location);

          ACE_THROW (CORBA::INTERNAL ());
        }
    }
  else if (result == 1)
    {
      ACE_THROW (CosLoadBalancing::MonitorAlreadyPresent ());
    }
  else if (result != 0)
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "TAO_LB_LoadManager::register_load_monitor: "
                    "Unable to register load monitor.\n"));

      ACE_THROW (CORBA::INTERNAL ());
    }
}

CosLoadBalancing::LoadMonitor_ptr
TAO_LB_LoadManager::get_load_monitor (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LocationNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->monitor_lock_,
                    CosLoadBalancing::LoadMonitor::_nil ());

  TAO_LB_MonitorMap::ENTRY * entry;
  if (this->monitor_map_.find (the_location, entry) == 0)
    {
      return
        CosLoadBalancing::LoadMonitor::_duplicate (entry->int_id_.in ());
    }

  ACE_THROW_RETURN (CosLoadBalancing::LocationNotFound (),
                    CosLoadBalancing::LoadMonitor::_nil ());
}

void
TAO_LB_LoadManager::remove_load_monitor (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosLoadBalancing::LocationNotFound))
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->monitor_lock_);

  if (this->monitor_map_.unbind (the_location) != 0)
    ACE_THROW (CosLoadBalancing::LocationNotFound ());

  // If no load monitors are registered with the load balancer than
  // shutdown the "pull monitoring."
  if (this->timer_id_ != -1
      && this->monitor_map_.current_size () == 0)
    {
      if (this->reactor_->cancel_timer (this->timer_id_) == 0)
        {
          if (TAO_debug_level > 0)
            ACE_ERROR ((LM_ERROR,
                        "TAO_LB_LoadManager::remove_load_monitor: "
                        "Unable to cancel timer.\n"));

          ACE_THROW (CORBA::INTERNAL ());
        }

      this->timer_id_ = -1;
    }
}

void
TAO_LB_LoadManager::set_default_properties (
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  PortableGroup::Properties new_props (props);
  this->preprocess_properties (new_props
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->property_manager_.set_default_properties (new_props
                                                  ACE_ENV_ARG_PARAMETER);
}

PortableGroup::Properties *
TAO_LB_LoadManager::get_default_properties (
    ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_default_properties (
      ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
TAO_LB_LoadManager::remove_default_properties (
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.remove_default_properties (props
                                                     ACE_ENV_ARG_PARAMETER);
}

void
TAO_LB_LoadManager::set_type_properties (
    const char *type_id,
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  PortableGroup::Properties new_overrides (overrides);
  this->preprocess_properties (new_overrides
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->property_manager_.set_type_properties (type_id,
                                               new_overrides
                                               ACE_ENV_ARG_PARAMETER);
}

PortableGroup::Properties *
TAO_LB_LoadManager::get_type_properties (
    const char *type_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_type_properties (type_id
                                                 ACE_ENV_ARG_PARAMETER);
}

void
TAO_LB_LoadManager::remove_type_properties (
    const char *type_id,
    const PortableGroup::Properties & props
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.remove_type_properties (type_id,
                                                  props
                                                  ACE_ENV_ARG_PARAMETER);
}

void
TAO_LB_LoadManager::set_properties_dynamically (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Properties & overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  PortableGroup::Properties new_overrides (overrides);
  this->preprocess_properties (new_overrides
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->property_manager_.set_properties_dynamically (object_group,
                                                      new_overrides
                                                      ACE_ENV_ARG_PARAMETER);
}

PortableGroup::Properties *
TAO_LB_LoadManager::get_properties (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->property_manager_.get_properties (object_group
                                            ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroup_ptr
TAO_LB_LoadManager::create_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    const char * type_id,
    const PortableGroup::Criteria & the_criteria
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::CannotMeetCriteria))
{
  return
    this->object_group_manager_.create_member (object_group,
                                               the_location,
                                               type_id,
                                               the_criteria
                                               ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroup_ptr
TAO_LB_LoadManager::add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CORBA::Object_ptr member
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::ObjectNotAdded))
{
  return
    this->object_group_manager_.add_member (object_group,
                                            the_location,
                                            member
                                            ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroup_ptr
TAO_LB_LoadManager::remove_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  return
    this->object_group_manager_.remove_member (object_group,
                                               the_location
                                               ACE_ENV_ARG_PARAMETER);
}

PortableGroup::Locations *
TAO_LB_LoadManager::locations_of_members (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.locations_of_members (object_group
                                                      ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroups *
TAO_LB_LoadManager::groups_at_location (
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->object_group_manager_.groups_at_location (the_location
                                                    ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroupId
TAO_LB_LoadManager::get_object_group_id (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_id (object_group
                                                     ACE_ENV_ARG_PARAMETER);
}

PortableGroup::ObjectGroup_ptr
TAO_LB_LoadManager::get_object_group_ref (
    PortableGroup::ObjectGroup_ptr object_group
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_ref (object_group
                                                      ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_LB_LoadManager::get_member_ref (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  return
    this->object_group_manager_.get_member_ref (object_group,
                                                the_location
                                                ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_LB_LoadManager::create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out
      factory_creation_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria))
{
//   this->init (ACE_ENV_SINGLE_ARG_PARAMETER);
//   ACE_CHECK_RETURN (CORBA::Object::_nil ());


  PortableGroup::Criteria new_criteria (the_criteria);
  this->preprocess_properties (new_criteria
                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::Object_ptr obj =
    this->generic_factory_.create_object (type_id,
                                          new_criteria,
                                          factory_creation_id
                                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());


  return obj;
}

#if 0
void
TAO_LB_LoadManager::process_criteria (
  const PortableGroup::Criteria & the_criteria
  ACE_ENV_ARG_DECL)
{
  // List of invalid criteria.  If this list has a length greater than
  // zero, then the PortableGroup::InvalidCriteria exception will
  // be thrown.
  PortableGroup::Criteria invalid_criteria;

  int found_factory = 0; // If factory was found in the_criteria, then
                         // set to 1.

  // Parse the criteria.
  CORBA::ULong criteria_count = the_criteria.length ();
  for (CORBA::ULong i = 0; i < criteria_size; ++i)
    {
      CORBA::UShort initial_number_replicas = 0;
      PortableGroup::FactoryInfos factory_infos;

      // Obtain the InitialNumberMembers from the_criteria.
      if (this->get_initial_number_replicas (type_id,
                                             the_criteria[i],
                                             initial_number_replicas) != 0)
        {
          CORBA::ULong len = invalid_criteria.length ();
          invalid_criteria.length (len + 1);
          invalid_criteria[len] = the_criteria[i];
        }

      // Obtain the FactoryInfos from the_criteria.  This method also
      // ensures that GenericFactories at different locations are used.
      else if (this->get_factory_infos (type_id,
                                        the_criteria[i],
                                        factory_infos) == 0)
        found_factory = 1;

      // Unknown property
      else
        ACE_THROW (PortableGroup::InvalidProperty (the_criteria[i].nam,
                                                   the_criteria[i].val));
    }

  if (invalid_criteria.length () != 0)
    ACE_THROW (PortableGroup::InvalidCriteria (invalid_criteria));

  if (found_factory == 0)
    ACE_THROW (PortableGroup::NoFactory ());
}
#endif  /* 0 */

void
TAO_LB_LoadManager::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      factory_creation_id
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  this->generic_factory_.delete_object (factory_creation_id
                                        ACE_ENV_ARG_PARAMETER);
}

CORBA::Object_ptr
TAO_LB_LoadManager::next_member (const PortableServer::ObjectId & oid
                                 ACE_ENV_ARG_DECL)
{
  PortableGroup::ObjectGroup_var object_group =
    this->object_group_manager_.object_group (oid);

  if (CORBA::is_nil (object_group.in ()))
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CORBA::Object::_nil ());

  PortableGroup::Properties_var properties =
    this->get_properties (object_group.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Prefer custom load balancing strategies over built-in ones.
  PortableGroup::Value value;
  CosLoadBalancing::Strategy_ptr strategy;

  if ((TAO_PG::get_property_value (this->built_in_balancing_strategy_name_,
                                   properties.in (),
                                   value)
       || TAO_PG::get_property_value (this->custom_balancing_strategy_name_,
                                      properties.in (),
                                      value))
       && (value >>= strategy)
       && !CORBA::is_nil (strategy))
    {
      return strategy->next_member (object_group.in (),
                                    this->lm_ref_.in ()
                                    ACE_ENV_ARG_PARAMETER);
    }

  ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                    CORBA::Object::_nil ());
}

void
TAO_LB_LoadManager::init (ACE_Reactor * reactor,
                          CORBA::ORB_ptr orb,
                          PortableServer::POA_ptr root_poa
                          ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (!CORBA::is_nil (orb));
  ACE_ASSERT (!CORBA::is_nil (root_poa));

  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->lock_);

  if (CORBA::is_nil (this->poa_.in ()))
    {
      // Create a new transient servant manager object in the child
      // POA.
      PortableServer::ServantManager_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO_LB_MemberLocator (this),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK;

      PortableServer::ServantManager_var member_locator = tmp;

      // Create the appropriate RequestProcessingPolicy
      // (USE_SERVANT_MANAGER) and ServantRetentionPolicy (NON_RETAIN)
      // for a ServantLocator.
      PortableServer::RequestProcessingPolicy_var request =
        root_poa->create_request_processing_policy (
          PortableServer::USE_SERVANT_MANAGER
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      PortableServer::ServantRetentionPolicy_var retention =
        root_poa->create_servant_retention_policy (
          PortableServer::NON_RETAIN
          ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      // Create the PolicyList containing the policies necessary for
      // the POA to support ServantLocators.
      CORBA::PolicyList policy_list;
      policy_list.length (2);
      policy_list[0] =
        PortableServer::RequestProcessingPolicy::_duplicate (
          request.in ());
      policy_list[1] =
        PortableServer::ServantRetentionPolicy::_duplicate (
           retention.in ());

      // Create the child POA with the above ServantManager policies.
      // The ServantManager will be the MemberLocator.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // The child POA's name will consist of a string that includes
      // the current time in milliseconds in hexidecimal format (only
      // four bytes will be used).  This is an attempt to prevent
      // different load manager servants within the same ORB from
      // using the same POA.
      const ACE_Time_Value tv = ACE_OS::gettimeofday ();
      const CORBA::Long time =
        ACE_static_cast (CORBA::Long,
                         tv.msec ()); // Time in milliseconds.

      char poa_name[] = "TAO_LB_LoadManager_POA - 0xZZZZZZZZ";
      char * astr =
        poa_name
        + sizeof (poa_name)
        - 9 /* 8 + 1 */;

      // Overwrite the last 8 characters in the POA name with the
      // hexadecimal representation of the time in milliseconds.
      ACE_OS::sprintf (astr, "%x", time);

      this->poa_ = root_poa->create_POA (poa_name,
                                         poa_manager.in (),
                                         policy_list
                                         ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      request->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      retention->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Now set the MemberLocator as the child POA's Servant
      // Manager.
      this->poa_->set_servant_manager (member_locator.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      this->object_group_manager_.poa (this->poa_.in ());
      this->generic_factory_.poa (this->poa_.in ());

      // Activate the child POA.
      poa_manager->activate (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      this->reactor_ = reactor;
      this->root_poa_ = PortableServer::POA::_duplicate (root_poa);
    }

  if (CORBA::is_nil (this->lm_ref_.in ()))
    {
      this->lm_ref_ = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      orb->register_initial_reference ("LoadManager",
                                       this->lm_ref_.in ()
                                       ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (CORBA::is_nil (this->load_alert_handler_.in ()))
    {
      TAO_LB_LoadAlert_Handler * handler;
      ACE_NEW_THROW_EX (handler,
                        TAO_LB_LoadAlert_Handler,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK;

      PortableServer::ServantBase_var safe_handler = handler;

      this->load_alert_handler_ =
        handler->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  this->built_in_balancing_strategy_info_name_.length (1);
  this->built_in_balancing_strategy_info_name_[0].id =
    CORBA::string_dup ("org.omg.CosLoadBalancing.StrategyInfo");

  this->built_in_balancing_strategy_name_.length (1);
  this->built_in_balancing_strategy_name_[0].id =
    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy");

  this->custom_balancing_strategy_name_.length (1);
  this->custom_balancing_strategy_name_[0].id =
    CORBA::string_dup ("org.omg.CosLoadBalancing.CustomStrategy");
}

void
TAO_LB_LoadManager::preprocess_properties (PortableGroup::Properties & props
                                           ACE_ENV_ARG_DECL)
{
  // @@ This is slow.  Optimize this code.

  const CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      PortableGroup::Property & property = props[i];
      if (property.nam == this->custom_balancing_strategy_name_)
        {
          CosLoadBalancing::CustomStrategy_ptr strategy;
          if (!(property.val >>= strategy)
              || CORBA::is_nil (strategy))
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }

      else if (property.nam == this->built_in_balancing_strategy_info_name_)
        {
          CosLoadBalancing::StrategyInfo * info;

          if (property.val >>= info)
            {
              // Convert the property from a "StrategyInfo" property
              // to a "Strategy" property.

              CosLoadBalancing::Strategy_var strategy =
                this->make_strategy (info
                                     ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              if (!CORBA::is_nil (strategy.in ()))
                {
                  property.nam = this->built_in_balancing_strategy_name_;

                  property.val <<= strategy.in ();
                }
              else
                ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                           property.val));
            }
          else
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }
      else if (property.nam == this->built_in_balancing_strategy_name_)
        {
          // It is illegal to set the Strategy property externally.
          ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                     property.val));
        }
    }
}

CosLoadBalancing::Strategy_ptr
TAO_LB_LoadManager::make_strategy (CosLoadBalancing::StrategyInfo * info
                                   ACE_ENV_ARG_DECL)
{
  /**
   * @todo We need a strategy factory.  This is just too messy.
   */

  if (ACE_OS::strcmp (info->name.in (), "RoundRobin") == 0)
    {
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                          monitor,
                          this->lock_,
                          CosLoadBalancing::Strategy::_nil ());

        if (CORBA::is_nil (this->round_robin_.in ()))
          {
            TAO_LB_RoundRobin * rr_servant;
            ACE_NEW_THROW_EX (rr_servant,
                              TAO_LB_RoundRobin (this->root_poa_.in ()),
                              CORBA::NO_MEMORY ());
            ACE_CHECK_RETURN (CosLoadBalancing::Strategy::_nil ());

            PortableServer::ServantBase_var s = rr_servant;

            this->round_robin_ =
              rr_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK_RETURN (CosLoadBalancing::Strategy::_nil ());
          }
      }

      return CosLoadBalancing::Strategy::_duplicate (this->round_robin_.in ());
    }

  else if (ACE_OS::strcmp (info->name.in (), "Random") == 0)
    {
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                          monitor,
                          this->lock_,
                          CosLoadBalancing::Strategy::_nil ());

        if (CORBA::is_nil (this->random_.in ()))
          {
            TAO_LB_Random * rnd_servant;
            ACE_NEW_THROW_EX (rnd_servant,
                              TAO_LB_Random (this->root_poa_.in ()),
                              CORBA::NO_MEMORY ());
            ACE_CHECK_RETURN (CosLoadBalancing::Strategy::_nil ());

            PortableServer::ServantBase_var s = rnd_servant;

            this->random_ =
              rnd_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_CHECK_RETURN (CosLoadBalancing::Strategy::_nil ());
          }
      }

      return CosLoadBalancing::Strategy::_duplicate (this->random_.in ());
    }

  else if (ACE_OS::strcmp (info->name.in (), "LeastLoaded") == 0)
    {
      // If no LeastLoaded properties have been set, just use the
      // default/cached LeastLoaded instance.  Otherwise create and
      // return a new LeastLoaded instance with the appropriate
      // properties.

      if (info->props.length () == 0)
        {
          {
            ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                              monitor,
                              this->lock_,
                              CosLoadBalancing::Strategy::_nil ());

            if (CORBA::is_nil (this->least_loaded_.in ()))
              {
                TAO_LB_LeastLoaded * ll_servant;
                ACE_NEW_THROW_EX (ll_servant,
                                  TAO_LB_LeastLoaded (this->root_poa_.in ()),
                                  CORBA::NO_MEMORY ());
                ACE_CHECK_RETURN (CosLoadBalancing::Strategy::_nil ());

                PortableServer::ServantBase_var s = ll_servant;

                this->least_loaded_ =
                  ll_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
                ACE_CHECK_RETURN (CosLoadBalancing::Strategy::_nil ());
              }
          }

          return
            CosLoadBalancing::Strategy::_duplicate (this->least_loaded_.in ());
        }
      else
        {
          TAO_LB_LeastLoaded * ll_servant;
          ACE_NEW_THROW_EX (ll_servant,
                            TAO_LB_LeastLoaded (this->root_poa_.in ()),
                            CORBA::NO_MEMORY ());
          ACE_CHECK_RETURN (CosLoadBalancing::Strategy::_nil ());

          PortableServer::ServantBase_var s = ll_servant;

          ll_servant->init (info->props
                            ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CosLoadBalancing::Strategy::_nil ());

          return ll_servant->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
        }
    }

  return CosLoadBalancing::Strategy::_nil ();
}

// void
// TAO_LB_LoadManager::update_strategy ()
// {
// }

// void
// TAO_LB_LoadManager::deactivate_strategy (ACE_ENV_ARG_DECL)
// {
//   PortableServer::ObjectId_var oid =
//     this->poa_->reference_to_id (
//   this->poa_->deactivate_object ();
// }
