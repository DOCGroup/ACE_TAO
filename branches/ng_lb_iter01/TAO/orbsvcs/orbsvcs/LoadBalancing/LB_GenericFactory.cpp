// -*- C++ -*-

#include "LB_GenericFactory.h"

ACE_RCSID (LoadBalancing,
           LB_GenericFactory,
           "$Id$")


TAO_LB_GenericFactory::TAO_LB_GenericFactory (
  PortableServer::POA_ptr poa,
  TAO_LB_PropertyManager &property_manager,
  TAO_LB_ObjectGroup_Map &object_group_map)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    property_manager_ (property_manager),
    object_group_map_ (object_group_map),
    next_fcid_ (0)
{
}

CORBA::Object_ptr
TAO_LB_GenericFactory::create_object (
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
  LoadBalancing::Criteria_var creation_criteria =
    this->property_manager_.process_criteria (the_criteria,
                                              ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::ULong factory_infos_count = factory_infos.length ();

  // If the number of factories is less than the initial number of
  // replicas, then the desired number of replicas cannot possibly be
  // created.
  if (factory_infos_count < initial_number_replicas)
    ACE_THROW_RETURN (LoadBalancing::CannotMeetCriteria (),
                      CORBA::Object::_nil ());

  TAO_LB_ObjectGroup_Map::Map_Entry *object_group_entry = 0;
  ACE_NEW_THROW_EX (object_group_entry,
                    TAO_LB_ObjectGroup_Map::Map_Entry,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  auto_ptr<TAO_LB_ObjectGroup_Map::Map_Entry> safe_object_group_entry (
    object_group_entry);

  if (this->property_manager_.infrastructure_controlled_membership ())
    {
      this->populate_object_group (object_group_entry, ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());
    }

  // Set the RepositoryId associated with the created ObjectGroupMap
  // entry.
  object_group_entry.type_id = CORBA::string_dup (type_id);

  TAO_LB_GenericFactory::FactoryCreationId fcid = this->next_fcid_;

  // The ObjectId for the newly created object group is comprised
  // solely of the FactoryCreationId.
  PortableServer::ObjectId_var oid;
  this->get_ObjectId (fcid, oid.out ());

  // Create a reference for the ObjectGroup corresponding to the
  // RepositoryId of the object being created.
  CORBA::Object_var object_group =
    this->poa_->create_reference_with_id (oid.in (),
                                          type_id,
                                          ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  object_group_entry->object_group =
    CORBA::Object::_duplicate (object_group.in ());

  // Allocate a new FactoryCreationId for use as an "out" parameter.
  LoadBalancing::GenericFactory::FactoryCreationId *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    LoadBalancing::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  factory_creation_id = tmp;

  // Only increment the next FactoryCreationId if the object group was
  // successfully created.
  factory_creation_id <<= fcid;

  // Now associate the ObjectId with the ObjectGroup reference.  In
  // this implementation, the FactoryCreationId is the ObjectId.
  //
  // The bind() must be the last operation to perform since a bind()
  // should occur only if the object group and FactoryCreationId out
  // parameter we successfully created, and no exception was thrown.
  // Otherwise, an invalid object group entry would remain inside the
  // map.
  if (this->object_group_map_.bind (fcid, object_group_entry) != 0)
    ACE_THROW_RETURN (TAO_LoadBalancer::ObjectNotCreated (),
                      CORBA::Object::_nil ());

  // No longer need to protect the allocated ObjectGroup_Map entry.
  (void) safe_object_group_entry.release ();

  // Object group was successfully created.  Increment the next
  // FactoryCreationId in preparation for the next object group.
  this->next_fcid_++;

  return object_group._retn ();
}

void
TAO_LB_GenericFactory::delete_object (
    const LoadBalancing::GenericFactory::FactoryCreationId &
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectNotFound))
{
  CORBA::ULong fcid = 0;

  if (factory_creation_id >>= fcid) // Extract the actual FactoryCreationId.
    {
      // Successfully extracted the FactoryCreationId.  Now find the
      // object group map corresponding to it.
      TAO_LB_ObjectGroup_Entry *object_group = 0;

      if (this->object_group_map_.find (fcid, object_group) == -1)
        ACE_THROW (LoadBalancing::ObjectNotFound ());

      TAO_LB_Replica_Map *replica_map = object_group->replica_map;

      for (TAO_LB_Replica_Map::Table::iterator i = replica_map->begin ();
           i != replica_map->end ();
           ++i)
        {
          TAO_LB_Replica_Map_Entry *replica = (*i).ext_id_;

          LoadBalancing::GenericFactory_ptr factory =
            replica->factory.in ();

          if (!CORBA::is_nil (factory))
            {
              LoadBalancing::GenericFactory::FactoryCreationId
                &replica_fcid = replica->factory_creation_id;

              factory->delete_object (replica_fcid.in, ACE_TRY_ENV);
              ACE_CHECK;
            }

          
          (void) replica_map->unbind (&(*i));

          delete replica_map;
        }

      // Now delete the ObjectGroup from the set of ObjectGroups.
      this->object_group_map_.unbind (fcid);

      delete object_group;
    }

  ACE_THROW (LoadBalancing::ObjectNotFound ());
}

void
TAO_LB_GenericFactory::populate_object_group (
  TAO_LB_ObjectGroup_Map::Map_Entry *object_group_entry,
  CORBA::Environment &ACE_TRY_ENV)
{
  for (CORBA::ULong j = 0; j < factory_infos_count; ++j)
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


      LoadBalancing::FactoryInfo &factory_info =
        factory_infos[j];

      TAO_LoadBalancer::GenericFactory_ptr factory =
        factory_info.the_factory;

      LoadBalancing::GenericFactory::FactoryCreationId_var
        replica_fcid;

      CORBA::Object_var replica =
        factory->create_object (type_id,
                                factory_info.the_criteria,
                                replica_fcid.out (),
                                ACE_TRY_ENV);
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

#if 0
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
          factory->delete_object (replica_fcid,
                                  ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          ACE_THROW_RETURN (TAO_LoadBalancer::NoFactory (),
                            CORBA::Object::_nil ());
        }
#endif  /* 0 */

      // Create a new Replica_Map map entry.
      TAO_LB_ReplicaInfo *replica_info = 0;
      ACE_NEW_THROW_EX (replica_info,
                        TAO_LB_ReplicaInfo,
                        CORBA::NO_MEMORY (
                          CORBA::SystemException::_tao_minor_code (
                            TAO_DEFAULT_MINOR_CODE,
                            ENOMEM),
                          CORBA::COMPLETED_NO));
      ACE_CHECK_RETURN (CORBA::Object::_nil ());

      auto_ptr<TAO_LB_ReplicaInfo> safe_replica_info (replica_info);

      replica_info->replica = replica;

      replica_info->factory_info = factory_info;  // Deep copy.

      replica_info->factory_creation_id = replica_fcid;

      if (object_group_entry.replica_infos.insert (replica_info) != 0)
        {
          // An Object of incorrect type was created.  Delete it, and
          // throw a NoFactory exception.
          factory->delete_object (replica_fcid, ACE_TRY_ENV);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());

          ACE_THROW_RETURN (TAO_LoadBalancer::ObjectNotCreated (),
                            CORBA::Object::_nil ());
        }

      // No longer need to protect the allocated Replica_Map.
      safe_replica_entry.release ();
    }
}

void
TAO_LoadBalancing_ReplicationManager_i::get_ObjectId (
  TAO_LB_GenericFactory::FactoryCreationId fcid,
  PortableServer::ObjectId_out oid)
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

  // 4294967295UL -- Largest 32 bit unsigned integer
  // 123456789012 -- 10 digits
  //                + 2 for "UL"  (unnecessary, but let's be safe)
  //                + 1 for null terminator
  //                + 1 for good luck. :-)
  const size_t MAX_OID_LEN = 14;

  char oid_str[MAX_OID_LEN] = { 0 };
  ACE_OS::sprintf (oid_str,
                   "%ul",
                   fcid);

  oid = PortableServer::string_to_ObjectId (oid_str);
}
