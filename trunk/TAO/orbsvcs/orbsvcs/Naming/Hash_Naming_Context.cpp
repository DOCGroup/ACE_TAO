// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Hash_Naming_Context.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================
#include "ace/Auto_Ptr.h"
#include "Hash_Naming_Context.h"

ACE_RCSID(Naming, Hash_Naming_Context, "$Id:")

TAO_Hash_Naming_Context::TAO_Hash_Naming_Context (TAO_Naming_Context *interface,
                                                  PortableServer::POA_ptr poa,
                                                  const char *poa_id,
                                                  size_t default_hash_table_size,
                                                  int root)
  : poa_ (PortableServer::POA::_duplicate (poa)),
    counter_ (0),
    poa_id_ (poa_id),
    context_ (default_hash_table_size),
    root_ (root),
    lock_ (0),
    hash_table_size_ (default_hash_table_size),
    interface_ (interface)
{
  // Get the lock from the ORB, which knows what type is appropriate.
  // This method must be called AFTER the ORB has been initialized via
  // <CORBA::ORB_init> since otherwise <TAO_ORB_Core_instance> won't
  // work correctly...
  ACE_ALLOCATOR (this->lock_,
                 TAO_ORB_Core_instance ()->server_factory ()->create_servant_lock ());

  interface_->impl (this);
}

TAO_Hash_Naming_Context::~TAO_Hash_Naming_Context (void)
{
  delete this->lock_;
}

PortableServer::POA_ptr
TAO_Hash_Naming_Context::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CosNaming::NamingContext_ptr
TAO_Hash_Naming_Context::get_context (const CosNaming::Name &name,
                                      CORBA::Environment &ACE_TRY_ENV)
{
  // The naming context we will return.
  CosNaming::NamingContext_var result =
    CosNaming::NamingContext::_nil ();

  // Create compound name to be resolved (<name> - last component).
  // To avoid copying (and thus memory allocations), we can just reuse
  // <name>'s buffer, since we will not be modifying it.
  CORBA::ULong len = name.length ();
  CosNaming::Name comp_name (name.maximum (),
                             len - 1,
                             ACE_const_cast
                             (CosNaming::NameComponent*,
                              name.get_buffer ()));
  ACE_TRY
    {
      // Resolve the name.
      CORBA::Object_var context = resolve (comp_name,
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      // Try narrowing object reference to a context type.
      result = CosNaming::NamingContext::_narrow (context.in (),
                                                  ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, ex)
    {
      // Add the last component of the name, which was stripped before
      // the call to resolve.
      CORBA::ULong rest_len = ex.rest_of_name.length ();
      ex.rest_of_name.length (rest_len + 1);
      ex.rest_of_name[rest_len] = name[len - 1];

      if (ex.why == CosNaming::NamingContext::not_object)
        ex.why = CosNaming::NamingContext::missing_node;

      ACE_RETHROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (result._retn ());

  // This has to be outside the TRY block, othewise we'll catch our
  // own exception.
  if (CORBA::is_nil (result.in ()))
    {
      CosNaming::Name rest;
      rest.length (2);
      rest[0] = name[len - 2];
      rest[1] = name[len - 1];
      ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                        (CosNaming::NamingContext::not_context,
                         rest),
                        result._retn());
    }
  // Finally, if everything went smoothly, just return the resolved
  // context.
  return result._retn ();
}

void
TAO_Hash_Naming_Context::bind (const CosNaming::Name& n,
                               CORBA::Object_ptr obj,
                               CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_Lock,
                      ace_mon, *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var context =
        this->get_context (n, ACE_TRY_ENV);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      context->bind (simple_name, obj, ACE_TRY_ENV);
      ACE_CHECK;
    }
  // If we received a simple name, we need to bind it in this context.
  else
    {
      TAO_IntId entry (obj, CosNaming::nobject);
      TAO_ExtId name (n[0].id, n[0].kind);

      // Try binding the name.
      int result = this->context_.bind (name, entry);
      if (result == 1)
        ACE_THROW (CosNaming::NamingContext::AlreadyBound());

      // Something went wrong with the internal structure
      else if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());
    }
}

void
TAO_Hash_Naming_Context::rebind (const CosNaming::Name& n,
                                 CORBA::Object_ptr obj,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n, ACE_TRY_ENV);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      context->rebind (simple_name, obj, ACE_TRY_ENV);
      ACE_CHECK;
    }
  else
    {
      // If we received a simple name, we need to rebind it in this
      // context.

      TAO_IntId entry (obj, CosNaming::nobject);
      TAO_ExtId name (n[0].id, n[0].kind);
      TAO_IntId oldentry;
      TAO_ExtId oldname;

      // Try rebinding the name.
      int result = this->context_.rebind (name,
                                          entry,
                                          oldname,
                                          oldentry);
      // Something went wrong with the internal structure
      if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());
    }
}

void
TAO_Hash_Naming_Context::bind_context (const CosNaming::Name &n,
                                       CosNaming::NamingContext_ptr nc,
                                       CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n, ACE_TRY_ENV);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      context->bind_context (simple_name, nc, ACE_TRY_ENV);
      ACE_CHECK;
    }
  // If we received a simple name, we need to bind it in this context.
  else
    {
      TAO_IntId entry (nc, CosNaming::ncontext);
      TAO_ExtId name (n[0].id, n[0].kind);

      // Try binding the name.
      int result = this->context_.bind (name, entry);
      if (result == 1)
        ACE_THROW (CosNaming::NamingContext::AlreadyBound());

      // Something went wrong with the internal structure
      else if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());
    }
}

void
TAO_Hash_Naming_Context::rebind_context (const CosNaming::Name &n,
                                         CosNaming::NamingContext_ptr nc,
                                         CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n, ACE_TRY_ENV);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      context->rebind_context (simple_name, nc, ACE_TRY_ENV);
      ACE_CHECK;
    }
  // If we received a simple name, we need to rebind it in this
  // context.
  else
    {
      TAO_IntId entry (nc, CosNaming::ncontext);
      TAO_ExtId name (n[0].id, n[0].kind);
      TAO_IntId oldentry;
      TAO_ExtId oldname;

      // Try rebinding the name.
      if (this->context_.rebind (name,
                                 entry,
                                 oldname,
                                 oldentry) < 0)
        ACE_THROW (CORBA::INTERNAL ());
    }
}

CORBA::Object_ptr
TAO_Hash_Naming_Context::resolve (const CosNaming::Name& n,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_ptr result = CORBA::Object::_nil ();
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (result);

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    ACE_THROW_RETURN (CosNaming::NamingContext::InvalidName(),
                      result);

  // Resolve the first component of the name.
  TAO_ExtId name (n[0].id, n[0].kind);
  TAO_IntId entry;

  if (this->context_.find (name,
                           entry) == -1)
    ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                      (CosNaming::NamingContext::not_object,
                       n),
                      result);
  result = entry.ref_;

  //@@ Once TAO_TRY_THROW is implemented, the whole if (len > 1) can be put in a try block
  //with rethrow in CATCHANY.

  // If the name we have to resolve is a compound name we need to
  // recursively resolve it.
  if (len > 1)
    {
      CosNaming::NamingContext_var context =
        CosNaming::NamingContext::_nil ();
      if (entry.type_ == CosNaming::ncontext)
        {
          context = CosNaming::NamingContext::_narrow (result,
                                                       ACE_TRY_ENV);
          ACE_CHECK_RETURN (context._retn ());
        }
      else
        ACE_THROW_RETURN (CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_context,
                                                              n),
                          context._retn ());
      if (CORBA::is_nil (context.in ()))
        ACE_THROW_RETURN (CosNaming::NamingContext::NotFound (CosNaming::NamingContext::not_context,
                                                              n),
                          context._retn ());
      else
        {
          // Recursively call resolve on <n> without the first component.

          // We need a name just like <n> but without the first
          // component.  Instead of copying data we can reuse <n>'s
          // buffer since we will only be using it for 'in' parameters
          // (no modifications).
          CosNaming::Name rest_of_name
            (n.maximum () - 1,
             n.length () - 1,
             ACE_const_cast (CosNaming::NameComponent*,
                             n.get_buffer ())
             + 1);
          // If there are any exceptions, they will propagate up.
          return context->resolve (rest_of_name,
                                   ACE_TRY_ENV);
        }
    }
  // If the name we had to resolve was simple, we just need to return
  // the result.  Since we don't want the result to be destroyed when
  // this method returns we need to duplicate it.
  return CORBA::Object::_duplicate (result);
}

void
TAO_Hash_Naming_Context::unbind (const CosNaming::Name& n,
                                 CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_Lock, ace_mon, *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Get the length of the name.
  CORBA::ULong len = n.length ();

  // Check for invalid name.
  if (len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the unbinding should take place, then perform the unbinding
  // on target context.
  if (len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n, ACE_TRY_ENV);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[len - 1];
      context->unbind (simple_name, ACE_TRY_ENV);
      ACE_CHECK;
    }
  // If we received a simple name, we need to unbind it in this
  // context.
  else
    {
      TAO_ExtId name (n[0].id, n[0].kind);
      // Try unbinding the name.
      if (this->context_.unbind (name) == -1)
        ACE_THROW (CosNaming::NamingContext::NotFound
                   (CosNaming::NamingContext::not_object,
                    n));
    }
}

CosNaming::NamingContext_ptr
TAO_Hash_Naming_Context::new_context (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  TAO_Hash_Naming_Context *c_impl = 0;
  TAO_Naming_Context *c = 0;
  CosNaming::NamingContext_var result;
  char poa_id[BUFSIZ];
  ACE_OS::sprintf (poa_id,
                   "%s_%ld",
                   this->poa_id_.c_str (),
                   this->counter_++);

  ACE_NEW_THROW_EX (c,
                    TAO_Naming_Context,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Put <c> into the auto pointer temporarily, in case next
  // allocation fails.
  ACE_Auto_Basic_Ptr<TAO_Naming_Context> temp (c);

  ACE_NEW_THROW_EX (c_impl,
                    TAO_Hash_Naming_Context (c,
                                             poa_.in (),
                                             poa_id,
                                             this->hash_table_size_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (result._retn ());

  // Allocation succeeded, get rid of auto pointer.
  temp.release ();

  ACE_TRY
    {
      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id);

      this->poa_->activate_object_with_id (id.in (),
                                           c,
                                           ACE_TRY_ENV);
      ACE_TRY_CHECK;

      result = c->_this (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      delete c;
      ACE_RETHROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (result._retn ());

  return result._retn ();
}

CosNaming::NamingContext_ptr
TAO_Hash_Naming_Context::bind_new_context (const CosNaming::Name& n,
                                           CORBA::Environment &ACE_TRY_ENV)
{
  ACE_GUARD_THROW_EX (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  CosNaming::NamingContext_var result =
    CosNaming::NamingContext::_nil ();

  result = new_context (ACE_TRY_ENV);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  ACE_TRY
    {
      bind_context (n,
                    result.in (),
                    ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      {
        ACE_DECLARE_NEW_CORBA_ENV;
        result->destroy (ACE_TRY_ENV);
      }
      ACE_RETHROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  return result._retn ();
}

void
TAO_Hash_Naming_Context::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  {
    ACE_GUARD_THROW_EX (ACE_Lock,
                        ace_mon,
                        *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    if (this->context_.current_size () != 0)
      ACE_THROW (CosNaming::NamingContext::NotEmpty());

    if (this->root_ != 0)
      return; // Destroy is a no-op on a root context.
    else
      {
        PortableServer::POA_var poa =
          this->_default_POA ();

        PortableServer::ObjectId_var id =
          poa->servant_to_id (interface_,
                              ACE_TRY_ENV);
        ACE_CHECK;

        poa->deactivate_object (id.in (),
                                ACE_TRY_ENV);
        ACE_CHECK;
      }
  }

  // Let go of the lock and kill the interface, which will, in turn,
  // invoke our destructor.  We *must* be dynamically allocated for this to work right...
  // The code below, and in <destroy> in general, should NOT be
  // factored into the TAO_Naming_Context, because only the *concrete
  // implementation* knows how to clean up appropriately, e.g., we may
  // have one servant serving many objects and will not want to delete
  // the servant in <destroy>.

  //@@ Now that the POA has been fixed to work right, this is no
  // longer a correct approach to disposing off the servants.  This is
  // a temporary fix to avoid crashing (but with the memory leak), a
  // proper fix will be in the next release.

  //if (this->root_ == 0)
  //delete interface_;
}

void
TAO_Hash_Naming_Context::list (CORBA::ULong how_many,
                               CosNaming::BindingList_out &bl,
                               CosNaming::BindingIterator_out &bi,
                               CORBA::Environment &ACE_TRY_ENV)
{
  // Allocate nil out parameters in case we won't be able to complete
  // the operation.
  bi = CosNaming::BindingIterator::_nil ();
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Obtain a lock before we proceed with the operation.
  ACE_GUARD_THROW_EX (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Dynamically allocate hash map iterator.
  TAO_Hash_Naming_Context::HASH_MAP::ITERATOR *hash_iter = 0;
  ACE_NEW_THROW_EX (hash_iter,
                    TAO_Hash_Naming_Context::HASH_MAP::ITERATOR (context_),
                    CORBA::NO_MEMORY ());
  ACE_CHECK;

  // Number of bindings that will go into the BindingList.
  CORBA::ULong n;

  // A pointer to BindingIterator servant.
  TAO_Hash_Binding_Iterator *bind_iter = 0;

  // Calculate number of bindings that will go into bl.
  if (this->context_.current_size () > how_many)
    n = how_many;
  else
    n = this->context_.current_size ();

  // Use hash iterator to populate a BindingList with bindings.
  bl->length (n);

  TAO_Hash_Naming_Context::HASH_MAP::ENTRY *hash_entry;

  for (CORBA::ULong i = 0; i < n; i++)
    {
      hash_iter->next (hash_entry);
      hash_iter->advance ();

      if (populate_binding (hash_entry, bl[i]) == 0)
        {
          delete hash_iter;
          ACE_THROW (CORBA::NO_MEMORY());
        }
    }

  // Now we are done with the BindingsList, and we can follow up on
  // the iterator business.

  // If we do not need to pass back BindingIterator.
  if (this->context_.current_size () <= how_many)
    delete hash_iter;
  else
    {
      // Create a BindingIterator for return.

      bind_iter = new TAO_Hash_Binding_Iterator (hash_iter, this->poa_.in (), this->lock_);
      if (bind_iter == 0)
        {
          delete hash_iter;
          ACE_THROW (CORBA::NO_MEMORY ());
        }

      ACE_TRY
        {
          char poa_id[BUFSIZ];
          ACE_OS::sprintf (poa_id,
                           "%s_%ld",
                           this->poa_id_.c_str (),
                           this->counter_++);

          PortableServer::ObjectId_var id =
            PortableServer::string_to_ObjectId (poa_id);

          this->poa_->activate_object_with_id (id.in (),
                                               bind_iter,
                                               ACE_TRY_ENV);
          ACE_TRY_CHECK;

          bi = bind_iter->_this (ACE_TRY_ENV);
          ACE_TRY_CHECK;

        }
      ACE_CATCHANY
        {
          delete bind_iter;
          ACE_RETHROW;
        }
      ACE_ENDTRY;
      ACE_CHECK;
    }
}

TAO_Hash_Binding_Iterator::TAO_Hash_Binding_Iterator (TAO_Hash_Naming_Context::HASH_MAP::ITERATOR *hash_iter,
                                                      PortableServer::POA_ptr poa,
                                                      ACE_Lock *lock)
  : hash_iter_ (hash_iter),
    lock_ (lock),
    poa_ (PortableServer::POA::_duplicate (poa))

{
}

TAO_Hash_Binding_Iterator::~TAO_Hash_Binding_Iterator (void)
{
  delete hash_iter_;
}

// Return the Default POA of this Servant
PortableServer::POA_ptr
TAO_Hash_Binding_Iterator::_default_POA (CORBA::Environment &/*env*/)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CORBA::Boolean
TAO_Hash_Binding_Iterator::next_one (CosNaming::Binding_out b,
                                     CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Binding *binding;

  // Allocate a binding to be returned (even if there no more
  // bindings, we need to allocate an out parameter.)
  ACE_NEW_THROW_EX (binding,
                    CosNaming::Binding,
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  b = binding;

  ACE_GUARD_THROW_EX (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);
  // If there are no more bindings.
  if (hash_iter_->done ())
    return 0;
  else
    {
      TAO_Hash_Naming_Context::HASH_MAP::ENTRY *hash_entry;
      hash_iter_->next (hash_entry);

      if (TAO_Hash_Naming_Context::populate_binding (hash_entry, *binding) == 0)
        ACE_THROW_RETURN (CORBA::NO_MEMORY (), 0);

      hash_iter_->advance ();
      return 1;
    }
}

int
TAO_Hash_Naming_Context::populate_binding (TAO_Hash_Naming_Context::HASH_MAP::ENTRY *hash_entry,
                                           CosNaming::Binding &b)
{
  b.binding_type = hash_entry->int_id_.type_;
  b.binding_name.length (1);

  // Here we perform a check before assignment to make sure
  // CORBA::string_dup is not called on 0 pointer, since the spec does
  // not say what should happen in that case.
  if (hash_entry->ext_id_.id_.fast_rep () != 0)
    {
      b.binding_name[0].id =
        hash_entry->ext_id_.id_.fast_rep ();
      if (b.binding_name[0].id.in () == 0)
        return 0;
    }
  if (hash_entry->ext_id_.kind_.fast_rep () != 0)
    {
      b.binding_name[0].kind =
        hash_entry->ext_id_.kind_.fast_rep ();
      if (b.binding_name[0].kind.in () == 0)
        return 0;
    }
  return 1;
}

CORBA::Boolean
TAO_Hash_Binding_Iterator::next_n (CORBA::ULong how_many,
                                   CosNaming::BindingList_out bl,
                                   CORBA::Environment &ACE_TRY_ENV)
{
  // We perform an allocation before obtaining the lock so that an out
  // parameter is allocated in case we fail to obtain the lock.
  ACE_NEW_THROW_EX (bl,
                    CosNaming::BindingList (0),
                    CORBA::NO_MEMORY ());
  ACE_CHECK_RETURN (0);
  // Obtain a lock.
  ACE_GUARD_THROW_EX (ACE_Lock,
                      ace_mon,
                      *this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (0);

  // If there are no more bindings...
  if (hash_iter_->done ())
      return 0;
  else
    {
      // Initially assume that iterator has the requested number of
      // bindings.
      bl->length (how_many);

      TAO_Hash_Naming_Context::HASH_MAP::ENTRY *hash_entry;

      // Iterate and populate the BindingList.

      for (CORBA::ULong i = 0; i < how_many; i++)
        {
          hash_iter_->next (hash_entry);

          if (TAO_Hash_Naming_Context::populate_binding (hash_entry, bl[i]) == 0)
            ACE_THROW_RETURN (CORBA::NO_MEMORY (), 0);

          if (hash_iter_->advance () == 0)
            {
              // If no more bindings left, reset length to the actual
              // number of bindings populated and get out of the loop.
              bl->length (i + 1);
              break;
            }
        }
      return 1;
    }
}

void
TAO_Hash_Binding_Iterator::destroy (CORBA::Environment &ACE_TRY_ENV)
{
  {
    ACE_GUARD_THROW_EX (ACE_Lock,
                        ace_mon,
                        *this->lock_,
                        CORBA::INTERNAL ());
    ACE_CHECK;

    PortableServer::POA_var poa =
      this->_default_POA (ACE_TRY_ENV);
    ACE_CHECK;

    PortableServer::ObjectId_var id =
      poa->servant_to_id (this,
                          ACE_TRY_ENV);
    ACE_CHECK;

    poa->deactivate_object (id.in (),
                            ACE_TRY_ENV);
    ACE_CHECK;
  }

  // Let go of the lock and commit suicide: must have been dynamically allocated
  //@@ Now that the POA has been fixed to work right, this is no
  // longer a correct approach to disposing off the servants.  This is
  // a temporary fix to avoid crashing (but with the memory leak), a
  // proper fix will be in the next release.
  //delete this;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Auto_Basic_Ptr<TAO_Hash_Naming_Context>;
template class ACE_Hash_Map_Manager<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Manager_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Entry<TAO_ExtId, TAO_IntId>;
template class ACE_Hash<TAO_ExtId>;
template class ACE_Equal_To<TAO_ExtId>;
template class ACE_Hash_Map_Iterator_Base_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>;
template class ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Auto_Basic_Ptr<TAO_Hash_Naming_Context>
#pragma instantiate ACE_Hash_Map_Manager<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Manager_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Entry<TAO_ExtId, TAO_IntId>
#pragma instantiate ACE_Hash<TAO_ExtId>
#pragma instantiate ACE_Equal_To<TAO_ExtId>
#pragma instantiate ACE_Hash_Map_Iterator_Base_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>
#pragma instantiate ACE_Hash_Map_Reverse_Iterator_Ex<TAO_ExtId, TAO_IntId, ACE_Hash<TAO_ExtId>, ACE_Equal_To<TAO_ExtId>, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
