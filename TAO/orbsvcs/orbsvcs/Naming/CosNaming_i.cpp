// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   CosNaming_i.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu> &
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#include "CosNaming_i.h"

NS_NamingContext::NS_NamingContext (size_t default_hash_table_size)
  : lock_ (0)
{
  // Deal with faults.
  if (context_.open (default_hash_table_size) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "NS_NamingContext"));
}

int
NS_NamingContext::init (void)
{
  // Get the lock from the ORB, which knows what type is appropriate.
  ACE_ALLOCATOR_RETURN (this->lock_,
                        TAO_ORB_Core_instance ()->server_factory ()->create_servant_lock (),
                        -1);
  return 0;
}
NS_NamingContext::~NS_NamingContext (void)
{
  delete this->lock_;
}

CosNaming::NamingContext_ptr
NS_NamingContext::get_context (const CosNaming::Name &name)
{
  // Create compound name to be resolved (<name> - last component).
  CORBA::Environment _env;
  CORBA::ULong len = name.length ();
  CosNaming::Name comp_name (name);
  comp_name.length (len - 1);

  // resolve
  CORBA::Object_var context = resolve (comp_name, _env);

  // Deal with exceptions in resolve: basically, add the last component
  // of the name to <rest_of_name> and rethrow.
  if (_env.exception () != 0)
    {
      _env.print_exception ("NS_NamingContext::get_context");
      return 0;
    }

  // Reference to a context from <resolve> cannot be nil because
  // cannot <bind> to a nil object.

  // Try narrowing object reference to a context type.
  CosNaming::NamingContext_ptr c;
  c = CosNaming::NamingContext::_narrow (context.in (), _env);
  if (_env.exception () != 0)
    {
      _env.print_exception ("NS_NamingContext::get_context - _narrow");
      return 0;
    }

  CosNaming::Name rest;
  rest.length (2);
  rest[0] = name[len - 2];
  rest[1] = name[len - 1];

  if (CORBA::is_nil (c))
    {
      CosNaming::Name rest;
      rest.length (2);
      rest[0] = name[len - 2];
      rest[1] = name[len - 1];
    }
  return c;
}

void
NS_NamingContext::bind (const CosNaming::Name& n,
                        CORBA::Object_ptr obj,
                        CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  int result = 0;
  _env.clear ();

  // get the length of the name
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    {
      _env.clear ();
      _env.exception (new CosNaming::NamingContext::InvalidName);
      return;
    }

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->bind (simple_name, obj, _env);
    }

  // If we received a simple name, we need to bind it in this context.
 else
    {
      NS_IntId entry (obj);
      NS_ExtId name (n[0].id, n[0].kind);

      // Try binding the name.
      result = context_.bind (name, entry);
      if (result == 1)
        {
          _env.clear ();
          _env.exception (new CosNaming::NamingContext::AlreadyBound);
          return;
        }
      // Something went wrong with the internal structure
      else if (result == -1)
        {
          _env.clear ();
          _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
          return;
        }

      ACE_DEBUG ((LM_DEBUG,
                  "bound: <%s,%s>\n",
                  n[0].id.in () == 0? "nil" : n[0].id.in (),
                  n[0].kind.in () == 0? "nil" : n[0].kind.in () ));
    }
}

void
NS_NamingContext::rebind (const CosNaming::Name& n,
                          CORBA::Object_ptr obj,
                          CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  int result = 0;
  _env.clear ();

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    {
      _env.clear ();
      _env.exception (new CosNaming::NamingContext::InvalidName);
      return;
    }

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->rebind (simple_name, obj, _env);
    }

  else
    {
      // If we received a simple name, we need to rebind it in this
      // context.

      NS_IntId entry (obj);
      NS_ExtId name (n[0].id, n[0].kind);
      NS_IntId oldentry;
      NS_ExtId oldname;

      // Try rebinding the name.
      result = context_.rebind (name, entry, oldname, oldentry);

      // Something went wrong with the internal structure
      if (result == -1)
        {
          _env.clear ();
          _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
          return;
        }
    }
}

void
NS_NamingContext::bind_context (const CosNaming::Name &n,
                                CosNaming::NamingContext_ptr nc,
                                CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  int result = 0;
  _env.clear ();

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    {
      _env.clear ();
      _env.exception (new CosNaming::NamingContext::InvalidName);
      return;
    }

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->bind_context (simple_name, nc, _env);
    }

  // If we received a simple name, we need to bind it in this context.
  else
    {
      CosNaming::NameComponent comp = n[0];
      NS_IntId entry (nc, CosNaming::ncontext);
      NS_ExtId name (n[0].id, n[0].kind);

      // Try binding the name.
      result = context_.bind (name, entry);
      if ( result == 1)
        {
          _env.clear ();
          _env.exception (new CosNaming::NamingContext::AlreadyBound);
          return;
        }
      // Something went wrong with the internal structure
      else if (result == -1)
        {
          _env.clear ();
          _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
          return;
        }
    }
}

void
NS_NamingContext::rebind_context (const CosNaming::Name &n,
                                  CosNaming::NamingContext_ptr nc,
                                  CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  _env.clear ();

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    {
      _env.clear ();
      _env.exception (new CosNaming::NamingContext::InvalidName);
      return;
    }

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->rebind_context (simple_name, nc, _env);
    }

  // if we received a simple name, we need to rebind it in this context.
  else
    {
      CosNaming::NameComponent comp = n[0];
      NS_IntId entry (nc, CosNaming::ncontext);
      NS_ExtId name (n[0].id, n[0].kind);
      NS_IntId oldentry;
      NS_ExtId oldname;

      // try rebinding the name.
      if (context_.rebind (name, entry, oldname, oldentry) < 0);
        // deal with consequences
    }
}

CORBA::Object_ptr
NS_NamingContext::resolve (const CosNaming::Name& n,
                           CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, CORBA::Object::_nil ());

  // clear the environment.
  _env.clear ();

  // get the length of the name
  CORBA::ULong len = n.length ();

  // check for invalid name.
  if (len == 0)
    {
      _env.clear ();
      _env.exception (new CosNaming::NamingContext::InvalidName);
      return CORBA::Object::_nil ();
    }

  ACE_DEBUG ((LM_DEBUG,
              "Trying to resolve <%s,%s>\n",
              n[0].id.in () == 0? "nil" : n[0].id.in (),
              n[0].kind.in () == 0? "nil" : n[0].kind.in ()));

  // Resolve the first component of the name.
  NS_ExtId name (n[0].id, n[0].kind);
  NS_IntId entry;
  if (context_.find (name, entry) == -1)
    {
      _env.clear ();
      _env.exception (new CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_object, n));
      return CORBA::Object::_nil ();
    }

  CORBA::Object_ptr item = entry.ref_;

  // if the name we have to resolve is a compound name
  // we need to recursively resolve it.
  if (len > 1)
    {
      CosNaming::NamingContext_var cont;
      if (entry.type_ == CosNaming::ncontext)
        {
          cont = CosNaming::NamingContext::_narrow (item, _env);
          if (_env.exception () != 0)
            {
              _env.print_exception ("NS_NamingContext::resolve");
              return CORBA::Object::_nil ();
            }
        }
      else
        {
          _env.clear ();
          _env.exception (new CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_context, n));
          return CORBA::Object::_nil ();
        }

      CosNaming::Name rest_of_name (len - 1);
      rest_of_name.length (len - 1);
      for (CORBA::ULong i = 1; i < len; i++)
        rest_of_name[i-1] = n[i];

      return cont->resolve (rest_of_name, _env);
    }

  ACE_DEBUG ((LM_DEBUG,
              "Resolved <%s,%s> to %08.8x\n",
              n[0].id.in () == 0? "nil" : n[0].id.in (),
              n[0].kind.in () == 0? "nil" : n[0].kind.in (),
              item));

  // If the name we had to resolve was simple, we just need to return
  // the result.
  return CORBA::Object::_duplicate (item);
}

void
NS_NamingContext::unbind (const CosNaming::Name& n,
                          CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  _env.clear ();

  // get the length of the name
  CORBA::ULong len = n.length ();

  // check for invalid name.
  if (len == 0)
    {
      _env.clear ();
      _env.exception (new CosNaming::NamingContext::InvalidName);
      return;
    }

  // If we received compound name, resolve it to get the context in
  // which the unbinding should take place, then perform the unbinding
  // on target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var cont = get_context (n);
      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      cont->unbind (simple_name, _env);
    }
  else
    {
      // If we received a simple name, we need to unbind it in this
      // context.

      NS_ExtId name (n[0].id, n[0].kind);
      // try unbinding the name.
      if (context_.unbind (name) == -1)
        {
          _env.clear ();
          _env.exception (new CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_object, n));
          return;
        }
    }
}

CosNaming::NamingContext_ptr
NS_NamingContext::new_context (CORBA::Environment &_env)
{
  NS_NamingContext * c = 0;

  // If allocation fails, the environment must be set to indicate
  // error.
  _env.clear ();
  _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_NEW_RETURN (c,
                  NS_NamingContext,
                  CosNaming::NamingContext::_nil ());

  // Clear the environment.
  _env.clear ();

  if (c->init () == -1)
    return CosNaming::NamingContext::_nil ();

  // (1) do we have to duplicate () the object reference????
  // (2) Also, how about memory leaks?
  return c->_this (_env);
}

CosNaming::NamingContext_ptr
NS_NamingContext::bind_new_context (const CosNaming::Name& n,
                                    CORBA::Environment &_env)
{
  NS_NamingContext *c = 0;

  // if allocation fails, the environment must be set to indicate error.
  _env.clear ();
  _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

  ACE_NEW_RETURN (c,
                  NS_NamingContext,
                  CosNaming::NamingContext::_nil ());

  // clear the environment.
  _env.clear ();

  if (c->init () == -1)
    return CosNaming::NamingContext::_nil ();

  this->bind_context (n,
                      c->_this (_env),
                      _env);

  // Release object if exception occurs.
  if (_env.exception () != 0)
    {
      delete c;
      return CosNaming::NamingContext::_nil ();
    }

  return c->_this (_env);
}

void
NS_NamingContext::destroy (CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  _env.clear ();

  if (context_.current_size () != 0)
    {
      _env.clear ();
      _env.exception (new CosNaming::NamingContext::NotEmpty);
      return;
    }
}

void
NS_NamingContext::list (CORBA::ULong how_many,
                        CosNaming::BindingList_out bl,
                        CosNaming::BindingIterator_out bi,
                        CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD (ACE_Lock, ace_mon, *this->lock_);

  _env.clear ();

  // Dynamically allocate hash map iterator.
  NS_NamingContext::HASH_MAP::ITERATOR *hash_iter =
    new NS_NamingContext::HASH_MAP::ITERATOR (context_);

  // Number of bindings that will go into the BindingList.
  CORBA::ULong n;

  // Number of bindings in the context is > <how_many> so need to
  // return a BindingIterator.
  if (context_.current_size () > how_many)
     {
       NS_BindingIterator *bind_iter;

       // if allocation fails, the environment must be set to indicate error.
       _env.clear ();
       _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
       ACE_NEW (bind_iter, NS_BindingIterator (hash_iter, this->lock_));

       // clear the environment.
       _env.clear ();

       bi = bind_iter->_this (_env);

       CosNaming::BindingIterator::_duplicate (bi);

       n = how_many;
    }
  else
    {
      // Number of bindings in the context is <= <how_many>,
      // so do not need to return a BindingIterator.
      bi = CosNaming::BindingIterator::_nil ();
      n = context_.current_size ();
    }

  // Use hash iterator to populate a BindingList with bindings.
  CosNaming::BindingList bindings;
  bindings.length (n);
  NS_NamingContext::HASH_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      bindings[i].binding_type = hash_entry->int_id_.type_;

      bindings[i].binding_name.length (1);
      bindings[i].binding_name[0].id =
        CORBA::string_dup (hash_entry->ext_id_.id_.fast_rep ());
      bindings[i].binding_name[0].kind =
        CORBA::string_dup (hash_entry->ext_id_.kind_.fast_rep ());
    }

  // check for memory failure.
  // if allocation fails, the environment must be set to indicate error.
  _env.clear ();
  _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
  ACE_NEW (bl, CosNaming::BindingList (bindings));

  // clear the environment.
  _env.clear ();

  // If did not allocate BindingIterator, deallocate hash map
  // iterator.
  if (context_.current_size () <= how_many)
    delete hash_iter;
}

NS_BindingIterator::NS_BindingIterator (NS_NamingContext::HASH_MAP::ITERATOR *hash_iter,
                                        ACE_Lock * lock)
  : hash_iter_ (hash_iter),
    lock_ (lock)
{
}

NS_BindingIterator::~NS_BindingIterator (void)
{
  delete hash_iter_;
}

CORBA::Boolean
NS_BindingIterator::next_one (CosNaming::Binding_out b,
                              CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  _env.clear ();

  if (hash_iter_->done ())
    {
      // We are allocating a binding here because it's an out parameter
      // and the server will try to free the memory

      // check for memory failure.
      // if allocation fails, the environment must be set to indicate error.
      _env.clear ();
      _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
      ACE_NEW_RETURN (b, CosNaming::Binding, 0);

      // clear the environment.
      _env.clear ();

      return 0;
    }
  else
    {
      // check for memory failure.
      // if allocation fails, the environment must be set to indicate error.
      _env.clear ();
      _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
      ACE_NEW_RETURN (b, CosNaming::Binding, 0);

      // clear the environment.
      _env.clear ();

      NS_NamingContext::HASH_MAP::ENTRY *hash_entry;
      hash_iter_->next (hash_entry);
      hash_iter_->advance ();

      b->binding_type = hash_entry->int_id_.type_;

      b->binding_name.length (1);
      b->binding_name[0].id =
        CORBA::string_dup (hash_entry->ext_id_.id_.fast_rep ());
      b->binding_name[0].kind =
        CORBA::string_dup (hash_entry->ext_id_.kind_.fast_rep ());

      return 1;
    }
}

CORBA::Boolean
NS_BindingIterator::next_n (CORBA::ULong how_many,
                            CosNaming::BindingList_out bl,
                            CORBA::Environment &_env)
{
  // if Guard fails to get the lock, the environment must be set.
  _env.clear ();
  _env.exception (new CORBA::UNKNOWN (CORBA::COMPLETED_NO));
  ACE_GUARD_RETURN (ACE_Lock, ace_mon, *this->lock_, 0);

  _env.clear ();

  if (hash_iter_->done ())
    {
      // check for memory failure.
      // if allocation fails, the environment must be set to indicate error.
      _env.clear ();
      _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));
      ACE_NEW_RETURN (bl, CosNaming::BindingList, 0);

      // clear the environment.
      _env.clear ();

      return 0;
    }
  else
    {
      // Statically allocate a BindingList.
      CosNaming::BindingList bindings;

      // Initially assume that iterator has the requested number of
      // bindings.
      bindings.length (how_many);

      // Iterate and populate the BindingList.
      NS_NamingContext::HASH_MAP::ENTRY *hash_entry;
      for (CORBA::ULong i = 0; i < how_many; i++)
        {
          hash_iter_->next (hash_entry);

          bindings[i].binding_type = hash_entry->int_id_.type_;

          bindings[i].binding_name.length (1);
          bindings[i].binding_name[0].id =
            CORBA::string_dup (hash_entry->ext_id_.id_.fast_rep ());
          bindings[i].binding_name[0].kind =
            CORBA::string_dup (hash_entry->ext_id_.kind_.fast_rep ());

          if (hash_iter_->advance () == 0)
            {
              // If no more bindings left, reset length to the actual
              // number of bindings populated and get out of the loop.
              bindings.length (i + 1);
              break;
            }
        }

      // if mem allocation fails, the environment must be set to throw
      //   an exception.
      _env.clear ();
      _env.exception (new CORBA::NO_MEMORY (CORBA::COMPLETED_NO));

      // Check for failed memory allocation.
      ACE_NEW_RETURN (bl, CosNaming::BindingList (bindings), 0);

      // if allocation went well, clear the exception.
      _env.clear ();

      return 1;
    }
}

void
NS_BindingIterator::destroy (CORBA::Environment &_env)
{
  // @@ Not sure if this is the correct way to do this.
  CORBA::release (this->_this (_env));
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Hash_Map_Manager<NS_ExtId, NS_IntId, ACE_Null_Mutex>;
template class  ACE_Hash_Map_Entry<NS_ExtId, NS_IntId>;
template class  ACE_Hash_Map_Iterator_Base<NS_ExtId, NS_IntId, ACE_Null_Mutex>;
template class  ACE_Hash_Map_Iterator<NS_ExtId, NS_IntId, ACE_Null_Mutex>;
template class  ACE_Hash_Map_Reverse_Iterator<NS_ExtId, NS_IntId, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Hash_Map_Manager<NS_ExtId, NS_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<NS_ExtId, NS_IntId>
#pragma instantiate ACE_Hash_Map_Iterator_Base<NS_ExtId, NS_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<NS_ExtId, NS_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<NS_ExtId, NS_IntId, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
