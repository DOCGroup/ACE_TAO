// -*- C++ -*-


#include "HasherFactory.h"

#include "tao/debug.h"


ACE_RCSID (LoadBalancing,
           HasherFactory,
           "$Id$")


HasherFactory::HasherFactory (CORBA::ORB_ptr orb,
                              PortableServer::POA_ptr root_poa)
  : orb_ (CORBA::ORB::_duplicate (orb)),
    root_poa_ (PortableServer::POA::_duplicate (root_poa)),
    poa_ (),
    hasher_ (),
    lock_ (),
    initialized_ (0),
    next_fcid_ (0),
    table_ ()
{
}

HasherFactory::~HasherFactory (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      for (Table::iterator i = this->table_.begin ();
           i != this->table_.end ();
           ++i)
        {
          const PortableServer::ObjectId &oid = (*i).int_id_;

          // Deactivate the Hasher corresponding to the given ObjectId.
          this->poa_->deactivate_object (oid, ACE_TRY_ENV);
          ACE_TRY_CHECK;
        }

      this->poa_->destroy (1, 1, ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "~HasherFactory()");
    }
  ACE_ENDTRY;
}

CORBA::Object_ptr
HasherFactory::create_object (
    const char *type_id,
    const LoadBalancing::Criteria &the_criteria,
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
  // This factory is only designed to create Hasher objects.
  // However, a GenericFactory need not be limited to being able to
  // only create one type of object.

  // Since this factory is only capable of creating Hasher
  // objects, throw an exception if the type_id does not match that of
  // the Hasher.
  if (ACE_OS::strcmp (type_id,
                      "IDL:Hasher:1.0"
                      ) != 0)
    {
      LoadBalancing::Location the_location; // @@ Hack.
      ACE_THROW_RETURN (LoadBalancing::NoFactory (the_location,
                                                  type_id),
                        CORBA::Object::_nil ());
    }

  this->parse_criteria (the_criteria,
                        ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

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

  // Make sure this factory is initialized before using the POA.
  this->init (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  PortableServer::ObjectId_var oid =
    this->poa_->activate_object (&this->hasher_,
                                 ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  HasherFactory::FactoryCreationId fcid =
    this->bind_fcid (oid.in (), ACE_TRY_ENV);
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  *tmp <<= fcid;

  return this->poa_->id_to_reference (oid.in (), ACE_TRY_ENV);
}

void
HasherFactory::delete_object (
    const LoadBalancing::GenericFactory::FactoryCreationId
      &factory_creation_id,
    CORBA::Environment &ACE_TRY_ENV)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   LoadBalancing::ObjectNotFound))
{
  HasherFactory::FactoryCreationId fcid;

  /// Extract the FactoryCreationId.
  if (factory_creation_id >>= fcid)
    {
      // Get the object ID associated with the given
      // FactoryCreationId.
      PortableServer::ObjectId_var oid;

      this->unbind_fcid (fcid, oid.out (), ACE_TRY_ENV);
      ACE_CHECK;

      // Deactivate the Hasher.
      this->poa_->deactivate_object (oid.in (), ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      if (TAO_debug_level > 0)
        ACE_ERROR ((LM_ERROR,
                    "HasherFactory: Unable to extract "
                    "FactoryCreationId.\n"));

      ACE_THROW (LoadBalancing::ObjectNotFound ());  // @@ Correct?
    }
}

void
HasherFactory::init (CORBA::Environment &ACE_TRY_ENV)
{
  // Double-checked locking.
  if (this->initialized_ == 0)
    {
      ACE_GUARD (TAO_SYNCH_MUTEX,
                 factory_mon,
                 this->lock_);

      if (this->initialized_ == 1)
        return;

      // For this test, a single servant will do.  Create a POA with the
      // appropriate policies to allow this.
      CORBA::PolicyList policies (2);
      policies.length (2);

      policies[0] =
        this->root_poa_->create_id_uniqueness_policy (
          PortableServer::MULTIPLE_ID);

      policies[1] =
        this->root_poa_->create_id_assignment_policy (
          PortableServer::SYSTEM_ID);

      this->poa_ =
        this->root_poa_->create_POA ("Hasher_POA",
                                     PortableServer::POAManager::_nil (),
                                     policies,
                                     ACE_TRY_ENV);
      ACE_CHECK;

      policies[0]->destroy (ACE_TRY_ENV);
      ACE_CHECK;

      policies[1]->destroy (ACE_TRY_ENV);
      ACE_CHECK;

      // Successfully initialized the Factory.
      this->initialized_ = 1;
    }
}

void
HasherFactory::parse_criteria (
  const LoadBalancing::Criteria &criteria,
  CORBA::Environment &ACE_TRY_ENV)
{
  // We don't use any criteria!

  if (criteria.length () != 0)
    ACE_THROW (LoadBalancing::CannotMeetCriteria (criteria));
}

HasherFactory::FactoryCreationId
HasherFactory::bind_fcid (const PortableServer::ObjectId &oid,
                          CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                    factory_mon,
                    this->lock_,
                    0);

  HasherFactory::FactoryCreationId fcid = this->next_fcid_;

  // @note In the event the following bind() fails due to an existing
  //       entry with the same FactoryCreationId, we could loop until
  //       we find a free FactoryCreationId.  This could potentially
  //       be very time consuming since we'd be doing a linear
  //       search on over 4 billion (2^32) FactoryCreationIds, at the
  //       maximum.  We'd also have to be careful to avoid introducing
  //       an infinite loop, i.e. we'd need a stopping condition for
  //       loop.  For example, we could stop searching for a free
  //       FactoryCreationId once we hit the FactoryCreationId that
  //       was originally requested.
  int result = this->table_.bind (fcid, oid);
  if (result == 0)
    {
      // Only increment the next FactoryCreationId if the bind
      // succeeded.
      ++this->next_fcid_;

      return fcid;
    }

  ACE_THROW_RETURN (LoadBalancing::ObjectNotCreated (), 0);
}

void
HasherFactory::unbind_fcid (
  const HasherFactory::FactoryCreationId &fcid,
  PortableServer::ObjectId_out oid,
  CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD (TAO_SYNCH_MUTEX,
             factory_mon,
             this->lock_);

  PortableServer::ObjectId *noid;
  ACE_NEW_THROW_EX (noid,
                    PortableServer::ObjectId,
                    CORBA::NO_MEMORY (
                      CORBA::SystemException::_tao_minor_code (
                        TAO_DEFAULT_MINOR_CODE,
                        ENOMEM),
                      CORBA::COMPLETED_NO));
  ACE_CHECK;

  oid = noid;

  // Unbind the ObjectId/FactoryCreationId from the table.
  if (this->table_.unbind (fcid, *noid) != 0)
    ACE_THROW (LoadBalancing::ObjectNotFound ());

}

const char *
HasherFactory::replica_type_id (void)
{
  return "IDL:Hasher:1.0";
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class ACE_Hash_Map_Entry<HasherFactory::FactoryCreationId, PortableServer::ObjectId>;
template class ACE_Hash_Map_Manager_Ex<HasherFactory::FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Base_Ex<HasherFactory::FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<HasherFactory::FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<HasherFactory::FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Entry<HasherFactory::FactoryCreationId, PortableServer::ObjectId>
#pragma instantiate ACE_Hash_Map_Manager_Ex<HasherFactory::FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<HasherFactory::FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<HasherFactory::FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<HasherFactory::FactoryCreationId, PortableServer::ObjectId, ACE_Hash<ACE_UINT32>, ACE_Equal_To<ACE_UINT32>, ACE_Null_Mutex>

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
