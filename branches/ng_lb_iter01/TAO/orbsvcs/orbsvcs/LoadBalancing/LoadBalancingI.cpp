// -*- C++ -*-
//
// $Id$

#include "LoadBalancingI.h"

ACE_RCSID (LoadBalancing,
           LoadBalancingI,
           "$Id$")


// Implementation skeleton constructor
TAO_LoadBalancing_ReplicationManager_i::TAO_LoadBalancing_ReplicationManager_i
(void)
  : locator_ (this),
    poa_ (),
    lock_ (),
    object_group_map_ (),
    property_manager_ (),
    generic_factory_ (this->property_manager_,
                      this->object_group_map_),
    object_group_manager_ (this->property_manager_,
                           this->object_group_map_)
{
  (void) this->init ();
}

// Implementation skeleton destructor
TAO_LoadBalancing_ReplicationManager_i::~TAO_LoadBalancing_ReplicationManager_i (void)
{
}

void
TAO_LoadBalancing_ReplicationManager_i::register_load_notifier (
    TAO_LoadBalancing::LoadNotifier_ptr /* load_notifier */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

TAO_LoadBalancing::LoadNotifier_ptr
TAO_LoadBalancing_ReplicationManager_i::get_load_notifier (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
    TAO_LoadBalancing::InterfaceNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_LoadBalancing_ReplicationManager_i::set_default_properties (
    const TAO_LoadBalancing::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.set_default_properties (props,
                                                    ACE_TRY_ENV);
}

TAO_LoadBalancing::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  return
    this->property_manager_.get_default_properties (ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::remove_default_properties (
    const TAO_LoadBalancing::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.remove_default_properties (props,
                                                       ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::set_type_properties (
    const char *type_id,
    const TAO_LoadBalancing::Properties &overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.set_type_properties (type_id,
                                                 overrides,
                                                 ACE_TRY_ENV);
}

TAO_LoadBalancing::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_type_properties (
    const char * /* type_id */,
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
    const TAO_LoadBalancing::Properties &props,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.remove_type_properties (type_id,
                                                    props,
                                                    ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::set_properties_dynamically (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    const TAO_LoadBalancing::Properties &overrides,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  return
    this->property_manager_.set_properties_dynamically (object_group,
                                                        overrides,
                                                        ACE_TRY_ENV);
}

TAO_LoadBalancing::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_properties (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  return
    this->property_manager_.get_properties_dynamically (object_group,
                                                        ACE_TRY_ENV);
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::create_member (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    const TAO_LoadBalancing::Location &the_location,
    const char *type_id,
    const TAO_LoadBalancing::Criteria &the_criteria,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberAlreadyPresent,
                   TAO_LoadBalancing::NoFactory,
                   TAO_LoadBalancing::ObjectNotCreated,
                   TAO_LoadBalancing::InvalidCriteria,
                   TAO_LoadBalancing::CannotMeetCriteria))
{
  return
    this->object_group_manager_.create_member (object_group,
                                               the_location,
                                               type_id,
                                               the_criteria,
                                               ACE_TRY_ENV);
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::add_member (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    const TAO_LoadBalancing::Location &the_location,
    CORBA::Object_ptr member,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberAlreadyPresent,
                   TAO_LoadBalancing::ObjectNotAdded))
{
  return
    this->object_group_manager_.add_member (object_group,
                                            the_location,
                                            member,
                                            ACE_TRY_ENV);
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::remove_member (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    const TAO_LoadBalancing::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound))
{
  return
    this->object_group_manager_.remove_member (object_group,
                                               the_location,
                                               ACE_TRY_ENV);
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::set_primary_member (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    const TAO_LoadBalancing::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound,
                   TAO_LoadBalancing::PrimaryNotSet,
                   TAO_LoadBalancing::BadReplicationStyle))
{
  return
    this->object_group_manager_.set_primary_member (object_group,
                                                    the_location,
                                                    ACE_TRY_ENV);
}

TAO_LoadBalancing::Locations *
TAO_LoadBalancing_ReplicationManager_i::locations_of_members (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.locations_of_members (object_group,
                                                      ACE_TRY_ENV);
}

TAO_LoadBalancing::ObjectGroupId
TAO_LoadBalancing_ReplicationManager_i::get_object_group_id (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_id (object_group,
                                                     ACE_TRY_ENV);
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::get_object_group_ref (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  return
    this->object_group_manager_.get_object_group_ref (object_group,
                                                      ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::get_member_ref (
    TAO_LoadBalancing::ObjectGroup_ptr object_group,
    const TAO_LoadBalancing::Location &the_location,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound))
{
  return
    this->object_group_manager_.get_member_ref (object_group,
                                                the_location,
                                                ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::create_object (
    const char * type_id,
    const TAO_LoadBalancing::Criteria & the_criteria,
    TAO_LoadBalancing::GenericFactory::FactoryCreationId_out
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::NoFactory,
                   TAO_LoadBalancing::ObjectNotCreated,
                   TAO_LoadBalancing::InvalidCriteria,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::CannotMeetCriteria))
{
  return
    this->generic_factory_.create_object (type_id,
                                          the_criteria,
                                          factory_creation_id,
                                          ACE_TRY_ENV);
}

void
TAO_LoadBalancing_ReplicationManager_i::process_criteria (
  const TAO_LoadBalancing::Criteria & the_criteria,
  CORBA::Environment &ACE_TRY_ENV)
{
  // List of invalid criteria.  If this list has a length greater than
  // zero, then the TAO_LoadBalancing::InvalidCriteria exception will
  // be thrown.
  TAO_Loadbalancing::Criteria invalid_criteria;

  int found_factory = 0; // If factory was found in the_criteria, then
                         // set to 1.

  // Parse the criteria.
  CORBA::ULong criteria_count = the_criteria.length ();
  for (CORBA::ULong i = 0; i < criteria_size; ++i)
    {
      CORBA::UShort initial_number_replicas = 0;
      TAO_LoadBalancing::FactoryInfos factory_infos;

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
        ACE_THROW (TAO_LoadBalancer::InvalidProperty (the_criteria[i].nam,
                                                      the_criteria[i].val));
    }

  if (invalid_criteria.length () != 0)
    ACE_THROW (TAO_LoadBalancing::InvalidCriteria (invalid_criteria));

  if (found_factory == 0)
    ACE_THROW_RETURN (TAO_LoadBalancing::NoFactory ());
}

void
TAO_LoadBalancing_ReplicationManager_i::delete_object (
    const TAO_LoadBalancing::GenericFactory::FactoryCreationId &
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectNotFound))
{
  return
    this->generic_factory_.delete_object (factory_creation_id,
                                          ACE_TRY_ENV);
}

int
TAO_LoadBalancing_ReplicationManager_i::init (
  PortableServer::POA_ptr root_poa)
{
  ACE_TRY_NEW_ENV
    {
      // Create a new transient servant manager object in the Root
      // POA.
      PortableServer::ServantManager_ptr tmp;
      ACE_NEW_RETURN (servant_manager,
                      TAO_LB_ReplicaLocator,
                      -1);

      PortableServer::ServantManager_var servant_manager =
        tmp;

      // Create the appropriate RequestProcessingPolicy
      // (USE_SERVANT_MANAGER) and ServantRetentionPolicy (NON_RETAIN)
      // for a ServantLocator.
      PortableServer::RequestProcessingPolicy_var request =
        root_poa->create_request_processing_policy (
          PortableServer::USE_SERVANT_MANAGER,
          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      PortableServer::ServantRetentionPolicy_var retention =
        root_poa->create_servant_retention_policy (
          PortableServer::NON_RETAIN,
          ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Create the PolicyList.
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
      ACE_TRY_CHECK;
      this->poa_ = root_poa->create_POA ("TAO_LB_ReplicationManager_POA",
                                         poa_manager.in (),
                                         policy_list,
                                         ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Activate the child POA.
      poa_manager->activate (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      request->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      retention->destroy (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Now set the ReplicaLocator as the child POA's Servant
      // Manager.
      this->poa_->set_servant_manager (servant_manager.in (),
                                       ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // @@ Should we do anything here?

      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "(%P|%t) TAO_LB_ReplicationManager_i::init:");

      return -1;
    }
  ACE_ENDTRY;

  return 0;
}

void
TAO_LoadBalancing_ReplicationManager_i::operator= (
  TAO_LoadBalancing::FactoryInfo &lhs,
  const TAO_LoadBalancing::FactoryInfo &rhs)
{
  lhs.the_factory =
    TAO_LoadBalancing::GenericFactory::_duplicate (rhs.the_factory);

  lhs.the_location = rhs.the_location;

  lhs.the_criteria = rhs.the_criteria;
}
