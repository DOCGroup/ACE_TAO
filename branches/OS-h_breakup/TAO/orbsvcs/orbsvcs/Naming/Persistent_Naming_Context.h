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
#include "ace/pre.h"

#include "Hash_Naming_Context.h"
#include "Persistent_Entries.h"
#include "naming_export.h"

class TAO_Naming_Export TAO_Persistent_Bindings_Map : public TAO_Bindings_Map
{
  // = TITLE
  //     Provides hash-table-based persistent storage for
  //     name to object  bindings in a Naming Context.
  //
  // = DESCRIPTION
  //     Wrapper on top of ACE_Hash_Map_With_Allocator (which is a wrapper
  //     around ACE_Hash_Map_Manager).  Uses ACE_Allocator (allocating
  //     from persistent storage) to make bindings persistent and
  //     supports TAO_Bindings_Map interface.  Used by TAO_Persistent_Naming_Context.
public:

  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId, TAO_Persistent_IntId> HASH_MAP;
  // Underlying data structure - typedef for ease of use.

  // = Initialization and termination methods.

  TAO_Persistent_Bindings_Map (CORBA::ORB_ptr orb);
  // Constructor.

  int open (size_t hash_map_size,
            ACE_Allocator *alloc);
  // Allocate hash map of size <hash_map_size> from persistent storage
  // using the <alloc>.

  void set (HASH_MAP *map,
            ACE_Allocator *alloc);
  // The hash map has already been preallocated for us.  We just need
  // to set our data members take ownership of it.

  virtual ~TAO_Persistent_Bindings_Map (void);
  // Destructor.  Does not deallocate the hash map: if an instance of
  // this class goes out of scope, its hash_map remains in persistent storage.

  void destroy (void);
  // This method removes the hash map from persistent storage/frees up
  // the memory.  The hash map better be empty, since we are not
  // cleaning up the insides. (We could add <close> to clean entries,
  // but not the data inside the entries.

  // = Accessor methods.

  HASH_MAP *map (void);
  // Get a pointer to the underlying hash map.

  size_t total_size (void);
  // Return the size of the underlying hash table.

  virtual size_t current_size (void);
  // Return the size of the underlying hash table.

  // = Name bindings manipulation methods.

  virtual int bind (const char *id,
                    const char *kind,
                    CORBA::Object_ptr obj,
                    CosNaming::BindingType type);
  // Add a binding with the specified parameters to the table.
  // Return 0 on success and -1 on failure, 1 if there already is a
  // binding with <id> and <kind>.

  virtual int rebind (const char *id,
                      const char *kind,
                      CORBA::Object_ptr obj,
                      CosNaming::BindingType type);
  // Overwrite a binding containing <id> and <kind> (or create a new
  // one if one doesn't exist) with the specified parameters.  Return
  // 0 or 1 on success.  Return -1 or -2 on failure. (-2 is returned
  // if the new and old bindings differ in type).

  virtual int unbind (const char * id,
                      const char * kind);
  // Remove a binding containing <id> and <kind> from the table.
  // Return 0 on success and -1 on failure.

  virtual int find (const char * id,
                    const char * kind,
                    CORBA::Object_ptr & obj,
                    CosNaming::BindingType &type);
  // Find the binding containing <id> and <kind> in the table, and
  // pass binding's type and object back to the caller by reference.
  // Return 0 on success and -1 on failure.  Note: a 'duplicated' object
  // reference is assigned to <obj>, so the caller is responsible for
  // its deallocation.

protected:

  int open_helper (size_t hash_table_size,
                   void *buffer);
  // Helper to the <open> method.  By isolating placement new into a
  // separate method, we can deal with memory allocation failures more
  // efficiently.  If there is a problem in HASH_MAP constructor, we
  // can clean up preallocated space.

  int shared_bind (const char *id,
                   const char *kind,
                   CORBA::Object_ptr obj,
                   CosNaming::BindingType type,
                   int rebind);
  // Helper: factors common code from <bind> and <rebind>.

  ACE_Allocator *allocator_;
  // Pointer to the allocator we use to make bindings persistent.

  HASH_MAP *map_;
  // Pointer to the underlying hash map.

  CORBA::ORB_var orb_;
  // Pointer to the orb.  We need it to do string/object conversions.
};

class TAO_Persistent_Context_Index;

class TAO_Naming_Export TAO_Persistent_Naming_Context : public TAO_Hash_Naming_Context
{
  // = TITLE
  //     This class plays a role of a 'ConcreteImplementor' in the
  //     Bridge pattern architecture of the CosNaming::NamingContext implementation.
  //
  // = DESCRIPTION
  //     This class provides a persistent implementation of the
  //     NamingContext functionality, i.e., the state is preserved across
  //     process boundaries.  Derives from TAO_Hash_Naming_Context and
  //     uses TAO_Persistent_Bindings_Map to store name to object bindings.
  //
public:
  typedef TAO_Persistent_Bindings_Map::HASH_MAP HASH_MAP;
  // Underlying data structure - typedef for ease of use.

  // = Initialization and termination methods.

  TAO_Persistent_Naming_Context (PortableServer::POA_ptr poa,
                                 const char *poa_id,
                                 TAO_Persistent_Context_Index *context_index);
  // Constructor.  MUST be followed up by <init> to allocate the
  // underlying data structure from persistent storage!

  int init (size_t hash_table_size = ACE_DEFAULT_MAP_SIZE);
  // Allocate the underlying data structure from persistent storage.
  // Returns 0 on success and -1 on failure.

  TAO_Persistent_Naming_Context (PortableServer::POA_ptr poa,
                                 const char *poa_id,
                                 TAO_Persistent_Context_Index *context_index,
                                 HASH_MAP * map,
                                 ACE_UINT32 *counter);
  // Constructor that takes in preallocated data structure and takes
  // ownership of it.  This constructor is for 'recreating' servants
  // from persistent state.

  virtual ~TAO_Persistent_Naming_Context (void);
  // Destructor.

  // = Utility methods.
  static CosNaming::NamingContext_ptr make_new_context (PortableServer::POA_ptr poa,
                                                        const char *poa_id,
                                                        size_t context_size,
                                                        TAO_Persistent_Context_Index *ind
                                                        ACE_ENV_ARG_DECL);
  // This utility method factors out the code needed to create a new
  // Persistent Naming Context servant and activate it under the
  // specified POA with the specified id.  This function is static so
  // that the code can be used, both from inside the class (e.g., <new_context>),
  // and from outside (e.g., Naming_Utils.cpp).

  // = Methods not implemented in TAO_Hash_Naming_Context.

  virtual CosNaming::NamingContext_ptr new_context (ACE_ENV_SINGLE_ARG_DECL);
  // This operation returns a new naming context implemented by the
  // same naming server in which the operation was invoked.  The
  // context is not bound.

  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out &bl,
                     CosNaming::BindingIterator_out &bi
                     ACE_ENV_ARG_DECL);
  // Returns at most the requested number of bindings <how_many> in
  // <bl>.  If the naming context contains additional bindings, they
  // are returned with a BindingIterator.  In the naming context does
  // not contain any additional bindings <bi> returned as null.

protected:

  void set_cleanup_level (int level);
  // Set <destroyed_> flag (inherited from TAO_Hash_Naming_Context) to
  // <level>.  Legal values for <destroyed_> are 0, 1, and 2.  The
  // values specify the extent of cleanup that should take place in the
  // context's destructor:
  // '0' - no cleanup (e.g., if the context goes out of scope, but
  // it's state is to remain in persistent storage);
  // '1' - free up the underlying data structure in persistent storage
  // (e.g., if the initialization of this context was only partially completed
  // due to some failures, and we need to roll back);
  // '2' - free up the underlying data structure, and deregister this
  // naming context from its <index_> (e.g., if the context had
  // <destroy> method invoked and needs to be completely removed from existence).

  ACE_UINT32 *counter_;
  // Counter used for generation of POA ids for children Naming
  // Contexts.

  TAO_Persistent_Bindings_Map *persistent_context_;
  // A pointer to the underlying data structure used to store name
  // bindings. While our superclass (TAO_Hash_Naming_Context) also
  // maintains a pointer to the data structure, keeping this pointer
  // around saves us from the need to downcast when invoking
  // non-virtual methods.

  TAO_Persistent_Context_Index *index_;
  // A pointer to the index object of this naming service: it keeps
  // track of all the naming contexts created.  Every time we make a
  // new context or destroy one, we need to make an entry there.
  // Also, we get the allocator needed to initialize us from this guy.
};

#include "ace/post.h"
#endif /* TAO_PERSISTENT_NAMING_CONTEXT_H */
