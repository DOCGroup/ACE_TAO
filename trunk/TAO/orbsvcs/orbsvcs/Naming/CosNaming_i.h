/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   CosNaming_i.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu> and
//    Sergio Flores-Gaitan <sergio@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_NAMING_I_H
#define TAO_NAMING_I_H

#include "orbsvcs/CosNamingS.h"
#include "Entries.h"

// Forward declaration.
class TAO_BindingIterator;

class TAO_ORBSVCS_Export TAO_NamingContext : public POA_CosNaming::NamingContext
{
  // = TITLE
  //     This class implements the <NamingContext> interface that is
  //     part of the <CosNaming> IDL module.
  //
  // = DESCRIPTION
  //     Extensive comments can be found in the Naming IDL file.

public:
  typedef ACE_Hash_Map_Manager<TAO_ExtId, TAO_IntId, ACE_Null_Mutex>
          HASH_MAP;

  // = Initialization and termination methods.
  TAO_NamingContext (PortableServer::POA_ptr poa,
                     const char *poa_id,
                     size_t default_hash_table_size = ACE_DEFAULT_MAP_SIZE,
                     int root = 0);
  // Default constructor, which initializes the <size> of the table,
  // and sets a root flag.

  ~TAO_NamingContext (void);
  // destructor.

  // Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

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

  virtual CosNaming::NamingContext_ptr new_context (CORBA::Environment &IT_env);
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

  static int populate_binding (TAO_NamingContext::HASH_MAP::ENTRY *hash_entry,
                               CosNaming::Binding &b);
  // Helper function used by TAO_NamingContext and
  // TAO_BindingIterator: populate a binding <b> with info contained
  // in <hash_entry>.  Return 1 if everything went smoothly, 0 if an
  // allocation failed.

protected:
  // = These are the helper methods used by other methods.

  CosNaming::NamingContext_ptr get_context (const CosNaming::Name &name,
                                            CORBA::Environment &_env);
  // This is used by methods that need to resolve a compound name to
  // get the reference to the target context before performing the
  // actual operation (e.g. bind, unbind, etc.)  Takes a full name
  // (including the last component that doesn't need to be resolved)
  // Returns a pointer to the target context.

  int is_valid (CORBA::ULong len,
                CORBA::Environment &_env);
  // Check to see if we've got a valid name and raise an exception if
  // we don't.

  void list_helper (TAO_BindingIterator *&bind_iter,
                     TAO_NamingContext::HASH_MAP::ITERATOR *hash_iter,
                     CORBA::Environment &_env);
  // This is a helper function for <list> method.  It is necessary due
  // to inability to have 2 TRY blocks in one function.

  HASH_MAP context_;
  // This implementation of <NamingContext> uses <ACE_Hash_Map> for
  // storage and manipulation of name-object bindings.

  int root_;
  // Flag indicating whether the context is a root or not.  It allows
  // to do things like have <destroy> be a no-op on root context.
  // Values: 1 indicates root, 0 indicates not a root.

  ACE_Lock *lock_;
  // Lock to serialize access to the underlying data structure.  This
  // is a lock adapter that hides the type of lock, which may be a
  // null lock if the ORB decides threading is not necessary.

  PortableServer::POA_var poa_;
  // Implement a different <_default_POA>.

  u_long counter_;
  // This counter is used to generate names for children.

  size_t hash_table_size_;
  // Hash table size.

  ACE_CString poa_id_;
  // My POA Id.
};

class TAO_ORBSVCS_Export TAO_BindingIterator : public POA_CosNaming::BindingIterator
{
  // = TITLE
  //     This class implements the <BindingIterator> interface that is
  //     part of the <CosNaming> idl module.
  //
  // = DESCRIPTION
  //     <TAO_BindingIterator> constructor expects a pointer to a
  //     dynamically allocated hash map iterator. Destructor
  //     deallocates hash map iterator.
public:
  // = Intialization and termination methods.
  TAO_BindingIterator (TAO_NamingContext::HASH_MAP::ITERATOR *hash_iter,
                       PortableServer::POA_ptr poa,
                       ACE_Lock *lock);
  // Constructor.

  ~TAO_BindingIterator (void);
  // Destructor.

  // Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  CORBA::Boolean next_one (CosNaming::Binding_out b,
                           CORBA::Environment &IT_env);
  // This operation returns the next binding.  If there are no more
  // bindings false is returned.

  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosNaming::BindingList_out bl,
                         CORBA::Environment &IT_env);
  // This operation returns at most the requested number of bindings.

  void destroy (CORBA::Environment &IT_env);
  // This operation destroys the iterator.

private:
  TAO_NamingContext::HASH_MAP::ITERATOR *hash_iter_;
  // A pointer to the hash map iterator.

  ACE_Lock *lock_;
  // Lock passed on from <TAO_NamingContext> to serialize access to the
  // internal data structure.

  PortableServer::POA_var poa_;
  // Implement a different _default_POA()
};

#endif /* TAO_NAMING_I_H */
