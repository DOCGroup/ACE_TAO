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
  PortableServer::POA_ptr poa)
  : destroyed_ (false),
    context_ (context),
    hash_iter_ (hash_iter),
    poa_ (PortableServer::POA::_duplicate (poa))

{
}

template <class ITERATOR, class TABLE_ENTRY>
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::~TAO_Bindings_Iterator (void)
{
  delete hash_iter_;

  // Since we are going away, decrement the reference count on the
  // Naming Context we were iterating over.
  context_->interface ()->_remove_ref ();
}

// Return the Default POA of this Servant
template <class ITERATOR, class TABLE_ENTRY> PortableServer::POA_ptr
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::_default_POA ()
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

template <class ITERATOR, class TABLE_ENTRY> CORBA::Boolean
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::next_one (
    CosNaming::Binding_out b)
{
  CosNaming::Binding *binding = 0;

  // Allocate a binding to be returned (even if there no more
  // bindings, we need to allocate an out parameter.)
  ACE_NEW_THROW_EX (binding,
                    CosNaming::Binding,
                    CORBA::NO_MEMORY ());

  b = binding;

  // Check to make sure this object is still valid.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // If the context we are iterating over has been destroyed,
  // self-destruct.
  if (context_->destroyed ())
    {
      destroy ();

      throw CORBA::OBJECT_NOT_EXIST ();
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
      ACE_READ_GUARD_THROW_EX (TAO_SYNCH_RW_MUTEX,
                               ace_mon,
                               this->context_->lock (),
                               CORBA::INTERNAL ());

      // Return a binding.
      TABLE_ENTRY *hash_entry = 0;
      hash_iter_->next (hash_entry);

      if (populate_binding (hash_entry, *binding) == 0)
        throw CORBA::NO_MEMORY ();

      hash_iter_->advance ();
      return 1;
    }
}

template <class ITERATOR, class TABLE_ENTRY> CORBA::Boolean
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::next_n (
    CORBA::ULong how_many,
    CosNaming::BindingList_out bl)
{
  // We perform an allocation before obtaining the lock so that an out
  // parameter is allocated in case we fail to obtain the lock.
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());

  // Check to make sure this object is still valid.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // If the context we are iterating over has been destroyed,
  // self-destruct.
  if (context_->destroyed ())
    {
      destroy ();

      throw CORBA::OBJECT_NOT_EXIST ();
    }

  // Check for illegal parameter values.
  if (how_many == 0)
    throw CORBA::BAD_PARAM ();

  // If there are no more bindings...
  if (hash_iter_->done ())
      return 0;
  else
    {
      ACE_READ_GUARD_THROW_EX (TAO_SYNCH_RW_MUTEX,
                               ace_mon,
                               this->context_->lock (),
                               CORBA::INTERNAL ());

      // Initially assume that the iterator has the requested number of
      // bindings.
      bl->length (how_many);

      TABLE_ENTRY *hash_entry = 0;

      // Iterate and populate the BindingList.
      for (CORBA::ULong i = 0; i < how_many; i++)
        {
          hash_iter_->next (hash_entry);

          if (populate_binding (hash_entry, bl[i]) == 0)
            throw CORBA::NO_MEMORY ();

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
TAO_Bindings_Iterator<ITERATOR, TABLE_ENTRY>::destroy (void)
{
  // Check to make sure this object is still valid.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Mark the object invalid.
  this->destroyed_ = true;

  PortableServer::ObjectId_var id =
    poa_->servant_to_id (this);

  poa_->deactivate_object (id.in ());
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
