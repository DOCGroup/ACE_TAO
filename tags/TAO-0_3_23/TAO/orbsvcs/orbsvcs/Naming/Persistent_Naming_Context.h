/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Persistent_Naming_Context.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PERSISTENT_NAMING_CONTEXT_H
#define TAO_PERSISTENT_NAMING_CONTEXT_H

#include "Naming_Context.h"
#include "Persistent_Bindings_Map.h"

class TAO_Index;

class TAO_ORBSVCS_Export TAO_Persistent_Naming_Context : public TAO_Naming_Context_Impl
{
  // = TITLE
  //     This class plays a role of a 'ConcreteImplementor' in the
  //     Bridge pattern architecture of the Naming Service
  //     implementation.  This implementation of the NamingContext
  //     uses ACE_Hash_Map_Manager to store name bindings.
  //
  // = DESCRIPTION
  //     Extensive idl method descriptions can be found in Naming_Context.h

public:
  typedef TAO_Persistent_Bindings_Map::HASH_MAP HASH_MAP;

  // = Initialization and termination methods.
  TAO_Persistent_Naming_Context (TAO_Naming_Context *interface,
                                 TAO_Index *context_index,
                                 PortableServer::POA_ptr poa,
                                 const char *poa_id,
                                 size_t default_hash_table_size = ACE_DEFAULT_MAP_SIZE);
  // Default constructor.  Sets the implementation pointer of the interface.

  TAO_Persistent_Naming_Context (TAO_Naming_Context *interface,
                                 TAO_Index *context_index,
                                 PortableServer::POA_ptr poa,
                                 const char *poa_id,
                                 HASH_MAP * map,
                                 ACE_UINT32 *counter);
  // Constructor for recreation.

  virtual ~TAO_Persistent_Naming_Context (void);
  // destructor.

  int init (void);
  // Allocate the map.

  // = CosNaming::NamingContext idl interface methods.
  virtual void bind (const CosNaming::Name &n,
                     CORBA::Object_ptr obj,
                     CORBA::Environment &ACE_TRY_ENV);

  virtual void rebind (const CosNaming::Name &n,
                       CORBA::Object_ptr obj,
                       CORBA::Environment &ACE_TRY_ENV);

  virtual void bind_context (const CosNaming::Name &n,
                             CosNaming::NamingContext_ptr nc,
                             CORBA::Environment &ACE_TRY_ENV);

  virtual void rebind_context (const CosNaming::Name &n,
                               CosNaming::NamingContext_ptr nc,
                               CORBA::Environment &ACE_TRY_ENV);

  virtual CORBA::Object_ptr resolve (const CosNaming::Name &n,
                                     CORBA::Environment &ACE_TRY_ENV);

  virtual void unbind (const CosNaming::Name &n,
                       CORBA::Environment &ACE_TRY_ENV);

  virtual CosNaming::NamingContext_ptr new_context (CORBA::Environment &ACE_TRY_ENV);

  virtual CosNaming::NamingContext_ptr bind_new_context (const CosNaming::Name &n,
                                                         CORBA::Environment &ACE_TRY_ENV);

  virtual void destroy (CORBA::Environment &ACE_TRY_ENV);

  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out &bl,
                     CosNaming::BindingIterator_out &bi,
                     CORBA::Environment &ACE_TRY_ENV);

  virtual PortableServer::POA_ptr _default_POA (void);
  // Returns the Default POA of this Servant object

  // = Helper functions.
  static int populate_binding (TAO_Persistent_Naming_Context::HASH_MAP::ENTRY *hash_entry,
                               CosNaming::Binding &b);
  // Helper function used by TAO_Persistent_Naming_Context and
  // TAO_BindingIterator: populate a binding <b> with info contained
  // in <hash_entry>.  Return 1 if everything went smoothly, 0 if an
  // allocation failed.

  ACE_UINT32 *counter_;
  // Counter used for generation of POA ids for objects created by
  // this context.

  TAO_Persistent_Bindings_Map context_;
  // This implementation of <NamingContext> uses <ACE_Hash_Map> for
  // storage and manipulation of name-object bindings.

protected:
  // = These are the helper methods used by other methods.

  CosNaming::NamingContext_ptr get_context (const CosNaming::Name &name,
                                            CORBA::Environment &_env);
  // This is used by methods that need to resolve a compound name to
  // get the reference to the target context before performing the
  // actual operation (e.g. bind, unbind, etc.)  Takes a full name
  // (including the last component that doesn't need to be resolved)
  // Returns a pointer to the target context.

  TAO_Index *index_;
  // A pointer to the index object of this naming service: it keeps
  // track of all the naming contexts created.  Every time we make a
  // new context or destroy one, we need to make an entry there.
  // Also, we get the allocator needed to initialize us from this guy.

  PortableServer::POA_var poa_;
  // POA we are registered with.

  ACE_CString poa_id_;
  // ID with which we are registered with <poa_>.

  ACE_Lock *lock_;
  // Lock to serialize access to the underlying data structure.  This
  // is a lock adapter that hides the type of lock, which may be a
  // null lock if the ORB decides threading is not necessary.

  size_t hash_table_size_;
  // Hash table size.

  TAO_Naming_Context *interface_;
  // Pointer to the 'Abstraction' in the bridge pattern.  We need this
  // pointer so that we can clean up the Abstraction as necessary when <destroy>
  // method is called.  The reason we need to clean up as opposed to
  // the Abstraction itself is that we, the concrete implementation,  have the *knowledge* of how
  // to do this, e.g., we may have one servant serving many objects
  // and would not want to delete the servant in the <destroy> operation.
};

class TAO_ORBSVCS_Export TAO_Persistent_Binding_Iterator :
  public POA_CosNaming::BindingIterator
{
  // = TITLE
  //     This class implements the <BindingIterator> interface that is
  //     part of the <CosNaming> idl module based on the Hash_Map
  //     implementation for CosNaming::NamingContext.
  //
  // = DESCRIPTION
  //     <TAO_Persistent_Binding_Iterator> constructor expects a pointer to a
  //     dynamically allocated hash map iterator. Destructor
  //     deallocates hash map iterator.
public:
  // = Intialization and termination methods.
  TAO_Persistent_Binding_Iterator (TAO_Persistent_Naming_Context::HASH_MAP::ITERATOR *hash_iter,
                                   PortableServer::POA_ptr poa,
                                   ACE_Lock *lock);
  // Constructor.

  ~TAO_Persistent_Binding_Iterator (void);
  // Destructor.

  // Returns the Default POA of this Servant object
  virtual PortableServer::POA_ptr _default_POA (CORBA::Environment &env);

  CORBA::Boolean next_one (CosNaming::Binding_out b,
                           CORBA::Environment &ACE_TRY_ENV);
  // This operation returns the next binding.  If there are no more
  // bindings false is returned.

  CORBA::Boolean next_n (CORBA::ULong how_many,
                         CosNaming::BindingList_out bl,
                         CORBA::Environment &ACE_TRY_ENV);
  // This operation returns at most the requested number of bindings.

  void destroy (CORBA::Environment &ACE_TRY_ENV);
  // This operation destroys the iterator.

private:
  TAO_Persistent_Naming_Context::HASH_MAP::ITERATOR *hash_iter_;
  // A pointer to the hash map iterator.

  ACE_Lock *lock_;
  // Lock passed on from <TAO_NamingContext> to serialize access to the
  // internal data structure.

  PortableServer::POA_var poa_;
  // Implement a different _default_POA()
};

#endif /* TAO_PERSISTENT_NAMING_CONTEXT_H */
