/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Naming_Context.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_NAMING_CONTEXT_H
#define TAO_NAMING_CONTEXT_H

#include "orbsvcs/CosNamingS.h"

class TAO_Naming_Context_Impl;

class TAO_ORBSVCS_Export TAO_Naming_Context : public POA_CosNaming::NamingContext
{
  // = TITLE
  //   This class plays a role of the 'Abstraction' (aka 'Interface')
  //   in the Bridge pattern architecture for the Naming Service implementation.
  //
  // = DESCRIPTION
  //   This class simply forwards all client requests to a concrete
  //   Naming Service implementation through its <impl_> pointer.  See
  //   README file for more info.  Comments for the idl methods
  //   describe methods semantics - actual actions are carried out by
  //   concrete implementors.

public:

  // = Initialization and termination methods.
  TAO_Naming_Context (TAO_Naming_Context_Impl *impl);
  // Constructor - initialize <impl_> with a concrete implementation.

  ~TAO_Naming_Context (void);
  // destructor.

  // = CosNaming::NamingContext idl interface methods.

  virtual void bind (const CosNaming::Name &n,
                     CORBA::Object_ptr obj,
                     CORBA::Environment &IT_env);
  // Create a binding for name <n> and object <obj> in the naming
  // context.  Compound names are treated as follows: ctx->bind (<c1;
  // c2; c3; cn>, obj) = (ctx->resolve (<c1; c2; cn-1>))->bind (<cn>,
  // obj) if the there already exists a binding for the specified
  // name, <AlreadyBound> exception is thrown.  Naming contexts should
  // be bound using <bind_context> and <rebind_context> in order to
  // participate in name resolution later.

  virtual void rebind (const CosNaming::Name &n,
                       CORBA::Object_ptr obj,
                       CORBA::Environment &IT_env);
  // This is similar to <bind> operation above, except for when the
  // binding for the specified name already exists in the specified
  // context.  In that case, the existing binding is replaced with the
  // new one.

  virtual void bind_context (const CosNaming::Name &n,
                             CosNaming::NamingContext_ptr nc,
                             CORBA::Environment &IT_env);
  // This is the version of <bind> specifically for binding naming
  // contexts, so that they will participate in name resolution when
  // compound names are passed to be resolved.

  virtual void rebind_context (const CosNaming::Name &n,
                               CosNaming::NamingContext_ptr nc,
                               CORBA::Environment &IT_env);
  // This is a version of <rebind> specifically for naming contexts,
  // so that they can participate in name resolution when compound
  // names are passed.

  virtual CORBA::Object_ptr resolve (const CosNaming::Name &n,
                                     CORBA::Environment &IT_env);
  // Return object reference that is bound to the name.  Compound name
  // resolve is defined as follows: ctx->resolve (<c1; c2; cn>) =
  // ctx->resolve (<c1; c2 cn-1>)->resolve (<cn>) The naming service
  // does not return the type of the object.  Clients are responsible
  // for "narrowing" the object to the appropriate type.

  virtual void unbind (const CosNaming::Name &n,
                       CORBA::Environment &IT_env);
  // Remove the name binding from the context.  When compound names
  // are used, unbind is defined as follows: ctx->unbind (<c1; c2;
  // cn>) = (ctx->resolve (<c1; c2; cn-1>))->unbind (<cn>)

  virtual CosNaming::NamingContext_ptr new_context (CORBA::Environment
                                                    &IT_env);
  // This operation returns a new naming context implemented by the
  // same naming server in which the operation was invoked.  The
  // context is not bound.

  virtual CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name &n,
                                                         CORBA::Environment &IT_env);
  // This operation creates a new context and binds it to the name
  // supplied as an argument.  The newly-created context is
  // implemented by the same server as the context in which it was
  // bound (the name argument excluding the last component).

  virtual void destroy (CORBA::Environment &IT_env);
  // Delete the naming context.  NOTE: the user should <unbind> any
  // bindings in which the given context is bound to some names before
  // invoking <destroy> operation on it.
  // NOTE: this operation is a no-op on the root context.

  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out bl,
                     CosNaming::BindingIterator_out bi,
                     CORBA::Environment &IT_env);
  // Returns at most the requested number of bindings <how_many> in
  // <bl>.  If the naming context contains additional bindings, they
  // are returned with a BindingIterator.  In the naming context does
  // not contain any additional bindings <bi> returned as null.

  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);
  // Returns the Default POA of this Servant object

protected:

  TAO_Naming_Context_Impl *impl_;
  // A concrete implementor of the NamingContext functions.
};

class TAO_ORBSVCS_Export TAO_Naming_Context_Impl
{
  // = TITLE
  //   This abstract base class plays a role of the 'Implementor' in the Bridge
  //   pattern architecture for the implementation of the Naming Service.
  //
  // = DESCRIPTION
  //   Subclasses of TAO_NamingContext_Impl provide concrete
  //   implementations of the NamingContext functionality.

public:

  // = CosNaming::NamingContext idl interface methods.

  virtual void bind (const CosNaming::Name &n,
                     CORBA::Object_ptr obj,
                     CORBA::Environment &IT_env) = 0;
  // Create a binding for name <n> and object <obj> in the naming
  // context.  Compound names are treated as follows: ctx->bind (<c1;
  // c2; c3; cn>, obj) = (ctx->resolve (<c1; c2; cn-1>))->bind (<cn>,
  // obj) if the there already exists a binding for the specified
  // name, <AlreadyBound> exception is thrown.  Naming contexts should
  // be bound using <bind_context> and <rebind_context> in order to
  // participate in name resolution later.

  virtual void rebind (const CosNaming::Name &n,
                       CORBA::Object_ptr obj,
                       CORBA::Environment &IT_env) = 0;
  // This is similar to <bind> operation above, except for when the
  // binding for the specified name already exists in the specified
  // context.  In that case, the existing binding is replaced with the
  // new one.

  virtual void bind_context (const CosNaming::Name &n,
                             CosNaming::NamingContext_ptr nc,
                             CORBA::Environment &IT_env) = 0;
  // This is the version of <bind> specifically for binding naming
  // contexts, so that they will participate in name resolution when
  // compound names are passed to be resolved.

  virtual void rebind_context (const CosNaming::Name &n,
                               CosNaming::NamingContext_ptr nc,
                               CORBA::Environment &IT_env) = 0;
  // This is a version of <rebind> specifically for naming contexts,
  // so that they can participate in name resolution when compound
  // names are passed.

  virtual CORBA::Object_ptr resolve (const CosNaming::Name &n,
                                     CORBA::Environment &IT_env) = 0;
  // Return object reference that is bound to the name.  Compound name
  // resolve is defined as follows: ctx->resolve (<c1; c2; cn>) =
  // ctx->resolve (<c1; c2 cn-1>)->resolve (<cn>) The naming service
  // does not return the type of the object.  Clients are responsible
  // for "narrowing" the object to the appropriate type.

  virtual void unbind (const CosNaming::Name &n,
                       CORBA::Environment &IT_env) = 0;
  // Remove the name binding from the context.  When compound names
  // are used, unbind is defined as follows: ctx->unbind (<c1; c2;
  // cn>) = (ctx->resolve (<c1; c2; cn-1>))->unbind (<cn>)

  virtual CosNaming::NamingContext_ptr new_context (CORBA::Environment
                                                    &IT_env) = 0;
  // This operation returns a new naming context implemented by the
  // same naming server in which the operation was invoked.  The
  // context is not bound.

  virtual CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name &n,
                                                         CORBA::Environment &IT_env) = 0;
  // This operation creates a new context and binds it to the name
  // supplied as an argument.  The newly-created context is
  // implemented by the same server as the context in which it was
  // bound (the name argument excluding the last component).

  virtual void destroy (CORBA::Environment &IT_env) = 0;
  // Delete the naming context.  NOTE: the user should <unbind> any
  // bindings in which the given context is bound to some names before
  // invoking <destroy> operation on it.
  // NOTE: this operation is a no-op on the root context.

  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out &bl,
                     CosNaming::BindingIterator_out &bi,
                     CORBA::Environment &IT_env) = 0;
  // Returns at most the requested number of bindings <how_many> in
  // <bl>.  If the naming context contains additional bindings, they
  // are returned with a BindingIterator.  In the naming context does
  // not contain any additional bindings <bi> returned as null.

  virtual PortableServer::POA_ptr _default_POA (void) = 0;
  // Returns the Default POA of this Servant object
};


#endif /* TAO_NAMING_CONTEXT_H */
