#include "LB_LoadManager.h"
#include "LB_MemberLocator.h"
#include "LB_conf.h"

#include "orbsvcs/PortableGroup/PG_Property_Utils.h"
#include "orbsvcs/PortableGroup/PG_conf.h"

#include "tao/ORB_Core.h"

//#include "ace/Auto_Ptr.h"


ACE_RCSID (LoadBalancing,
           LB_LoadManager,
           "$Id$")


TAO_LB_LoadManager::TAO_LB_LoadManager (void)
  : reactor_ (0),
    poa_ (),
    lock_ (),
    monitor_map_ (TAO_PG_MAX_LOCATIONS),
    object_group_manager_ (),
    property_manager_ (object_group_manager_),
    generic_factory_ (object_group_manager_, property_manager_),
    pull_handler_ (),
    timer_id_ (-1),
    lm_ref_ ()
{
  this->pull_handler_.initialize (&this->monitor_map_, this);
}

TAO_LB_LoadManager::~TAO_LB_LoadManager (void)
{
}

void
TAO_LB_LoadManager::push_loads (
    const PortableGroup::Location & /* the_location */,
    const CosLoadBalancing::LoadList & /* loads */
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
#if 0
  // Only the first load is used by this load balancing strategy.
  if (loads.length () == 0)
    ACE_THROW (CORBA::BAD_PARAM ());
#endif  /* 0 */

  ACE_THROW (CORBA::NO_IMPLEMENT ());
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
             this->lock_);

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
                    this->lock_,
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
             this->lock_);

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
    const PortableGroup::Properties &props
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
    const PortableGroup::Properties &overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
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
    const PortableGroup::Properties &props
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
    const PortableGroup::Properties &overrides
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
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
    const PortableGroup::Location &the_location,
    const char *type_id,
    const PortableGroup::Criteria &the_criteria
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
    const PortableGroup::Location &the_location,
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
    const PortableGroup::Location &the_location
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
    const PortableGroup::Location &the_location
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
TAO_LB_LoadManager::member (const PortableServer::ObjectId & oid
                            ACE_ENV_ARG_DECL)
{
  // Pass the cached loads to the balancing strategy so that the
  // strategy may choose which member to forward requests to next.

  PortableGroup::ObjectGroup_var object_group =
    this->object_group_manager_.object_group (oid);

  if (CORBA::is_nil (object_group.in ()))
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CORBA::Object::_nil ());

  PortableGroup::Properties_var properties =
    this->get_properties (object_group.in ()
                          ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // @todo Cache this property name.  No need to create it each time
  //       this method is called.
  PortableGroup::Name balancing_strategy_name (1);
  balancing_strategy_name.length (1);
  balancing_strategy_name[0].id =
    CORBA::string_dup ("org.omg.CosLoadBalancing.Strategy");

  PortableGroup::Value value;
  CosLoadBalancing::Strategy_var balancing_strategy;
  if (TAO_PG::get_property_value (balancing_strategy_name,
                                  properties.in (),
                                  value)
      && (value >>= balancing_strategy.inout ())
      && !CORBA::is_nil (balancing_strategy.in ()))
    {
      // @todo Cache this ObjectGroupManager reference.
      PortableGroup::ObjectGroupManager_var group_manager =
        this->object_group_manager_._this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      return balancing_strategy->next_member (object_group.in (),
                                              this->lm_ref_.in ()
                                              ACE_ENV_ARG_PARAMETER);
    }
  else
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::Object::_nil ());
    }
}

void
TAO_LB_LoadManager::init (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr root_poa
  ACE_ENV_ARG_DECL)
{
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

      this->reactor_ = orb->orb_core ()->reactor ();
    }

  if (CORBA::is_nil (this->lm_ref_.in ()))
    {
      this->lm_ref_ = this->_this (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}
