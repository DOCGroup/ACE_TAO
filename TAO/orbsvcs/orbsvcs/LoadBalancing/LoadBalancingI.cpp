// -*- C++ -*-
//
// $Id$

#include "LoadBalancingI.h"

#include "LB_ReplicaLocator.h"
#include "LB_Balancing_Strategy.h"

#include "LB_Minimum_Dispersion.h"  // @@ REMOVE ME!

#include "tao/ORB_Core.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (LoadBalancing,
           LoadBalancingI,
           "$Id$")


// Implementation skeleton constructor
TAO_LoadBalancing_ReplicationManager_i::TAO_LoadBalancing_ReplicationManager_i
(void)
  : orb_ (),
    poa_ (),
    lock_ (),
    location_map_ (),
    object_group_map_ (),
    property_manager_ (this->object_group_map_),
    generic_factory_ (this->location_map_,
                      this->object_group_map_,
                      this->property_manager_),
    object_group_manager_ (this->location_map_,
                           this->object_group_map_,
                           this->property_manager_),
   balancing_strategy_ (new TAO_LB_Minimum_Dispersion_Strategy),
   pull_handler_ (this->location_map_)
{
  //  (void) this->init ();
}

// Implementation skeleton destructor
TAO_LoadBalancing_ReplicationManager_i::~TAO_LoadBalancing_ReplicationManager_i (void)
{
  delete this->balancing_strategy_; // @@ REMOVE ME!
}

void
TAO_LoadBalancing_ReplicationManager_i::register_load_notifier (
    LoadBalancing::LoadNotifier_ptr /* load_notifier */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

LoadBalancing::LoadNotifier_ptr
TAO_LoadBalancing_ReplicationManager_i::get_load_notifier (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
    LoadBalancing::InterfaceNotFound))
{
  ACE_THROW_RETURN (CORBA::NO_IMPLEMENT (),
                    LoadBalancing::LoadNotifier::_nil ());
}

void
TAO_LoadBalancing_ReplicationManager_i::register_load_monitor (
    LoadBalancing::LoadMonitor_ptr load_monitor,
    const PortableGroup::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::MonitorAlreadyPresent))
{
  TAO_LB_Location_Map_Entry *location_entry = 0;
  auto_ptr<TAO_LB_Location_Map_Entry> safe_location_entry;

  int result = this->location_map_.find (the_location,
                                         location_entry);

  // If no location entry exists for the given location, then create
  // and bind a new one.
  if (result != 0)
    {
      ACE_NEW_THROW_EX (location_entry,
                        TAO_LB_Location_Map_Entry,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK;

      ACE_AUTO_PTR_RESET (safe_location_entry,
                          location_entry,
                          TAO_LB_Location_Map_Entry);

      if (this->location_map_.bind (the_location,
                                    location_entry) != 0)
        ACE_THROW (CORBA::INTERNAL ());  // @@ Pick a better (user?)
                                         //    exception.

      // No longer need to protect the location entry.
      (void) safe_location_entry.release ();

      location_entry->load_monitor =
        LoadBalancing::LoadMonitor::_duplicate (load_monitor);
    }
  else if (CORBA::is_nil (location_entry->load_monitor.in ()))
    {
      location_entry->load_monitor =
        LoadBalancing::LoadMonitor::_duplicate (load_monitor);
    }
  else
    ACE_THROW (LoadBalancing::MonitorAlreadyPresent ());


  // Register the "pull monitoring" event handler only after the first
  // load monitor is registered.
  if (this->location_map_.current_size () == 1)
    {
      ACE_Time_Value interval (1, 0);
      ACE_Time_Value restart (1, 0);
      ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();
      (void) reactor->schedule_timer (&this->pull_handler_,
                                      0,
                                      interval,
                                      restart);
    }
}

LoadBalancing::LoadMonitor_ptr
TAO_LoadBalancing_ReplicationManager_i::get_load_monitor (
    const PortableGroup::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::LocationNotFound))
{
  TAO_LB_Location_Map_Entry *location_entry = 0;

  if (this->location_map_.find (the_location,
                                location_entry) == 0)
    {
      return
        LoadBalancing::LoadMonitor::_duplicate (
          location_entry->load_monitor.in ());
    }

  ACE_THROW_RETURN (LoadBalancing::LocationNotFound (),
                    LoadBalancing::LoadMonitor::_nil ());
}

void
TAO_LoadBalancing_ReplicationManager_i::remove_load_monitor (
    const PortableGroup::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::LocationNotFound))
{
  TAO_LB_Location_Map_Entry *location_entry = 0;

  if (this->location_map_.find (the_location,
                                location_entry) != 0)
    ACE_THROW (LoadBalancing::LocationNotFound ());

  (void) location_entry->load_monitor.out ();

  // If no load monitors are registered with the load balancer than
  // shutdown the "pull monitoring."
//   if (this->location_map_.current_size () == 0)
//     {
//       ACE_Reactor *reactor = this->orb_->orb_core ()->reactor ();
//       (void) reactor->cancel_timer (&this->pull_handler_);
//     }
}

void
TAO_LoadBalancing_ReplicationManager_i::set_default_properties (
    const PortableGroup::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.set_default_properties (props,
                                                  ACE_TRY_ENV);
}

PortableGroup::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_default_properties (ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::remove_default_properties (
    const PortableGroup::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.remove_default_properties (props,
                                                     ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::set_type_properties (
    const char *type_id,
    const PortableGroup::Properties &overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.set_type_properties (type_id,
                                               overrides,
                                               ACE_TRY_ENV);
}

PortableGroup::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_type_properties (
    const char *type_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_type_properties (type_id,
                                                 ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::remove_type_properties (
    const char *type_id,
    const PortableGroup::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.remove_type_properties (type_id,
                                                  props,
                                                  ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::set_properties_dynamically (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Properties &overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::InvalidProperty,
                   PortableGroup::UnsupportedProperty))
{
  this->property_manager_.set_properties_dynamically (object_group,
                                                      overrides,
                                                      ACE_TRY_ENV);
}

PortableGroup::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_properties (
    PortableGroup::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->property_manager_.get_properties (object_group,
                                            ACE_TRY_ENV);
}

PortableGroup::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::create_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location &the_location,
    const char *type_id,
    const PortableGroup::Criteria &the_criteria,
    CORBA::Environment &ACE_TRY_ENV)
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
                                               the_criteria,
                                               ACE_TRY_ENV);
}

PortableGroup::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::add_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location &the_location,
    CORBA::Object_ptr member,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberAlreadyPresent,
                   PortableGroup::ObjectNotAdded))
{
  return
    this->object_group_manager_.add_member (object_group,
                                            the_location,
                                            member,
                                            ACE_TRY_ENV);
}

PortableGroup::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::remove_member (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  return
    this->object_group_manager_.remove_member (object_group,
                                               the_location,
                                               ACE_TRY_ENV);
}

PortableGroup::Locations *
TAO_LoadBalancing_ReplicationManager_i::locations_of_members (
    PortableGroup::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.locations_of_members (object_group,
                                                      ACE_TRY_ENV);
}

PortableGroup::ObjectGroupId
TAO_LoadBalancing_ReplicationManager_i::get_object_group_id (
    PortableGroup::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_id (object_group,
                                                     ACE_TRY_ENV);
}

PortableGroup::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::get_object_group_ref (
    PortableGroup::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_ref (object_group,
                                                      ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::get_member_ref (
    PortableGroup::ObjectGroup_ptr object_group,
    const PortableGroup::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectGroupNotFound,
                   PortableGroup::MemberNotFound))
{
  return
    this->object_group_manager_.get_member_ref (object_group,
                                                the_location,
                                                ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::create_object (
    const char * type_id,
    const PortableGroup::Criteria & the_criteria,
    PortableGroup::GenericFactory::FactoryCreationId_out
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::NoFactory,
                   PortableGroup::ObjectNotCreated,
                   PortableGroup::InvalidCriteria,
                   PortableGroup::InvalidProperty,
                   PortableGroup::CannotMeetCriteria))
{
  CORBA::Object_ptr obj =
    this->generic_factory_.create_object (type_id,
                                          the_criteria,
                                          factory_creation_id,
                                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());


  return obj;
}

#if 0
void
TAO_LoadBalancing_ReplicationManager_i::process_criteria (
  const PortableGroup::Criteria & the_criteria,
  CORBA::Environment &ACE_TRY_ENV)
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

      // Obtain the InitialNumberReplicas from the_criteria.
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
TAO_LoadBalancing_ReplicationManager_i::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  this->generic_factory_.delete_object (factory_creation_id,
                                        ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::replica (
  const PortableServer::ObjectId &oid,
  CORBA::Environment &ACE_TRY_ENV)
{
  TAO_LB_ObjectGroup_Map_Entry *entry = 0;

  if (this->object_group_map_.find (oid, entry) != 0)
    {
      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                        CORBA::Object::_nil ());
    }

  return this->balancing_strategy_->replica (entry, ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::init (
  CORBA::ORB_ptr orb,
  PortableServer::POA_ptr root_poa,
  CORBA::Environment &ACE_TRY_ENV)
{
//   ACE_TRY_NEW_ENV
//     {
      // Create a new transient servant manager object in the child
      // POA.
      PortableServer::ServantManager_ptr tmp;
      ACE_NEW_THROW_EX (tmp,
                        TAO_LB_ReplicaLocator (this),
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK;

      PortableServer::ServantManager_var servant_manager =
        tmp;

      // Create the appropriate RequestProcessingPolicy
      // (USE_SERVANT_MANAGER) and ServantRetentionPolicy (NON_RETAIN)
      // for a ServantLocator.
      PortableServer::RequestProcessingPolicy_var request =
        root_poa->create_request_processing_policy (
          PortableServer::USE_SERVANT_MANAGER,
          ACE_TRY_ENV);
      ACE_CHECK;

      PortableServer::ServantRetentionPolicy_var retention =
        root_poa->create_servant_retention_policy (
          PortableServer::NON_RETAIN,
          ACE_TRY_ENV);
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
      // The ServantManager will be the ReplicaLocator.
      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (ACE_TRY_ENV);
      ACE_CHECK;

      this->poa_ = root_poa->create_POA ("TAO_LB_ReplicationManager_POA",
                                         poa_manager.in (),
                                         policy_list,
                                         ACE_TRY_ENV);
      ACE_CHECK;

      // Activate the child POA.
      poa_manager->activate (ACE_TRY_ENV);
      ACE_CHECK;

      request->destroy (ACE_TRY_ENV);
      ACE_CHECK;

      retention->destroy (ACE_TRY_ENV);
      ACE_CHECK;

      // Now set the ReplicaLocator as the child POA's Servant
      // Manager.
      this->poa_->set_servant_manager (servant_manager.in (),
                                       ACE_TRY_ENV);
      ACE_CHECK;

      this->object_group_map_.poa (this->poa_.in ());
      this->generic_factory_.poa (this->poa_.in ());

      this->orb_ = CORBA::ORB::_duplicate (orb);
//     }
//   ACE_CATCHANY
//     {
//       // @@ Should we do anything here?

//       ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
//                            "(%P|%t) TAO_LB_ReplicationManager_i::init:");

//       return -1;
//     }
//   ACE_ENDTRY;

//   return 0;
}
