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

ACE_RCSID (LoadBalancing,
           LB_LoadManager,
           "$Id$")


TAO_LB_LoadManager::TAO_LB_LoadManager (void)
  : reactor_ (0),
    poa_ (),
    monitor_lock_ (),
    load_lock_ (),
    load_alert_lock_ (),
    lock_ (),
    monitor_map_ (TAO_PG_MAX_LOCATIONS),
    load_map_ (TAO_PG_MAX_LOCATIONS),
    load_alert_map_ (TAO_PG_MAX_OBJECT_GROUPS),
    object_group_manager_ (),
    property_manager_ (object_group_manager_),
    generic_factory_ (object_group_manager_, property_manager_),
    pull_handler_ (),
    timer_id_ (-1),
    lm_ref_ (),
    round_robin_ (0),
    random_ (0),
    least_loaded_ (0),
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
  delete this->round_robin_;
  delete this->random_;
  delete this->least_loaded_;
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

  ACE_GUARD (TAO_SYNCH_MUTEX,
             guard,
             this->load_lock_);

  if (this->load_map_.rebind (the_location, loads) == -1)
    ACE_THROW (CORBA::INTERNAL ());
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
TAO_LB_LoadManager::enable_alert (PortableGroup::ObjectGroup_ptr object_group,
                                  const PortableGroup::Location & the_location
                                  ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((PortableGroup::ObjectGroupNotFound,
                   CosLoadBalancing::LoadAlertNotFound))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->load_alert_lock_);

  // TAO's PortableGroup library only uses the lower 32 bits of the
  // ObjectGroupId, so this cast is safe.
  ACE_UINT32 group_id =
    ACE_static_cast (ACE_UINT32,
                     this->get_object_group_id (object_group
                                                ACE_ENV_ARG_PARAMETER));
  ACE_CHECK;

  TAO_LB_LoadAlertMap::ENTRY * entry;
  if (this->load_alert_map_.find (group_id, entry) == 0)
    {
      // A LoadAlert list for the given ObjectGroup exists.  Find the
      // reference to the LoadAlert object residing at the given
      // location.
      TAO_LB_LoadAlertInfoSet & alert_set = entry->int_id_;

      TAO_LB_LoadAlertInfoSet::iterator end =
        alert_set.end ();

      for (TAO_LB_LoadAlertInfoSet::iterator i = alert_set.begin ();
           i != end;
           ++i)
        {
          if ((*i).location == the_location)
            {
              // Duplicate before releasing the LoadAlertMap lock to
              // prevent a race condition from occuring.  The
              // LoadAlertInfoSet may be altered prior to invoking an
              // operation on the LoadAlert object.
              CosLoadBalancing::LoadAlert_var load_alert =
                CosLoadBalancing::LoadAlert::_duplicate (
                  (*i).load_alert.in ());

              // The alert condition will be enabled.
              // @note There is a subtle problem here.  If the below
              //       remote invocation fails, this variable will be
              //       incorrectly to "true."
              (*i).alerted = 1;

              // Release the lock prior to making the below remote
              // invocation.
              ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (
                this->load_alert_lock_);
              ACE_GUARD (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                         reverse_guard,
                         reverse_lock);

              // Use AMI to make the following operation
              // "non-blocking," allowing the caller to continue
              // without being forced to wait for a response.
              load_alert->sendc_enable_alert (this->load_alert_handler_.in (),
                                              object_group
                                              ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              return;
            }
        }

      ACE_THROW (CosLoadBalancing::LoadAlertNotFound ());
    }
  else
    {
      ACE_THROW (PortableGroup::ObjectGroupNotFound ());
    }
}

void
TAO_LB_LoadManager::disable_alert (PortableGroup::ObjectGroup_ptr object_group,
                                   const PortableGroup::Location & the_location
                                   ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((PortableGroup::ObjectGroupNotFound,
                   CosLoadBalancing::LoadAlertNotFound))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->load_alert_lock_);

  // TAO's PortableGroup library only uses the lower 32 bits of the
  // ObjectGroupId, so this cast is safe.
  ACE_UINT32 group_id =
    ACE_static_cast (ACE_UINT32,
                     this->get_object_group_id (object_group
                                                ACE_ENV_ARG_PARAMETER));
  ACE_CHECK;

  TAO_LB_LoadAlertMap::ENTRY * entry;
  if (this->load_alert_map_.find (group_id, entry) == 0)
    {
      // A LoadAlert list for the given ObjectGroup exists.  Find the
      // reference to the LoadAlert object residing at the given
      // location.
      TAO_LB_LoadAlertInfoSet & alert_set = entry->int_id_;

      TAO_LB_LoadAlertInfoSet::iterator end =
        alert_set.end ();

      for (TAO_LB_LoadAlertInfoSet::iterator i = alert_set.begin ();
           i != end;
           ++i)
        {
          // Only perform a remote invocation if the LoadAlert object
          // was previously alerted.
          if ((*i).location == the_location && (*i).alerted)
            {
              // Duplicate before releasing the LoadAlertMap lock to
              // prevent a race condition on the iterator from
              // occuring.  The LoadAlertInfoSet may be altered prior
              // to invoking an operation on the LoadAlert object.
              CosLoadBalancing::LoadAlert_var load_alert =
                CosLoadBalancing::LoadAlert::_duplicate (
                  (*i).load_alert.in ());

              // @note There is a subtle problem here.  If the below
              //       remote invocation fails, this variable will be
              //       incorrectly to "false."
              (*i).alerted = 0; // The alert condition will be disabled.

              // Release the lock prior to making the below remote
              // invocation.
              ACE_Reverse_Lock<TAO_SYNCH_MUTEX> reverse_lock (
                this->load_alert_lock_);
              ACE_GUARD (ACE_Reverse_Lock<TAO_SYNCH_MUTEX>,
                         reverse_guard,
                         reverse_lock);

              // Use AMI to make the following operation
              // "non-blocking," allowing the caller to continue
              // without being forced to wait for a response.
              load_alert->sendc_disable_alert (this->load_alert_handler_.in ()
                                               ACE_ENV_ARG_PARAMETER);
              ACE_CHECK;

              return;
            }
        }

      ACE_THROW (CosLoadBalancing::LoadAlertNotFound ());
    }
  else
    {
      ACE_THROW (PortableGroup::ObjectGroupNotFound ());
    }
}

void
TAO_LB_LoadManager::register_load_alert (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location,
    CosLoadBalancing::LoadAlert_ptr load_alert
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   CosLoadBalancing::LoadAlertAlreadyPresent,
                   CosLoadBalancing::LoadAlertNotAdded))
{
  if (CORBA::is_nil (load_alert))
    ACE_THROW (CORBA::BAD_PARAM ());

  // @todo Verify that a member of the given object group actually
  //       exists at "the_location."  It doesn't make much sense to
  //       register a LoadAlert object for a non-existent member.

  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->load_alert_lock_);

  // TAO's PortableGroup library only uses the lower 32 bits of the
  // ObjectGroupId, so this cast is safe.
  ACE_UINT32 group_id =
    ACE_static_cast (ACE_UINT32,
                     this->get_object_group_id (object_group
                                                ACE_ENV_ARG_PARAMETER));
  ACE_CHECK;

  TAO_LB_LoadAlertMap::ENTRY * entry;
  if (this->load_alert_map_.find (group_id, entry) == 0)
    {
      // A LoadAlert list for the given ObjectGroup exists.  Verify
      // that no LoadAlert object for the member residing at the given
      // location exists.

      TAO_LB_LoadAlertInfoSet & alert_set = entry->int_id_;

      TAO_LB_LoadAlertInfoSet::iterator end =
        alert_set.end ();

      for (TAO_LB_LoadAlertInfoSet::iterator i = alert_set.begin ();
           i != end;
           ++i)
        {
          if ((*i).location == the_location)
            ACE_THROW (CosLoadBalancing::LoadAlertAlreadyPresent ());
        }

      TAO_LB_LoadAlertInfo load_alert_info;
      load_alert_info.load_alert =
        CosLoadBalancing::LoadAlert::_duplicate (load_alert);
      load_alert_info.location = the_location;

      if (alert_set.insert_tail (load_alert_info) != 0)
        ACE_THROW (CosLoadBalancing::LoadAlertNotAdded ());
    }
  else
    {
      // No LoadAlert list exists for the given ObjectGroup.  Create
      // one and insert its initial member.

      TAO_LB_LoadAlertInfo load_alert_info;
      load_alert_info.load_alert =
        CosLoadBalancing::LoadAlert::_duplicate (load_alert);
      load_alert_info.location = the_location;

      TAO_LB_LoadAlertInfoSet alert_set;
      if (alert_set.insert_tail (load_alert_info) != 0
          || this->load_alert_map_.bind (group_id, alert_set) != 0)
        ACE_THROW (CosLoadBalancing::LoadAlertNotAdded ());
    }
}

CosLoadBalancing::LoadAlert_ptr
TAO_LB_LoadManager::get_load_alert (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   CosLoadBalancing::LoadAlertNotFound))
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    guard,
                    this->load_alert_lock_,
                    CosLoadBalancing::LoadAlert::_nil ());

  // TAO's PortableGroup library only uses the lower 32 bits of the
  // ObjectGroupId, so this cast is safe.
  ACE_UINT32 group_id =
    ACE_static_cast (ACE_UINT32,
                     this->get_object_group_id (object_group
                                                ACE_ENV_ARG_PARAMETER));
  ACE_CHECK_RETURN (CosLoadBalancing::LoadAlert::_nil ());

  TAO_LB_LoadAlertMap::ENTRY * entry;
  if (this->load_alert_map_.find (group_id, entry) == 0)
    {
      // A LoadAlert list for the given ObjectGroup exists.  Find the
      // LoadAlert object for the member residing at the given
      // location.

      TAO_LB_LoadAlertInfoSet & alert_set = entry->int_id_;

      TAO_LB_LoadAlertInfoSet::iterator end =
        alert_set.end ();

      for (TAO_LB_LoadAlertInfoSet::iterator i = alert_set.begin ();
           i != end;
           ++i)
        {
          if ((*i).location == the_location)
            return
              CosLoadBalancing::LoadAlert::_duplicate ((*i).load_alert.in ());
        }

      ACE_THROW_RETURN (CosLoadBalancing::LoadAlertNotFound (),
                        CosLoadBalancing::LoadAlert::_nil ());
    }
  else
    ACE_THROW_RETURN (PortableGroup::ObjectGroupNotFound (),
                      CosLoadBalancing::LoadAlert::_nil ());
}

void
TAO_LB_LoadManager::remove_load_alert (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location & the_location
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   CosLoadBalancing::LoadAlertNotFound))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->load_alert_lock_);

  // TAO's PortableGroup library only uses the lower 32 bits of the
  // ObjectGroupId, so this cast is safe.
  ACE_UINT32 group_id =
    ACE_static_cast (ACE_UINT32,
                     this->get_object_group_id (object_group
                                                ACE_ENV_ARG_PARAMETER));
  ACE_CHECK;

  TAO_LB_LoadAlertMap::ENTRY * entry;
  if (this->load_alert_map_.find (group_id, entry) == 0)
    {
      // A LoadAlert list for the given ObjectGroup exists.  Find the
      // LoadAlert object for the member residing at the given
      // location.

      TAO_LB_LoadAlertInfoSet & alert_set = entry->int_id_;

      TAO_LB_LoadAlertInfo alert_info;
      alert_info.location = the_location;
      // No need to set the LoadAlert member.  It isn't used in
      // comparisons.

      if (alert_set.remove (alert_info) != 0)
        ACE_THROW (CosLoadBalancing::LoadAlertNotFound ());
    }
  else
    ACE_THROW (PortableGroup::ObjectGroupNotFound ());

}

void
TAO_LB_LoadManager::register_load_monitor (
    CosLoadBalancing::LoadMonitor_ptr load_monitor,
    const PortableGroup::Location & the_location
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

          ACE_THROW (CORBA::INTERNAL ());
        }
    }
  else if (result == 1)
    ACE_THROW (CosLoadBalancing::MonitorAlreadyPresent ());
  else
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
  this->check_strategy_prop (props
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->property_manager_.set_default_properties (props
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
  this->check_strategy_prop (overrides
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->property_manager_.set_type_properties (type_id,
                                               overrides
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
  this->check_strategy_prop (overrides
                             ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  this->property_manager_.set_properties_dynamically (object_group,
                                                      overrides
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

  CORBA::Object_ptr obj =
    this->generic_factory_.create_object (type_id,
                                          the_criteria,
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
  CosLoadBalancing::Strategy_var balancing_strategy;
  const char * built_in;  // Name of built-in Strategy
  if ((TAO_PG::get_property_value (this->custom_balancing_strategy_name_,
                                   properties.in (),
                                   value)
       && (value >>= balancing_strategy.inout ())
       && !CORBA::is_nil (balancing_strategy.in ())))
    {
      return balancing_strategy->next_member (object_group.in (),
                                              this->lm_ref_.in ()
                                              ACE_ENV_ARG_PARAMETER);
    }
  else if (TAO_PG::get_property_value (this->built_in_balancing_strategy_name_,
                                       properties.in (),
                                       value)
           && (value >>= built_in))
    {
      balancing_strategy = this->built_in_strategy (built_in);
      if (!CORBA::is_nil (balancing_strategy.in ()))
        {
          return balancing_strategy->next_member (object_group.in (),
                                                  this->lm_ref_.in ()
                                                  ACE_ENV_ARG_PARAMETER);
        }
      else
        {
          ACE_THROW_RETURN (CORBA::INTERNAL (),
                            CORBA::Object::_nil ());
        }
    }
  else
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::Object::_nil ());
    }
}

void
TAO_LB_LoadManager::init (
  ACE_Reactor * reactor,
  PortableServer::POA_ptr root_poa
  ACE_ENV_ARG_DECL)
{
  ACE_ASSERT (reactor != 0);
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

      // Overwrite the last 8 characters in the POA name.
      ACE_OS::sprintf (astr, "%d", time);

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
    }

  if (CORBA::is_nil (this->lm_ref_.in ()))
    {
      this->lm_ref_ = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
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

  this->built_in_balancing_strategy_name_.length (1);
  this->built_in_balancing_strategy_name_[0].id =
    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy");

  this->custom_balancing_strategy_name_.length (1);
  this->custom_balancing_strategy_name_[0].id =
    CORBA::string_dup ("org.omg.CosLoadBalancing.CustomStrategy");
}

CosLoadBalancing::Strategy_ptr
TAO_LB_LoadManager::built_in_strategy (const char * strategy)
{
  ACE_ASSERT (strategy != 0);

  CosLoadBalancing::Strategy_ptr s;

  // @todo We should probably shove this code into a factory.

  if (ACE_OS::strcmp ("RoundRobin", strategy) == 0)
    {
      // Double-checked locking
      if (this->round_robin_ == 0)
        {
          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            0);

          if (this->round_robin_ == 0)
            {
              ACE_NEW_RETURN (this->round_robin_,
                              TAO_LB_RoundRobin,
                              CosLoadBalancing::Strategy::_nil ());
            }
        }

      s = this->round_robin_;
    }

  else if (ACE_OS::strcmp ("Random", strategy) == 0)
    {
      // Double-checked locking
      if (this->random_ == 0)
        {
          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            0);

          if (this->random_ == 0)
            {
              ACE_NEW_RETURN (this->random_,
                              TAO_LB_Random,
                              CosLoadBalancing::Strategy::_nil ());
            }
        }

      s = this->random_;
    }

  else if (ACE_OS::strcmp ("LeastLoaded", strategy) == 0)
    {
      // Double-checked locking
      if (this->least_loaded_ == 0)
        {
          ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                            guard,
                            this->lock_,
                            0);

          if (this->least_loaded_ == 0)
            {
              // Use default "LeastLoaded" properties.
              ACE_NEW_RETURN (this->least_loaded_,
                              TAO_LB_LeastLoaded,
                              CosLoadBalancing::Strategy::_nil ());
            }
        }

      s = this->least_loaded_;
    }

  else
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "ERROR: TAO_LB_LoadManager::next_member - "
                    "Unknown/unexpected built-in Strategy:\n"
                    "     \"%s\"\n",
                    strategy));

      // This should never occur!
      return CosLoadBalancing::Strategy::_nil ();
    }

  ACE_ASSERT (!CORBA::is_nil (s));

  return CosLoadBalancing::Strategy::_duplicate (s);
}

void
TAO_LB_LoadManager::check_strategy_prop (
  const PortableGroup::Properties & props
  ACE_ENV_ARG_DECL)
{
  const CORBA::ULong len = props.length ();
  for (CORBA::ULong i = 0; i < len; ++i)
    {
      const PortableGroup::Property & property = props[i];
      if (ACE_OS::strcmp (property.nam[0].id.in (),
                          "org.omg.CosLoadBalancing.CustomStrategy") == 0)
        {
          CosLoadBalancing::CustomStrategy_var strategy;
          if (!(property.val >>= strategy.inout ())
              || CORBA::is_nil (strategy.in ()))
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }

      else if (ACE_OS::strcmp (property.nam[0].id.in (),
                               "org.omg.CosLoadBalancing.Strategy") == 0)
        {
          CosLoadBalancing::StrategyInfo * info;
          if (property.val >>= info)
            {
              if (ACE_OS::strcmp (info->name.in (), "LeastLoaded") == 0)
                {
                  this->init_least_loaded (info->props
                                           ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }
            }
          else
            ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                                       property.val));
        }

    }
}

void
TAO_LB_LoadManager::init_least_loaded (const PortableGroup::Properties & props
                                       ACE_ENV_ARG_DECL)
{
  // Double-checked locking
  if (this->least_loaded_ == 0)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX, guard, this->lock_);

      if (this->least_loaded_ == 0)
        {
          CORBA::Float critical_threshold =
            TAO_LB::LL_DEFAULT_CRITICAL_THRESHOLD;
          CORBA::Float reject_threshold = TAO_LB::LL_DEFAULT_REJECT_THRESHOLD;
          CORBA::Float tolerance = TAO_LB::LL_DEFAULT_TOLERANCE;
          CORBA::Float dampening = TAO_LB::LL_DEFAULT_DAMPENING;
          CORBA::Float per_balance_load = TAO_LB::LL_DEFAULT_PER_BALANCE_LOAD;

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
                }

              else if (ACE_OS::strcmp (property.nam[0].id.in (),
                                       "org.omg.CosLoadBalancing.Strategy.LeastLoaded.RejectThreshold") == 0)
                {
                  this->extract_float_property (property,
                                                reject_threshold
                                                ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }

              else if (ACE_OS::strcmp (property.nam[0].id.in (),
                                       "org.omg.CosLoadBalancing.Strategy.LeastLoaded.Tolerance") == 0)
                {
                  this->extract_float_property (property,
                                                tolerance
                                                ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
                }

              else if (ACE_OS::strcmp (property.nam[0].id.in (),
                                       "org.omg.CosLoadBalancing.Strategy.LeastLoaded.Dampening") == 0)
                {
                  this->extract_float_property (property,
                                                dampening
                                                ACE_ENV_ARG_PARAMETER);
                  ACE_CHECK;
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

          // Use default "LeastLoaded" properties.
          ACE_NEW_THROW_EX (this->least_loaded_,
                            TAO_LB_LeastLoaded (critical_threshold,
                                                reject_threshold,
                                                tolerance,
                                                dampening,
                                                per_balance_load),
                            CORBA::NO_MEMORY (
                              CORBA::SystemException::_tao_minor_code (
                                TAO_DEFAULT_MINOR_CODE,
                                ENOMEM),
                              CORBA::COMPLETED_NO));
          ACE_CHECK;
        }
    }
}

void
TAO_LB_LoadManager::extract_float_property (
  const PortableGroup::Property & property,
  CORBA::Float & value
  ACE_ENV_ARG_DECL)
{
  if (!(property.val >>= value))
    ACE_THROW (PortableGroup::InvalidProperty (property.nam,
                                               property.val));
}
