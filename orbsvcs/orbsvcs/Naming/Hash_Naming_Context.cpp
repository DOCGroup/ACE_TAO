//=============================================================================
/**
 *  @file   Hash_Naming_Context.cpp
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu> and Sergio Flores-Gaitan <sergio@cs.wustl.edu>
 */
//=============================================================================


#include "orbsvcs/Naming/Hash_Naming_Context.h"
#include "orbsvcs/Naming/nsconf.h"
#include "ace/Auto_Ptr.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Bindings_Map::~TAO_Bindings_Map (void)
{
}

TAO_Hash_Naming_Context::TAO_Hash_Naming_Context (PortableServer::POA_ptr poa,
                                                  const char *poa_id)
  : context_ (0),
    interface_ (0),
    destroyed_ (0),
    poa_ (PortableServer::POA::_duplicate (poa)),
    poa_id_ (poa_id)
{
}

void
TAO_Hash_Naming_Context::interface (TAO_Naming_Context *i)
{
  this->interface_ = i;
}

TAO_Hash_Naming_Context::~TAO_Hash_Naming_Context (void)
{
  delete context_;
}

PortableServer::POA_ptr
TAO_Hash_Naming_Context::_default_POA (void)
{
  return PortableServer::POA::_duplicate (this->poa_.in ());
}

CosNaming::NamingContext_ptr
TAO_Hash_Naming_Context::get_context (const CosNaming::Name &name)
{
  // Naming context we will return.
  CosNaming::NamingContext_var result =
    CosNaming::NamingContext::_nil ();

  // Create compound name to be resolved, i.e.,
  // (<name> - last component).  To avoid copying, we can just reuse
  // <name>'s buffer, since we will not be modifying it.
  CORBA::ULong name_len = name.length ();
  CosNaming::Name comp_name (name.maximum (),
                             name_len - 1,
                             const_cast<CosNaming::NameComponent*> (name.get_buffer ()));
  try
    {
      // Resolve the name.
      CORBA::Object_var context = resolve (comp_name);

      // Try narrowing object reference to the NamingContext type.
      result = CosNaming::NamingContext::_narrow (context.in ());
    }
  catch (CosNaming::NamingContext::NotFound& ex)
    {
      // Add the last component of the name, which was stripped before
      // the call to resolve.
      CORBA::ULong const rest_len = ex.rest_of_name.length () + 1;
      ex.rest_of_name.length (rest_len);
      ex.rest_of_name[rest_len - 1] = name[name_len - 1];

      throw;
    }

  if (CORBA::is_nil (result.in ()))
    {
      CosNaming::Name rest;
      rest.length (2);
      rest[0] = name[name_len - 2];
      rest[1] = name[name_len - 1];
      throw CosNaming::NamingContext::NotFound(
        CosNaming::NamingContext::not_context,
        rest);
    }
  // Finally, if everything went smoothly, just return the resolved
  // context.
  return result._retn ();
}

void
TAO_Hash_Naming_Context::bind (const CosNaming::Name& n, CORBA::Object_ptr obj)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Get the length of the name.
  CORBA::ULong const name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context = this->get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      try
        {
          context->bind (simple_name, obj);
        }
      catch (const CORBA::SystemException&)
        {
          throw CosNaming::NamingContext::CannotProceed(
            context.in (), simple_name);
        }
    }
  // If we received a simple name, we need to bind it in this context.
  else
    {
      // Try binding the name.
      int result = this->context_->bind (n[0].id,
                                        n[0].kind,
                                        obj,
                                        CosNaming::nobject);
      if (result == 1)
        throw CosNaming::NamingContext::AlreadyBound();

      // Something went wrong with the internal structure
      else if (result == -1)
        throw CORBA::INTERNAL ();
    }
}

void
TAO_Hash_Naming_Context::rebind (const CosNaming::Name& n,
                                 CORBA::Object_ptr obj)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Get the length of the name.
  CORBA::ULong const name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context = get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      try
        {
          context->rebind (simple_name, obj);
        }
      catch (const CORBA::SystemException&)
        {
          throw CosNaming::NamingContext::CannotProceed(
            context.in (), simple_name);
        }
    }
  else
    // If we received a simple name, we need to rebind it in this
    // context.
    {
      int result = this->context_->rebind (n[0].id,
                                           n[0].kind,
                                           obj,
                                           CosNaming::nobject);
      // Check for error conditions.
      if (result == -1)
        throw CORBA::INTERNAL ();

      else if (result == -2)
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_object, n);
    }
}

void
TAO_Hash_Naming_Context::bind_context (const CosNaming::Name &n,
                                       CosNaming::NamingContext_ptr nc)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Do not allow binding of nil context reference.
  if (CORBA::is_nil (nc))
    throw CORBA::BAD_PARAM ();

  // Get the length of the name.
  CORBA::ULong const name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context = get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      try
        {
          context->bind_context (simple_name, nc);
        }
      catch (const CORBA::SystemException&)
        {
          throw CosNaming::NamingContext::CannotProceed(
            context.in (), simple_name);
        }
    }
  // If we received a simple name, we need to bind it in this context.
  else
    {
      // Try binding the name.
      int result = this->context_->bind (n[0].id,
                                        n[0].kind,
                                        nc,
                                        CosNaming::ncontext);
      if (result == 1)
        throw CosNaming::NamingContext::AlreadyBound();

      // Something went wrong with the internal structure
      else if (result == -1)
        throw CORBA::INTERNAL ();
    }
}

void
TAO_Hash_Naming_Context::rebind_context (const CosNaming::Name &n,
                                         CosNaming::NamingContext_ptr nc)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Get the length of the name.
  CORBA::ULong const name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      try
        {
          context->rebind_context (simple_name, nc);
        }
      catch (const CORBA::SystemException&)
        {
          throw CosNaming::NamingContext::CannotProceed(
            context.in (), simple_name);
        }
    }
  else
    // If we received a simple name, we need to rebind it in this
    // context.
    {
      int result = this->context_->rebind (n[0].id,
                                           n[0].kind,
                                           nc,
                                           CosNaming::ncontext);
      // Check for error conditions.
      if (result == -1)
        throw CORBA::INTERNAL ();

      else if (result == -2)
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);
    }
}

CORBA::Object_ptr
TAO_Hash_Naming_Context::resolve (const CosNaming::Name& n)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Get the length of the name.
  CORBA::ULong const name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // Resolve the first component of the name.

  // Stores the binding type for the first name component.
  CosNaming::BindingType type;

  // Stores the object reference bound to the first name component.
  CORBA::Object_var result;

  if (this->context_->find (n[0].id,
                            n[0].kind,
                            result.out (),
                            type) == -1)
    throw CosNaming::NamingContext::NotFound(
      CosNaming::NamingContext::missing_node,
      n);

  // If the name we have to resolve is a compound name, we need to
  // resolve it recursively.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        CosNaming::NamingContext::_nil ();

      if (type == CosNaming::ncontext)
        {
          // Narrow to NamingContext.
          context = CosNaming::NamingContext::_narrow (result.in ());
        }
      else
        // The first name component wasn't bound to a NamingContext.
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);

      // If narrow failed...
      if (CORBA::is_nil (context.in ()))
        throw CosNaming::NamingContext::NotFound(
          CosNaming::NamingContext::not_context,
          n);
      else
        {
          // Successfully resolved the first name component, need to
          // recursively call resolve on <n> without the first component.

          // We need a name just like <n> but without the first
          // component.  Instead of copying data we can reuse <n>'s
          // buffer since we will only be using it for 'in' parameters
          // (no modifications).
          CosNaming::Name rest_of_name
            (n.maximum () - 1,
             n.length () - 1,
             const_cast<CosNaming::NameComponent*> (n.get_buffer ())
             + 1);

          // If there are any exceptions, they will propagate up.
          try
            {
              CORBA::Object_ptr resolved_ref;
              resolved_ref = context->resolve (rest_of_name);
              return resolved_ref;
            }
          catch (const CORBA::SystemException&)
            {
              throw CosNaming::NamingContext::CannotProceed(
                context.in (), rest_of_name);
            }
        }
    }
  // If the name we had to resolve was simple, we just need to return
  // the result.
  return result._retn ();
}

void
TAO_Hash_Naming_Context::unbind (const CosNaming::Name& n)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Get the length of the name.
  CORBA::ULong const name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // If we received compound name, resolve it to get the context in
  // which the unbinding should take place, then perform the unbinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      try
        {
          context->unbind (simple_name);
        }
      catch (const CORBA::SystemException&)
        {
          throw CosNaming::NamingContext::CannotProceed(
            context.in (), simple_name);
        }
    }
  // If we received a simple name, we need to unbind it in this
  // context.
  else
    if (this->context_->unbind (n[0].id,
                                n[0].kind) == -1)
      throw CosNaming::NamingContext::NotFound(
        CosNaming::NamingContext::missing_node, n);
}

CosNaming::NamingContext_ptr
TAO_Hash_Naming_Context::bind_new_context (const CosNaming::Name& n)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    throw CosNaming::NamingContext::InvalidName();

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the operation on
  // target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n);

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      return context->bind_new_context (simple_name);
    }

  // If we received a simple name, we need to bind it in this context.

  // Stores our new Naming Context.
  CosNaming::NamingContext_var result =
    CosNaming::NamingContext::_nil ();

  // Create new context.
  result = new_context ();

  // Bind the new context to the name.
  try
    {
      bind_context (n, result.in ());
    }
  catch (const CORBA::Exception&)
    {
      // If the bind() operation fails we must destroy the recently
      // created context, should any exceptions be raised by the
      // destroy() operation we want to ignore them.
      {
        try
          {
            result->destroy ();
          }
        catch (const CORBA::Exception&)
          {
          }
      }
      // Re-raise the exception in bind_context()
      throw;
    }
  return result._retn ();
}

void
TAO_Hash_Naming_Context::destroy (void)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    throw CORBA::OBJECT_NOT_EXIST ();

  if (this->context_->current_size () != 0)
    throw CosNaming::NamingContext::NotEmpty();

  // Destroy is a no-op on a root context.
  if (root ())
    return;

  else
    {
      this->destroyed_ = 2;

      // Remove self from POA.  Because of reference counting, the POA
      // will automatically delete the servant when all pending requests
      // on this servant are complete.

      PortableServer::POA_var poa =
        this->_default_POA ();

      PortableServer::ObjectId_var id =
        PortableServer::string_to_ObjectId (poa_id_.fast_rep ());

      poa->deactivate_object (id.in ());
    }
}

int
TAO_Hash_Naming_Context::root (void)
{
  return (ACE_OS::strcmp (this->poa_id_.fast_rep (),
                          TAO_ROOT_NAMING_CONTEXT) == 0);
}

int
TAO_Hash_Naming_Context::destroyed (void)
{
  return this->destroyed_;
}

TAO_Naming_Context *
TAO_Hash_Naming_Context::interface (void)
{
  return this->interface_;
}

TAO_END_VERSIONED_NAMESPACE_DECL
