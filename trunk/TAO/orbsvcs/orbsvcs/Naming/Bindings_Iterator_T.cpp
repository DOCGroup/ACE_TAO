// $Id$

#ifndef TAO_BINDINGS_ITERATOR_T_CPP
#define TAO_BINDINGS_ITERATOR_T_CPP

#include "orbsvcs/Naming/Bindings_Iterator_T.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

template <class ITERATOR, class TABLE_ENTRY>
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::TAO_Bindings_Iterator (
  TAO_Hash_Naming_Context *context,
  ITERATOR *hash_iter,
  PortableServer::POA_ptr poa,
  TAO_SYNCH_RECURSIVE_MUTEX &lock)
  : destroyed_ (0),
    context_ (context),
    hash_iter_ (hash_iter),
    lock_ (lock),
    poa_ (PortableServer::POA::_duplicate (poa))

{
}

template <class ITERATOR, class TABLE_ENTRY>
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::~TAO_Bindings_Iterator (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  delete hash_iter_;

  // Since we are going away, decrement the reference count on the
  // Naming Context we were iterating over.
  context_->interface ()->_remove_ref (ACE_ENV_SINGLE_ARG_PARAMETER);
}

// Return the Default POA of this Servant
template <class ITERATOR, class TABLE_ENTRY> PortableServer::POA_ptr
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::_default_POA (
  ACE_ENV_SINGLE_ARG_DECL_NOT_USED)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

template <class ITERATOR, class TABLE_ENTRY> CORBA::Boolean
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::next_one (
    CosNaming::Binding_out b
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  CosNaming::Binding *binding;

  // Allocate a binding to be returned (even if there no more
  // bindings, we need to allocate an out parameter.)
  ACE_NEW_THROW_EX (binding,
                    CosNaming::Binding,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);

  b = binding;

  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  // Check to make sure this object is still valid.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);

  // If the context we are iterating over has been destroyed,
  // self-destruct.
  if (context_->destroyed ())
    {
      destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
    }

  // If there are no more bindings.
  if (hash_iter_->done ())
    {
      b->binding_type = CosNaming::nobject;
      b->binding_name.length (0);
      return 0;
    }
  else
    {
      // Return a binding.
      TABLE_ENTRY *hash_entry = 0;
      hash_iter_->next (hash_entry);

      if (populate_binding (hash_entry, *binding) == 0)
        ACE_THROW_RETURN (CORBA::NO_MEMORY (), 0);

      hash_iter_->advance ();
      return 1;
    }
}

template <class ITERATOR, class TABLE_ENTRY> CORBA::Boolean
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::next_n (
    CORBA::ULong how_many,
    CosNaming::BindingList_out bl
    ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  // We perform an allocation before obtaining the lock so that an out
  // parameter is allocated in case we fail to obtain the lock.
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  // Obtain the lock.
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  // Check to make sure this object is still valid.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);

  // If the context we are iterating over has been destroyed,
  // self-destruct.
  if (context_->destroyed ())
    {
      destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK_RETURN (0);

      ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (), 0);
    }

  // Check for illegal parameter values.
  if (how_many == 0)
    ACE_THROW_RETURN (CORBA::BAD_PARAM (), 0);

  // If there are no more bindings...
  if (hash_iter_->done ())
      return 0;
  else
    {
      // Initially assume that the iterator has the requested number of
      // bindings.
      bl->length (how_many);

      TABLE_ENTRY *hash_entry = 0;

      // Iterate and populate the BindingList.
      for (CORBA::ULong i = 0; i < how_many; i++)
        {
          hash_iter_->next (hash_entry);

          if (populate_binding (hash_entry, bl[i]) == 0)
            ACE_THROW_RETURN (CORBA::NO_MEMORY (), 0);

          if (hash_iter_->advance () == 0)
            {
              // If no more bindings are left, reset length to the actual
              // number of bindings populated, and get out of the loop.
              bl->length (i + 1);
              break;
            }
        }
      return 1;
    }
}

template <class ITERATOR, class TABLE_ENTRY> void
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::destroy (ACE_ENV_SINGLE_ARG_DECL)
  ACE_THROW_SPEC ((CORBA::SystemException))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object is still valid.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Mark the object invalid.
  this->destroyed_ = 1;

  PortableServer::ObjectId_var id =
    poa_->servant_to_id (this
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  poa_->deactivate_object (id.in ()
                           ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

template <class ITERATOR, class TABLE_ENTRY> int
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::populate_binding (
  TABLE_ENTRY *hash_entry,
  CosNaming::Binding &b)
{
  b.binding_type = hash_entry->int_id_.type_;
  b.binding_name.length (1);

  // Here we perform a check before assignment to make sure
  // CORBA::string_dup is not called on 0 pointer, since the spec does
  // not say what should happen in that case.
  if (hash_entry->ext_id_.id () != 0)
    {
      b.binding_name[0].id =
        hash_entry->ext_id_.id ();
      if (b.binding_name[0].id.in () == 0)
        return 0;
    }
  if (hash_entry->ext_id_.kind () != 0)
    {
      b.binding_name[0].kind =
        hash_entry->ext_id_.kind ();
      if (b.binding_name[0].kind.in () == 0)
        return 0;
    }
  return 1;
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_BINDINGS_ITERATOR_T_CPP */
