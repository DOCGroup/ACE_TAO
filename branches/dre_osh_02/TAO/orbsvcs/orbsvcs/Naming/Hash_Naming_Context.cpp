// $Id$
// ============================================================================
//
// = LIBRARY
//    TAO_CosNaming
//
// = FILENAME
//   Hash_Naming_Context.cpp
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#include "Hash_Naming_Context.h"
#include "nsconf.h"
#include "ace/Auto_Ptr.h"

ACE_RCSID (Naming,
           Hash_Naming_Context,
           "$Id$")


// -------------------------------------------------

TAO_Bindings_Map::~TAO_Bindings_Map (void)
{
}

// -------------------------------------------------

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
TAO_Hash_Naming_Context::get_context (const CosNaming::Name &name
                                      ACE_ENV_ARG_DECL)
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
                             ACE_const_cast
                             (CosNaming::NameComponent*,
                              name.get_buffer ()));
  ACE_TRY
    {
      // Resolve the name.
      CORBA::Object_var context = resolve (comp_name
                                           ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Try narrowing object reference to the NamingContext type.
      result = CosNaming::NamingContext::_narrow (context.in ()
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CosNaming::NamingContext::NotFound, ex)
    {
      // Add the last component of the name, which was stripped before
      // the call to resolve.
      CORBA::ULong rest_len = ex.rest_of_name.length () + 1;
      ex.rest_of_name.length (rest_len);
      ex.rest_of_name[rest_len - 1] = name[name_len - 1];

      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  if (CORBA::is_nil (result.in ()))
    {
      CosNaming::Name rest;
      rest.length (2);
      rest[0] = name[name_len - 2];
      rest[1] = name[name_len - 1];
      ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                        (CosNaming::NamingContext::not_context,
                         rest),
                        CosNaming::NamingContext::_nil ());
    }
  // Finally, if everything went smoothly, just return the resolved
  // context.
  return result._retn ();
}

void
TAO_Hash_Naming_Context::bind (const CosNaming::Name& n,
                               CORBA::Object_ptr obj
                               ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        this->get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->bind (simple_name, obj ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
        ACE_THROW (CosNaming::NamingContext::AlreadyBound());

      // Something went wrong with the internal structure
      else if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());
    }
}

void
TAO_Hash_Naming_Context::rebind (const CosNaming::Name& n,
                                 CORBA::Object_ptr obj
                                 ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->rebind (simple_name, obj ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
        ACE_THROW (CORBA::INTERNAL ());

      else if (result == -2)
        ACE_THROW (CosNaming::NamingContext::NotFound
                   (CosNaming::NamingContext::not_object,
                    n));
    }
}

void
TAO_Hash_Naming_Context::bind_context (const CosNaming::Name &n,
                                       CosNaming::NamingContext_ptr nc
                                       ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Do not allow binding of nil context reference.
  if (CORBA::is_nil (nc))
    ACE_THROW (CORBA::BAD_PARAM ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the binding should take place, then perform the binding on
  // target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->bind_context (simple_name, nc ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
        ACE_THROW (CosNaming::NamingContext::AlreadyBound());

      // Something went wrong with the internal structure
      else if (result == -1)
        ACE_THROW (CORBA::INTERNAL ());
    }
}

void
TAO_Hash_Naming_Context::rebind_context (const CosNaming::Name &n,
                                         CosNaming::NamingContext_ptr nc
                                         ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the rebinding should take place, then perform the rebinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->rebind_context (simple_name, nc ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
        ACE_THROW (CORBA::INTERNAL ());

      else if (result == -2)
        ACE_THROW (CosNaming::NamingContext::NotFound
                   (CosNaming::NamingContext::not_context,
                    n));
    }
}

CORBA::Object_ptr
TAO_Hash_Naming_Context::resolve (const CosNaming::Name& n
                                  ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK_RETURN (CORBA::Object::_nil ());

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW_RETURN (CORBA::OBJECT_NOT_EXIST (),
                      CORBA::Object::_nil ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW_RETURN (CosNaming::NamingContext::InvalidName(),
                      CORBA::Object::_nil ());

  // Resolve the first component of the name.

  // Stores the binding type for the first name component.
  CosNaming::BindingType type;

  // Stores the object reference bound to the first name component.
  CORBA::Object_var result;

  if (this->context_->find (n[0].id,
                            n[0].kind,
                            result.out (),
                            type) == -1)
    ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                      (CosNaming::NamingContext::missing_node,
                       n),
                      CORBA::Object::_nil ());

  // If the name we have to resolve is a compound name, we need to
  // resolve it recursively.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        CosNaming::NamingContext::_nil ();

      if (type == CosNaming::ncontext)
        {
          // Narrow to NamingContext.
          context = CosNaming::NamingContext::_narrow (result.in ()
                                                       ACE_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (CORBA::Object::_nil ());
        }
      else
        // The first name component wasn't bound to a NamingContext.
        ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                          (CosNaming::NamingContext::not_context,
                           n),
                          CORBA::Object::_nil ());

      // If narrow failed...
      if (CORBA::is_nil (context.in ()))
        ACE_THROW_RETURN (CosNaming::NamingContext::NotFound
                          (CosNaming::NamingContext::not_context,
                           n),
                          CORBA::Object::_nil ());
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
             ACE_const_cast (CosNaming::NameComponent*,
                             n.get_buffer ())
             + 1);

          // If there are any exceptions, they will propagate up.
          return context->resolve (rest_of_name
                                   ACE_ENV_ARG_PARAMETER);
        }
    }
  // If the name we had to resolve was simple, we just need to return
  // the result.
  return result._retn ();
}

void
TAO_Hash_Naming_Context::unbind (const CosNaming::Name& n
                                 ACE_ENV_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX, ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  // Get the length of the name.
  CORBA::ULong name_len = n.length ();

  // Check for invalid name.
  if (name_len == 0)
    ACE_THROW (CosNaming::NamingContext::InvalidName());

  // If we received compound name, resolve it to get the context in
  // which the unbinding should take place, then perform the unbinding
  // on target context.
  if (name_len > 1)
    {
      CosNaming::NamingContext_var context =
        get_context (n ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      CosNaming::Name simple_name;
      simple_name.length (1);
      simple_name[0] = n[name_len - 1];
      context->unbind (simple_name ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
  // If we received a simple name, we need to unbind it in this
  // context.
  else
    if (this->context_->unbind (n[0].id,
                                n[0].kind) == -1)
      ACE_THROW (CosNaming::NamingContext::NotFound
                 (CosNaming::NamingContext::missing_node,
                  n));
}

CosNaming::NamingContext_ptr
TAO_Hash_Naming_Context::bind_new_context (const CosNaming::Name& n
                                           ACE_ENV_ARG_DECL)
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

  // Stores our new Naming Context.
  CosNaming::NamingContext_var result =
    CosNaming::NamingContext::_nil ();

  // Create new context.
  result = new_context (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  // Bind the new context to the name.
  ACE_TRY
    {
      bind_context (n,
                    result.in ()
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      // If the bind() operation fails we must destroy the recently
      // created context, should any exceptions be raised by the
      // destroy() operation we want to ignore them.
      {
        ACE_DECLARE_NEW_CORBA_ENV;
        ACE_TRY_EX(DESTROY)
          {
            result->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
            ACE_TRY_CHECK_EX(DESTROY);
          }
        ACE_CATCHANY
          {
          }
        ACE_ENDTRY;
      }
      // Re-raise the exception in bind_context()
      ACE_RE_THROW;
    }
  ACE_ENDTRY;
  ACE_CHECK_RETURN (CosNaming::NamingContext::_nil ());

  return result._retn ();
}

void
TAO_Hash_Naming_Context::destroy (ACE_ENV_SINGLE_ARG_DECL)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_RECURSIVE_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  // Check to make sure this object didn't have <destroy> method
  // invoked on it.
  if (this->destroyed_)
    ACE_THROW (CORBA::OBJECT_NOT_EXIST ());

  if (this->context_->current_size () != 0)
    ACE_THROW (CosNaming::NamingContext::NotEmpty());

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

      ACE_CHECK;

      poa->deactivate_object (id.in ()
                              ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
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
