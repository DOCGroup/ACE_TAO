// -*- C++ -*-

#include "LB_GenericFactory.h"
#include "LB_ReplicaInfo.h"
#include "LB_ObjectGroup_Map.h"
#include "LB_PropertyManager.h"

#include "ace/Auto_Ptr.h"

ACE_RCSID (LoadBalancing,
           LB_GenericFactory,
           "$Id$")


TAO_LB_GenericFactory::TAO_LB_GenericFactory (
  TAO_LB_Location_Map &location_map,
  TAO_LB_ObjectGroup_Map &object_group_map,
  TAO_LB_PropertyManager &property_manager)
  : poa_ (),
    location_map_ (location_map),
    object_group_map_ (object_group_map),
    property_manager_ (property_manager),
    next_fcid_ (0),
    lock_ ()
{
}

CORBA::Object_ptr
TAO_LB_GenericFactory::create_object (
    const char * type_id,
    const PortableGroup::Criteria &the_criteria,
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
//   PortableGroup::Properties_var properties =
//     this->property_manager_.get_type_properties (type_id,
//                                                  ACE_TRY_ENV);
//   ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Make sure the criteria for the object group being created are
  // valid.
//   this->property_manager_.process_criteria (the_criteria, ACE_TRY_ENV);
//   ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Extract the initial number of replicas to create.
  PortableGroup::InitialNumberMembersValue initial_number_members =
    this->property_manager_.initial_number_members (type_id,
                                                     the_criteria,
                                                     ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Extract the factory information for each of the replicas.
  PortableGroup::FactoryInfos_var factory_infos =
    this->property_manager_.factory_infos (type_id,
                                           the_criteria,
                                           ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  CORBA::ULong factory_infos_count = factory_infos->length ();

  // If the number of factories is less than the initial number of
  // replicas, then the desired number of replicas cannot possibly be
  // created.
  if (factory_infos_count < initial_number_members)
    ACE_THROW_RETURN (PortableGroup::CannotMeetCriteria (),
                      CORBA::Object::_nil ());

  TAO_LB_ObjectGroup_Map_Entry *object_group_entry = 0;
  ACE_NEW_THROW_EX (object_group_entry,
                    TAO_LB_ObjectGroup_Map_Entry,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  auto_ptr<TAO_LB_ObjectGroup_Map_Entry> safe_object_group_entry (
    object_group_entry);

  // Set the RepositoryId associated with the created ObjectGroupMap
  // entry.
  object_group_entry->type_id = CORBA::string_dup (type_id);

  CORBA::ULong fcid = 0;

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      CORBA::Object::_nil ());

    fcid = this->next_fcid_;
  }

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

//   if (this->property_manager_.infrastructure_controlled_membership ())
//     {
  this->populate_object_group (object_group_entry,
                               factory_infos.in (),
                               ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());
//     }

  // Allocate a new FactoryCreationId for use as an "out" parameter.
  PortableGroup::GenericFactory::FactoryCreationId *tmp = 0;
  ACE_NEW_THROW_EX (tmp,
                    PortableGroup::GenericFactory::FactoryCreationId,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  factory_creation_id = tmp;

  *tmp <<= fcid;

  // Now associate the ObjectId with the ObjectGroup reference.  In
  // this implementation, the FactoryCreationId is the ObjectId.
  //
  // The bind() must be the last operation to perform since a bind()
  // should occur only if the object group and FactoryCreationId out
  // parameter we successfully created, and no exception was thrown.
  // Otherwise, an invalid object group entry would remain inside the
  // map.
  if (this->object_group_map_.bind (oid.in (), object_group_entry) != 0)
    ACE_THROW_RETURN (PortableGroup::ObjectNotCreated (),
                      CORBA::Object::_nil ());

  // No longer need to protect the allocated ObjectGroup_Map entry.
  (void) safe_object_group_entry.release ();

  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      guard,
                      this->lock_,
                      CORBA::Object::_nil ());

    // Object group was successfully created.  Increment the next
    // FactoryCreationId in preparation for the next object group.
    this->next_fcid_++;
  }

  return object_group._retn ();
}

void
TAO_LB_GenericFactory::delete_object (
    const PortableGroup::GenericFactory::FactoryCreationId &
      factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   PortableGroup::ObjectNotFound))
{
  CORBA::ULong fcid = 0;

  if (factory_creation_id >>= fcid) // Extract the actual FactoryCreationId.
    {
      // Successfully extracted the FactoryCreationId.  Now find the
      // object group map corresponding to it.

      // The ObjectId for the newly created object group is comprised
      // solely of the FactoryCreationId.
      PortableServer::ObjectId_var oid;
      this->get_ObjectId (fcid, oid.out ());

      TAO_LB_ObjectGroup_Map_Entry *object_group = 0;

      if (this->object_group_map_.find (oid.in (), object_group) == -1)
        ACE_THROW (PortableGroup::ObjectNotFound ());

      TAO_LB_ReplicaInfo_Set &replica_infos = object_group->replica_infos;

      {
        ACE_GUARD (TAO_SYNCH_MUTEX, guard, object_group->lock);
        for (TAO_LB_ReplicaInfo_Set::iterator i = replica_infos.begin ();
             i != replica_infos.end ();
             ++i)
          {
            TAO_LB_ReplicaInfo *replica_info = (*i);

            PortableGroup::GenericFactory_ptr factory =
              replica_info->factory_info.the_factory.in ();

            // If the factory reference is not nil, then the replica
            // was created using a GenericFactory.  Make sure that
            // factory deletes it.
            if (!CORBA::is_nil (factory))
              {
                const PortableGroup::GenericFactory::FactoryCreationId
                  &replica_fcid = replica_info->factory_creation_id.in ();

                factory->delete_object (replica_fcid, ACE_TRY_ENV);
                ACE_CHECK;
              }

            (void) replica_infos.remove (replica_info);
            (void) replica_info->location_entry->replica_infos.remove (
                     replica_info);

            delete replica_info;
          }
      }

      // Now delete the ObjectGroup from the set of ObjectGroups.
      this->object_group_map_.unbind (oid.in ());

      delete object_group;
    }
  else
    ACE_THROW (PortableGroup::ObjectNotFound ());
}

void
TAO_LB_GenericFactory::poa (PortableServer::POA_ptr p)
{
  this->poa_ = PortableServer::POA::_duplicate (p);
}

void
TAO_LB_GenericFactory::populate_object_group (
  TAO_LB_ObjectGroup_Map_Entry *object_group_entry,
  const PortableGroup::FactoryInfos &factory_infos,
  CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::ULong factory_infos_count = factory_infos.length ();
  for (CORBA::ULong j = 0; j < factory_infos_count; ++j)
    {
      TAO_LB_Location_Map_Entry *location_entry = 0;
      auto_ptr<TAO_LB_Location_Map_Entry> safe_location_entry;

      const PortableGroup::FactoryInfo &factory_info =
        factory_infos[j];

      PortableGroup::GenericFactory_ptr factory =
        factory_info.the_factory.in ();

      PortableGroup::GenericFactory::FactoryCreationId_var
        replica_fcid;

      CORBA::Object_var replica =
        factory->create_object (object_group_entry->type_id.in (),
                                factory_info.the_criteria,
                                replica_fcid.out (),
                                ACE_TRY_ENV);
      ACE_CHECK;

      // If no location entry exists for the given location, then
      // create and bind a new one.
      if (this->location_map_.find (factory_info.the_location,
                                    location_entry) != 0)
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

          if (this->location_map_.bind (factory_info.the_location,
                                        location_entry) != 0)
            ACE_THROW (PortableGroup::ObjectNotCreated ());
        }

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
      ACE_CHECK;

      if (!right_type_id)
        {
          // An Object of incorrect type was created.  Delete it, and
          // throw a NoFactory exception.
          factory->delete_object (replica_fcid.in (),
                                  ACE_TRY_ENV);
          ACE_CHECK;

          ACE_THROW (PortableGroup::NoFactory ());
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
      ACE_CHECK;

      auto_ptr<TAO_LB_ReplicaInfo> safe_replica_info (replica_info);

      if (object_group_entry->replica_infos.insert (replica_info) != 0
          || location_entry->replica_infos.insert (replica_info) != 0)
        {
          // An Object of incorrect type was created.  Delete it, and
          // throw a NoFactory exception.
          factory->delete_object (replica_fcid.in (), ACE_TRY_ENV);
          ACE_CHECK;

          ACE_THROW (PortableGroup::ObjectNotCreated ());
        }

      replica_info->replica = replica;

      replica_info->factory_info = factory_info;  // Deep copy.

      replica_info->factory_creation_id = replica_fcid;

      replica_info->location_entry = location_entry;

      // No longer need to protect the allocated Replica_Info or the
      // Location map entry.
      (void) safe_replica_info.release ();
      (void) safe_location_entry.release ();
    }
}

void
TAO_LB_GenericFactory::get_ObjectId (
  CORBA::ULong fcid,
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class auto_ptr<TAO_LB_Location_Map_Entry>;
template class ACE_Auto_Basic_Ptr<TAO_LB_Location_Map_Entry>;

template class auto_ptr<TAO_LB_ObjectGroup_Map_Entry>;
template class ACE_Auto_Basic_Ptr<TAO_LB_ObjectGroup_Map_Entry>;

template class auto_ptr<TAO_LB_ReplicaInfo>;
template class ACE_Auto_Basic_Ptr<TAO_LB_ReplicaInfo>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate auto_ptr<TAO_LB_Location_Map_Entry>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_LB_Location_Map_Entry>

#pragma instantiate auto_ptr<TAO_LB_ObjectGroup_Map_Entry>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_LB_ObjectGroup_Map_Entry>

#pragma instantiate auto_ptr<TAO_LB_ReplicaInfo>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_LB_ReplicaInfo>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
