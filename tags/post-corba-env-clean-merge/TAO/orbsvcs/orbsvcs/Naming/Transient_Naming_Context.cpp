// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Transient_Naming_Context.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================
#include "ace/Auto_Ptr.h"
#include "Transient_Naming_Context.h"
#include "Bindings_Iterator_T.h"

ACE_RCSID(Naming, Transient_Naming_Context, "$Id$")

int
TAO_Transient_Bindings_Map::unbind (const char *id,
                                    const char *kind)
{
  TAO_ExtId name (id, kind);
  return this->map_.unbind (name);
}

int
TAO_Transient_Bindings_Map::bind (const char *id,
                                  const char *kind,
                                  CORBA::Object_ptr obj,
                                  CosNaming::BindingType type)
{
  return this->shared_bind (id, kind, obj, type, 0);
}

int
TAO_Transient_Bindings_Map::rebind (const char *id,
                                    const char *kind,
                                    CORBA::Object_ptr obj,
                                    CosNaming::BindingType type)
{
  return this->shared_bind (id, kind, obj, type, 1);
}

int
TAO_Transient_Bindings_Map::find (const char *id,
                                  const char *kind,
                                  CORBA::Object_ptr & obj,
                                  CosNaming::BindingType &type)
{
  TAO_ExtId name (id, kind);
  TAO_IntId entry;

  if (this->map_.find (name,
                       entry) != 0)
    return -1;
  else
    {
      obj = CORBA::Object::_duplicate (entry.ref_);
      type = entry.type_;
      return 0;
    }
}

TAO_Transient_Bindings_Map::TAO_Transient_Bindings_Map (size_t hash_table_size)
  : map_ (hash_table_size)
{
}

TAO_Transient_Bindings_Map::~TAO_Transient_Bindings_Map (void)
{
}

TAO_Transient_Bindings_Map::HASH_MAP &
TAO_Transient_Bindings_Map::map (void)
{
  return map_;
}

size_t
TAO_Transient_Bindings_Map::current_size (void)
{
  return map_.current_size ();
}

size_t
TAO_Transient_Bindings_Map::total_size (void)
{
  return map_.total_size ();
}

int
TAO_Transient_Bindings_Map::shared_bind (const char * id,
                                         const char * kind,
                                         CORBA::Object_ptr obj,
                                         CosNaming::BindingType type,
                                         int rebind)
{
  TAO_ExtId new_name (id, kind);
  TAO_IntId new_entry (obj, type);
  TAO_IntId old_entry;

  if (rebind == 0)
    // Do a normal bind.
    return this->map_.bind (new_name, new_entry);

  else
    // Rebind.
    {
      // Check that types of old and new entries match.
      if (this->map_.find (new_name,
                           old_entry) == 0
          && type != old_entry.type_)
        return -2;

      else
        return this->map_.rebind (new_name, new_entry);
    }
}

TAO_Transient_Naming_Context::TAO_Transient_Naming_Context (PortableServer::POA_ptr poa,
                                                            const char *poa_id,
                                                            size_t hash_table_size)
  : TAO_Hash_Naming_Context (poa,
                             poa_id),
    counter_ (0),
    transient_context_ (0)
{
  ACE_NEW (this->transient_context_,
           TAO_Transient_Bindings_Map (hash_table_size));

  context_ = transient_context_;
}

TAO_Transient_Naming_Context::~TAO_Transient_Naming_Context (void)
{
}

CosNaming::NamingContext_ptr
TAO_Transient_Naming_Context::make_new_context (PortableServer::POA_ptr poa,
                                                const char *poa_id,
                                                size_t context_size
                                                TAO_ENV_ARG_DECL)
{
  // Store the stub we will return here.
  CosNaming::NamingContext_var result;

  // Put together a servant for the new Naming Context.

  TAO_Transient_Naming_Context *context_impl = 0;
  ACE_NEW_THROW_EX (context_impl,
                    TAO_Transient_Naming_Context (poa,
                                                  poa_id,
                                                  context_size),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Put <context_impl> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Transient_Naming_Context> temp (context_impl);

  TAO_Naming_Context *context = 0;
  ACE_NEW_THROW_EX (context,
                    TAO_Naming_Context (context_impl),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Let <implementation> know about it's <interface>.
  context_impl->interface (context);

  // Release auto pointer, and start using reference counting to
  // control our servant.
  temp.release ();
  PortableServer::ServantBase_var s = context;

  // Register the new context with the POA.
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (poa_id);

  poa->activate_object_with_id (id.in (),
                                context
                                TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (result._retn ());

  result = context->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  return result._retn ();
}

CosNaming::NamingContext_ptr
TAO_Transient_Naming_Context::new_context (TAO_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CosNaming::NamingContext::_nil ());

  // Generate a POA id for the new context.
  char poa_id[BUFSIZ];
  ACE_OS::sprintf (poa_id,
                   "%s_%d",
                   this->poa_id_.c_str (),
                   this->counter_++);

  // Create a new context.
  CosNaming::NamingContext_var result =
    make_new_context (this->poa_.in (),
                      poa_id,
                      this->transient_context_->total_size ()
                      TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  return result._retn ();
}

void
TAO_Transient_Naming_Context::list (CORBA::ULong how_many,
                                    CosNaming::BindingList_out &bl,
                                    CosNaming::BindingIterator_out &bi
                                    TAO_ENV_ARG_DECL)
{
  // Allocate nil out parameters in case we won't be able to complete
  // the operation.
  bi = CosNaming::BindingIterator::_nil ();
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Obtain a lock before we proceed with the operation.
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Dynamically allocate iterator for traversing the underlying hash map.
  HASH_MAP::ITERATOR *hash_iter = 0;
  ACE_NEW_THROW_EX (hash_iter,
                    HASH_MAP::ITERATOR (transient_context_->map ()),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Store <hash_iter temporarily in auto pointer, in case we'll have
  // some failures and throw an exception.
  ACE_Auto_Basic_Ptr<HASH_MAP::ITERATOR> temp (hash_iter);

  // Silliness below is required because of broken old g++!!!  E.g.,
  // without it, we could have just said HASH_MAP::ITERATOR everywhere we use ITER_DEF.
  typedef ACE_Hash_Map_Manager<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>::ITERATOR ITER_DEF;
  typedef ACE_Hash_Map_Manager<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>::ENTRY ENTRY_DEF;

  // Typedef to the type of BindingIterator servant for ease of use.
  typedef TAO_Bindings_Iterator<ITER_DEF, ENTRY_DEF>
    ITER_SERVANT;

  // A pointer to BindingIterator servant.
  ITER_SERVANT *bind_iter = 0;

  // Number of bindings that will go into the BindingList <bl>.
  CORBA::ULong n;

  // Calculate number of bindings that will go into <bl>.
  if (this->context_->current_size () > how_many)
    n = how_many;
  else
    n = this->context_->current_size ();

  // Use the hash map iterator to populate <bl> with bindings.
  bl->length (n);

  ENTRY_DEF *hash_entry;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      if (ITER_SERVANT::populate_binding (hash_entry, bl[i]) == 0)
          ACE_THROW (CORBA::NO_MEMORY());
    }

  // Now we are done with the BindingsList, and we can follow up on
  // the BindingIterator business.

  // If we do not need to pass back BindingIterator.
  if (this->context_->current_size () <= how_many)
    return;
  else
    {
      // Create a BindingIterator for return.
      ACE_NEW_THROW_EX (bind_iter,
                        ITER_SERVANT (this, hash_iter, this->poa_.in (), this->lock_),
                        CORBA::NO_MEMORY ());
      ACE_CHECK;

      // Release <hash_iter> from auto pointer, and start using
      // reference counting to control our servant.
      temp.release ();
      PortableServer::ServantBase_var iter = bind_iter;

      // Increment reference count on this Naming Context, so it doesn't get
      // deleted before the BindingIterator servant gets deleted.
      interface_->_add_ref (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;

      // Register with the POA.
      char poa_id[BUFSIZ];
      ACE_OS::sprintf (poa_id,
                       "%s_%d",
                       this->poa_id_.c_str (),
                       this->counter_++);
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id);

      this->poa_->activate_object_with_id (id.in (),
                                           bind_iter
                                           TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;

      bi = bind_iter->_this (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)  || \
    defined (ACE_HAS_GNU_REPO)

template class ACE_Hash_Map_Iterator_Base_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>;
template class ACE_Hash<TAO_ExtId>;
template class ACE_Equal_To<TAO_ExtId>;
template class ACE_Hash_Map_Entry<TAO_ExtId, TAO_IntId>;
template class TAO_Bindings_Iterator<ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>, ACE_Hash_Map_Entry<TAO_ExtId, TAO_IntId> >;
template class ACE_Auto_Basic_Ptr<TAO_Transient_Naming_Context>;
template class ACE_Auto_Basic_Ptr<TAO_Bindings_Iterator<ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>, ACE_Hash_Map_Entry<TAO_ExtId, TAO_IntId> > >;
template class ACE_Auto_Basic_Ptr<ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex> >;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>

#pragma instantiate ACE_Hash_Map_Manager<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash<TAO_ExtId>
#pragma instantiate ACE_Equal_To<TAO_ExtId>
#pragma instantiate ACE_Hash_Map_Entry<TAO_ExtId, TAO_IntId>
#pragma instantiate TAO_Bindings_Iterator<ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>, ACE_Hash_Map_Entry<TAO_ExtId, TAO_IntId> >
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Transient_Naming_Context>
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Bindings_Iterator<ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>, ACE_Hash_Map_Entry<TAO_ExtId, TAO_IntId> > >
#pragma instantiate ACE_Auto_Basic_Ptr<ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex> >
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
