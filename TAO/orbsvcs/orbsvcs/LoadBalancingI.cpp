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
    next_oid_ (0),
    object_group_map_ ()
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
    const TAO_LoadBalancing::Properties & /* props */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  ACE_THROW (CORBA::NO_IMPLEMENT());
}

TAO_LoadBalancing::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_default_properties (
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_LoadBalancing_ReplicationManager_i::remove_default_properties (
    const TAO_LoadBalancing::Properties & /* props */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_LoadBalancing_ReplicationManager_i::set_type_properties (
    const char * /* type_id */,
    const TAO_LoadBalancing::Properties & /* overrides */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_type_properties (
    const char * /* type_id */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_LoadBalancing_ReplicationManager_i::remove_type_properties (
    const char * /* type_id */,
    const TAO_LoadBalancing::Properties & /* props */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

void
TAO_LoadBalancing_ReplicationManager_i::set_properties_dynamically (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Properties & /* overrides */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::UnsupportedProperty))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::Properties *
TAO_LoadBalancing_ReplicationManager_i::get_properties (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::create_member (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & /* the_location */,
    const char * /* type_id */,
    const TAO_LoadBalancing::Criteria & /* the_criteria */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberAlreadyPresent,
                   TAO_LoadBalancing::NoFactory,
                   TAO_LoadBalancing::ObjectNotCreated,
                   TAO_LoadBalancing::InvalidCriteria,
                   TAO_LoadBalancing::CannotMeetCriteria))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::add_member (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & /* the_location */,
    CORBA::Object_ptr /* member */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberAlreadyPresent,
                   TAO_LoadBalancing::ObjectNotAdded))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::remove_member (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & /* the_location */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::set_primary_member (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & /* the_location */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound,
                   TAO_LoadBalancing::PrimaryNotSet,
                   TAO_LoadBalancing::BadReplicationStyle))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::Locations *
TAO_LoadBalancing_ReplicationManager_i::locations_of_members (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroupId
TAO_LoadBalancing_ReplicationManager_i::get_object_group_id (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

TAO_LoadBalancing::ObjectGroup_ptr
TAO_LoadBalancing_ReplicationManager_i::get_object_group_ref (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::get_member_ref (
    TAO_LoadBalancing::ObjectGroup_ptr /* object_group */,
    const TAO_LoadBalancing::Location & loc,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectGroupNotFound,
                   TAO_LoadBalancing::MemberNotFound))
{
  ACE_THROW (CORBA::NO_IMPLEMENT ());
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
  // Check if an ObjectGroup for the given type hasn't already been
  // created.
  if (this->object_group_map_.find_type_id (type_id) == 0)
    ACE_THROW_RETURN (TAO_LoadBalancing::ObjectNotCreated ());    

  // List of invalid criteria.  If this list has a length greater than
  // zero, then the TAO_LoadBalancing::InvalidCriteria exception will
  // be thrown.
  TAO_Loadbalancing::Criteria invalid_criteria;

  int found_factory = 0; // If factory was found in the_criteria, then
                         // set to 1.

  // Parse the criteria.
  int criteria_count = the_criteria.length ();
  for (int i = 0; i < criteria_size; ++i)
    {
      CORBA::UShort initial_number_replicas = 0;
      TAO_LoadBalancing::FactoryInfos factory_infos;

      // Obtain the InitialNumberReplicas from the_criteria.
      if (this->get_initial_number_replicas (type_id,
                                             the_criteria[i],
                                             initial_number_replicas) != 0)
        {
          size_t length = invalid_criteria.length ();
          invalid_criteria.length (length + 1);
          invalid_criteria[length] = the_criteria[i];
        }

      // Obtain the FactoryInfos from the_criteria.  This method also
      // ensures that GenericFactories at different locations are used.
      else if (this->get_factory_infos (type_id,
                                        the_criteria[i],
                                        factory_infos) == 0)
        found_factory = 1;

      // Unknown property
      else
        ACE_THROW_RETURN (TAO_LoadBalancer::InvalidProperty (
                            the_criteria[i].nam,
                            the_criteria[i].val),
                          CORBA::Object::_nil ());
    }

  if (invalid_criteria.length () != 0)
    ACE_THROW_RETURN (TAO_LoadBalancing::InvalidCriteria (invalid_criteria),
                      CORBA::Object::_nil ());

  if (found_factory == 0)
    ACE_THROW_RETURN (TAO_LoadBalancing::NoFactory (),
                      CORBA::Object::_nil ());

  return this->create_object_i (type_id,
                                initial_number_replicas,
                                factory_infos,
                                ACE_TRY_ENV);
}

CORBA::Object_ptr
TAO_LoadBalancing_ReplicationManager_i::create_object_i (
    const char *type_id,
    CORBA::UShort initial_number_replicas,
    TAO_LoadBalancing::FactoryInfos &factory_infos,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::NoFactory,
                   TAO_LoadBalancing::ObjectNotCreated,
                   TAO_LoadBalancing::InvalidCriteria,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::CannotMeetCriteria))
{
  size_t factory_infos_count = factory_infos.length ();

  // If the number of factories is less than the initial number of
  // replicas, then the desired number of replicas cannot possibly be
  // created.
  if (factory_infos_count < initial_number_replicas)
    ACE_THROW_RETURN (TAO_LoadBalancing::CannotMeetCriteria (),
                      CORBA::Object::_nil ());

  TAO_LB_ObjectGroup_Map::Map_Entry *object_group_entry = 0;
  ACE_NEW_THROW_EX (object_group_entry,
                    TAO_LB_ObjectGroup_Map::Map_Entry,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  auto_ptr<TAO_LB_ObjectGroup_Map> safe_object_group_entry (
    object_group_entry);

  for (int j = 0; j < factory_infos_count; ++j)
    {
      // The FactoryInfo::the_location member was used when
      // determining which FactoryInfo 
      //    member?
      // @@ It looks like it is only used when the application
      //    control membership style is used.  The application
      //    requests that a replica be created at a given
      //    "location," at which point the ReplicationManager
      //    searches through its registered FactoryInfos for a
      //    FactoryInfo with a "Location" member that matches
      //    the location at which to create the desired
      //    replica.
      // @@ It is also used to ensure that only one replica of
      //    a given type is created at a given location.


      TAO_LoadBalancing::FactoryInfo &factory_info =
        factory_infos[j];

      TAO_LoadBalancer::GenericFactory_ptr factory =
        factory_info.factory;

      TAO_LoadBalancing::GenericFactory::FactoryCreationId_out
        replica_factory_creation_id;

      CORBA::Object_var replica =
        factory->create_object (type_id,
                                factory_info.the_criteria,
                                replica_factory_creation_id,
                                ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      // @@ Should an "_is_a()" be performed here?  While it appears
      //    to be the right thing to do, it can be expensive.
      //
      // Make sure an Object of the correct type was created.  It is
      // possible that an object of the wrong type was created if the
      // type_id parameter does not match the type of object the
      // GenericFactory creates.
      CORBA::Boolean right_type_id =
        replica->_is_a (type_id, ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      if (!right_type_id)
        {
          // An Object of incorrect type was created.  Delete it, and
          // throw a NoFactory exception.
          factory->delete_object (replica_factory_creation_id,
                                  ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          ACE_THROW_RETURN (TAO_LoadBalancer::NoFactory (),
                            CORBA::Object::_nil ());
        }

      // Create a new Replica_Map map entry.
      TAO_LB_Replica_Map::Map_Entry *replica_entry = 0;
      ACE_NEW_THROW_EX (replica_entry,
                        TAO_LB_Replica_Map::Map_Entry,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      auto_ptr<TAO_LB_Replica_Map> safe_replica_entry (replica_entry);

      replica_entry->replica = replica;

      TAO_LoadBalancer::FactoryInfo *new_factory_info = 0;
      ACE_NEW_THROW_EX (new_factory_info,
                        TAO_LoadBalancer::FactoryInfo,
                        CORBA::NO_MEMORY ());
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      replica_entry->factory_info = new_factory_info;

      // Copy the FactoryInfo structure.  A deep copy is actually
      // performed here.
      *new_factory_info = factory_info;

      replica_entry->factory_creation_id = replica_factory_creation_id;

      if (object_group_entry.replica_map.bind (replica_entry) != 0)
        {
          // An Object of incorrect type was created.  Delete it, and
          // throw a NoFactory exception.
          factory->delete_object (replica_factory_creation_id,
                                  ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          ACE_THROW_RETURN (TAO_LoadBalancer::ObjectNotCreated (),
                            CORBA::Object::_nil ());
        }

      // No longer need to protect the allocated Replica_Map.
      safe_replica_entry.release ();
    }

  // Create a reference for the ObjectGroup corresponding to the
  // RepositoryId of the object being created.

  PortableServer::ObjectId_var oid;
  this->get_ObjectId (oid.out ());

  CORBA::Object_var object_group =
    this->poa_->create_reference_with_id (oid.in (),
                                          type_id,
                                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  object_group_entry.type_id = CORBA::string_dup (type_id);
  object_group_entry.object_group = object_group;

  // @@ This is ugly, and needs to be cleaned up.
  object_group_entry.factory_creation_id = this->next_oid_ - 1;

  // Now (indirectly) associate the ObjectId with the ObjectGroup
  // reference.
  if (this->object_group_map_.bind (type_id, object_group_entry) != 0)
    ACE_THROW_RETURN (TAO_LoadBalancer::ObjectNotCreated (),
                      CORBA::Object::_nil ());

  // No longer need to protect the allocated ObjectGroup_Map.
  safe_object_group_entry.release ();

  return CORBA::Object::_duplicate (object_group.in ());
}

void
TAO_LoadBalancing_ReplicationManager_i::get_initial_number_replicas ()
{

}

void
TAO_LoadBalancing_ReplicationManager_i::get_factory_infos ()
{

}

void
TAO_LoadBalancing_ReplicationManager_i::delete_object (
    const TAO_LoadBalancing::GenericFactory::FactoryCreationId &
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::ObjectNotFound))
{
  ObjectGroup = this->object_groups_.find (factory_creation_id);
  if (ObjectGroup == -1)
    ACE_THROW (TAO_LoadBalancing::ObjectNotFound ());

  // Delete the individual replicas at their local factories.
  for (int i = 0; i < ObjectGroup.size (); ++i)
    {
      TAO_LoadBalancing::GenericFactory_ptr factory =
        ObjectGroup[i].factory;
      TAO_LoadBalancing::GenericFactory::FactoryCreationId
        replica_factory_id = ObjectGroup[i].replica_factory_id;
      factory->delete_object (replica_factory_id);
    }

  // Now delete the ObjectGroup from the set of ObjectGroups.
  this->object_groups_.unbind (factory_creation_id);
}

int
TAO_LoadBalancing_ReplicationManager_i::init (
  PortableServer::POA_ptr root_poa)
{
  ACE_TRY_NEW_ENV
    {
      // Create a new transient servant manager object in the Root
      // POA.
      PortableServer::ServantManager_var servant_manager =
        this->locator_._this (ACE_TRY_ENV);
      ACE_TRY_CHECK;

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
TAO_LoadBalancing_ReplicationManager_i::get_ObjectId (
  PortableServer::ObjectId_out &oid)
{
  // Since the POA used by the ReplicationManager uses the NON_RETAIN
  // policy, explicitly choose an ObjectId that is unique to a given
  // type.

  // Make the ObjectId be the next value of the number of types that
  // have been registered with the ReplicationManager.  For example,
  // if two types of objects have been registered with the
  // ReplicationManager, then the ObjectId for the object currently
  // being registered will be "3" since the object will be the third
  // type of object registered with the ReplicationManager.
  // Previously used values will not be reused to ensure that a
  // ServantLocator does not inadvertently return a reference to an
  // object that had a previously used ObjectId.  Specifcally, the
  // numerical value used for the ObjectId increases monotonically.

  char oid_str[BUFSIZ] = { 0 };
  ACE_OS::sprintf (oid_str,
                   "%ul",
                   this->next_oid_);

  oid = PortableServer::string_to_ObjectId (oid_str);

  // Increment the value for the next ObjectId.
  this->next_oid_++;
}

void
TAO_LoadBalancing_ReplicationManager_i::operator= (
  TAO_LoadBalancing::FactoryInfo &lhs,
  const TAO_LoadBalancing::FactoryInfo &rhs)
{
  lhs.factory =
    TAO_LoadBalancing::GenericFactory::_duplicate (rhs.factory);

  lhs.the_location = rhs.the_location;

  lhs.the_criteria = rhs.the_criteria;
}
