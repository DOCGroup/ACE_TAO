// -*- C++ -*-

#include "HashReplicaFactory.h"

ACE_RCSID (LB_test,
           HashReplicaFactory,
           "$Id$")

CORBA::Object_ptr
HashReplicaFactory::create_object (
    const char *type_id,
    const TAO_LoadBalancing::Criteria &the_criteria,
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
  // This factory is only designed to create HashReplica objects.
  // However, a GenericFactory need not be limited to being able to
  // only create one type of object.

  // Since this factory is only capable of creating HashReplica
  // objects, throw an exception if the type_id does not match that of
  // the HashReplica.
  if (ACE_OS::strcmp (type_id,
                      this->replica_type_id_) != 0)
    ACE_THROW_RETURN (TAO_LoadBalancing::NoFactory (the_location,
                                                    type_id),
                      CORBA::Object::_nil ());

  this->parse_criteria (criteria,
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  HashReplica_i *hash_replica_servant = 0;
  ACE_NEW_THROW_EX (hash_replica_servant,
                    HashReplica_i,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Transfer ownership of the servant to the POA (HashReplica_i
  // inherits from PortableServer::RefCountServantBase).
  PortableServer::ServantBase_var tmp = hash_replica_servant;

  // Activate the HashReplica and obtain a reference to it.
  this->hash_replica_ =
    hash_replica_servant->_this (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  factory_creation_id = ...FILL IN CREATION ID...;

  return HashReplica::_duplicate (this->hash_replica_.in ());
}

void
HashReplicaFactory::delete_object (
    const TAO_LoadBalancing::GenericFactory::FactoryCreationId
      &factory_creation_id,
    ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancer::ObjectNotFound))
{
  if (factory_creation_id != this->factory_creation_id_)
    ACE_THROW (TAO_LoadBalancer::ObjectNotFound ());

  // Get the POA used when activating the HashReplica
  PortableServer::POA_var poa =
    this->_default_POA (ACE_TRY_ENV);
  ACE_CHECK;

  // Get the object ID associated with the HashReplica reference.
  PortableServer::ObjectId_var oid =
    poa->reference_to_id (this->hash_replica_.in (),
                          ACE_TRY_ENV);
  ACE_CHECK;

  // Now deactivate the HashReplica.
  poa->deactivate_object (oid.in (), ACE_TRY_ENV);
  ACE_CHECK;

  // Decrease the reference count on the HashReplica.
  this->hash_replica->_remove_ref ();
}

void
HashReplicaFactory::parse_criteria (
    const TAO_LoadBalancer::Criteria &criteria,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   TAO_LoadBalancing::InvalidCriteria,
                   TAO_LoadBalancing::InvalidProperty,
                   TAO_LoadBalancing::CannotMeetCriteria))
{
  // We don't use any criteria!

  if (criteria.length () != 0)
    ACE_THROW (TAO_LoadBalancer::CannotMeetCriteria (criteria));
}
