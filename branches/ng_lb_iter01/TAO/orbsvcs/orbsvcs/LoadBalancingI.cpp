// -*- C++ -*-
//
// $Id$

#include "LoadBalancingI.h"

#include "LB_ReplicaLocator.h"
#include "LB_Balancing_Strategy.h"

#include "LB_Minimum_Dispersion.h"  // @@ REMOVE ME!

ACE_RCSID (LoadBalancing,
           LoadBalancingI,
           "$Id$")


// Implementation skeleton constructor
TAO_LoadBalancing_ReplicationManager_i::TAO_LoadBalancing_ReplicationManager_i
(void)
  : poa_ (),
    lock_ (),
    location_map_
    object_group_map_ (),
    property_manager_ (this->object_group_map_),
    generic_factory_ (this->location_map_,
                      this->object_group_map_,
                      this->property_manager_),
    object_group_manager_ (this->location_map_,
                           this->object_group_map_,
                           this->property_manager_),
    balancing_strategy_ (new TAO_LB_Minimum_Dispersion_Strategy) //@@ FIXME!
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
    const LoadBalancing::Location &the_location,
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
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK;

      safe_location_entry = location_entry;

      if (this->location_map_.bind (the_location,
                                    location_entry) != 0)
        ACE_THROW (CORBA::INTERNAL ());  // @@ Pick a better (user?)
                                         //    exception.
    }
  else if (CORBA::is_nil (location_entry->load_monitor))
    {
      location_entry->load_monitor =
        LoadBalancing::LoadMonitor::_duplicate (load_monitor);
    }
  else
    ACE_THROW (LoadBalancing::MonitorAlreadyPresent ());
}

LoadBalancing::LoadMonitor_ptr
TAO_LoadBalancing_ReplicationManager_i::get_load_monitor (
    const LoadBalancing::Location &the_location,
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
TAO_LoadBalancing_ReplicationManager_i::set_default_properties (
    const LoadBalancing::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.set_default_properties (props,
                                                    ACE_TRY_ENV);
}

LoadBalancing::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_default_properties (ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::remove_default_properties (
    const LoadBalancing::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.remove_default_properties (props,
                                                       ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::set_type_properties (
    const char *type_id,
    const LoadBalancing::Properties &overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.set_type_properties (type_id,
                                                 overrides,
                                                 ACE_TRY_ENV);
}

LoadBalancing::Properties *
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
    const LoadBalancing::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.remove_type_properties (type_id,
                                                    props,
                                                    ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::set_properties_dynamically (
    LoadBalancing::ObjectGroup_ptr object_group,
    const LoadBalancing::Properties &overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.set_properties_dynamically (object_group,
                                                        overrides,
                                                        ACE_TRY_ENV);
}

LoadBalancing::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_properties (
    LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  return
    this->property_manager_.get_properties (object_group,
                                            ACE_TRY_ENV);
}

LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::create_member (
    LoadBalancing::ObjectGroup_ptr object_group,
    const LoadBalancing::Location &the_location,
    const char *type_id,
    const LoadBalancing::Criteria &the_criteria,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberAlreadyPresent,
                   LoadBalancing::NoFactory,
                   LoadBalancing::ObjectNotCreated,
                   LoadBalancing::InvalidCriteria,
                   LoadBalancing::CannotMeetCriteria))
{
  return
    this->object_group_manager_.create_member (object_group,
                                               the_location,
                                               type_id,
                                               the_criteria,
                                               ACE_TRY_ENV);
}

LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::add_member (
    LoadBalancing::ObjectGroup_ptr object_group,
    const LoadBalancing::Location &the_location,
    CORBA::Object_ptr member,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberAlreadyPresent,
                   LoadBalancing::ObjectNotAdded))
{
  return
    this->object_group_manager_.add_member (object_group,
                                            the_location,
                                            member,
                                            ACE_TRY_ENV);
}

LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::remove_member (
    LoadBalancing::ObjectGroup_ptr object_group,
    const LoadBalancing::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberNotFound))
{
  return
    this->object_group_manager_.remove_member (object_group,
                                               the_location,
                                               ACE_TRY_ENV);
}

LoadBalancing::Locations *
TAO_LoadBalancing_ReplicationManager_i::locations_of_members (
    LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.locations_of_members (object_group,
                                                      ACE_TRY_ENV);
}

LoadBalancing::ObjectGroupId
TAO_LoadBalancing_ReplicationManager_i::get_object_group_id (
    LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_id (object_group,
                                                     ACE_TRY_ENV);
}

LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::get_object_group_ref (
    LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_ref (object_group,
                                                      ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::get_member_ref (
    LoadBalancing::ObjectGroup_ptr object_group,
    const LoadBalancing::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectGroupNotFound,
                   LoadBalancing::MemberNotFound))
{
  return
    this->object_group_manager_.get_member_ref (object_group,
                                                the_location,
                                                ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::create_object (
    const char * type_id,
    const LoadBalancing::Criteria & the_criteria,
    LoadBalancing::GenericFactory::FactoryCreationId_out
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::NoFactory,
                   LoadBalancing::ObjectNotCreated,
                   LoadBalancing::InvalidCriteria,
                   LoadBalancing::InvalidProperty,
                   LoadBalancing::CannotMeetCriteria))
{
  return
    this->generic_factory_.create_object (type_id,
                                          the_criteria,
                                          factory_creation_id,
                                          ACE_TRY_ENV);
}

#if 0
void
TAO_LoadBalancing_ReplicationManager_i::process_criteria (
  const LoadBalancing::Criteria & the_criteria,
  CORBA::Environment &ACE_TRY_ENV)
{
  // List of invalid criteria.  If this list has a length greater than
  // zero, then the LoadBalancing::InvalidCriteria exception will
  // be thrown.
  LoadBalancing::Criteria invalid_criteria;

  int found_factory = 0; // If factory was found in the_criteria, then
                         // set to 1.

  // Parse the criteria.
  CORBA::ULong criteria_count = the_criteria.length ();
  for (CORBA::ULong i = 0; i < criteria_size; ++i)
    {
      CORBA::UShort initial_number_replicas = 0;
      LoadBalancing::FactoryInfos factory_infos;

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
        ACE_THROW (LoadBalancing::InvalidProperty (the_criteria[i].nam,
                                                   the_criteria[i].val));
    }

  if (invalid_criteria.length () != 0)
    ACE_THROW (LoadBalancing::InvalidCriteria (invalid_criteria));

  if (found_factory == 0)
    ACE_THROW (LoadBalancing::NoFactory ());
}
#endif  /* 0 */

void
TAO_LoadBalancing_ReplicationManager_i::delete_object (
    const LoadBalancing::GenericFactory::FactoryCreationId &
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectNotFound))
{
  return
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
