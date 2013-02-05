// -*- C++ -*-

//=============================================================================
/**
 *  @file   Persistent_Naming_Context.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PERSISTENT_NAMING_CONTEXT_H
#define TAO_PERSISTENT_NAMING_CONTEXT_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/Hash_Naming_Context.h"
#include "orbsvcs/Naming/Persistent_Entries.h"
#include "orbsvcs/Naming/naming_serv_export.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Persistent_Bindings_Map
 *
 * @brief Provides hash-table-based persistent storage for
 * name to object  bindings in a Naming Context.
 *
 * Wrapper on top of ACE_Hash_Map_With_Allocator (which is a wrapper
 * around ACE_Hash_Map_Manager).  Uses ACE_Allocator (allocating
 * from persistent storage) to make bindings persistent and
 * supports TAO_Bindings_Map interface.  Used by TAO_Persistent_Naming_Context.
 */
class TAO_Naming_Serv_Export TAO_Persistent_Bindings_Map
  : public TAO_Bindings_Map
{
public:

  /// Underlying data structure - typedef for ease of use.
  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId, TAO_Persistent_IntId> HASH_MAP;

  // = Initialization and termination methods.

  /// Constructor.
  TAO_Persistent_Bindings_Map (CORBA::ORB_ptr orb);

  /// Allocate hash map of size <hash_map_size> from persistent storage
  /// using the <alloc>.
  int open (size_t hash_map_size,
            ACE_Allocator *alloc);

  /// The hash map has already been preallocated for us.  We just need
  /// to set our data members take ownership of it.
  void set (HASH_MAP *map,
            ACE_Allocator *alloc);

  /// Destructor.  Does not deallocate the hash map: if an instance of
  /// this class goes out of scope, its hash_map remains in persistent storage.
  virtual ~TAO_Persistent_Bindings_Map (void);

  /**
   * This method removes the hash map from persistent storage/frees up
   * the memory.  The hash map better be empty, since we are not
   * cleaning up the insides. (We could add <close> to clean entries,
   * but not the data inside the entries.
   */
  void destroy (void);

  // = Accessor methods.

  /// Get a pointer to the underlying hash map.
  HASH_MAP *map (void);

  /// Return the size of the underlying hash table.
  size_t total_size (void);

  /// Return the size of the underlying hash table.
  virtual size_t current_size (void);

  // = Name bindings manipulation methods.

  /**
   * Add a binding with the specified parameters to the table.
   * Return 0 on success and -1 on failure, 1 if there already is a
   * binding with <id> and <kind>.
   */
  virtual int bind (const char *id,
                    const char *kind,
                    CORBA::Object_ptr obj,
                    CosNaming::BindingType type);

  /**
   * Overwrite a binding containing <id> and <kind> (or create a new
   * one if one doesn't exist) with the specified parameters.  Return
   * 0 or 1 on success.  Return -1 or -2 on failure. (-2 is returned
   * if the new and old bindings differ in type).
   */
  virtual int rebind (const char *id,
                      const char *kind,
                      CORBA::Object_ptr obj,
                      CosNaming::BindingType type);

  /**
   * Remove a binding containing <id> and <kind> from the table.
   * Return 0 on success and -1 on failure.
   */
  virtual int unbind (const char * id,
                      const char * kind);

  /**
   * Find the binding containing <id> and <kind> in the table, and
   * pass binding's type and object back to the caller by reference.
   * Return 0 on success and -1 on failure.  Note: a 'duplicated' object
   * reference is assigned to <obj>, so the caller is responsible for
   * its deallocation.
   */
  virtual int find (const char * id,
                    const char * kind,
                    CORBA::Object_ptr & obj,
                    CosNaming::BindingType &type);

protected:

  /**
   * Helper to the <open> method.  By isolating placement new into a
   * separate method, we can deal with memory allocation failures more
   * efficiently.  If there is a problem in HASH_MAP constructor, we
   * can clean up preallocated space.
   */
  int open_helper (size_t hash_table_size,
                   void *buffer);

  /// Helper: factors common code from <bind> and <rebind>.
  int shared_bind (const char *id,
                   const char *kind,
                   CORBA::Object_ptr obj,
                   CosNaming::BindingType type,
                   int rebind);

  /// Pointer to the allocator we use to make bindings persistent.
  ACE_Allocator *allocator_;

  /// Pointer to the underlying hash map.
  HASH_MAP *map_;

  /// Pointer to the orb.  We need it to do string/object conversions.
  CORBA::ORB_var orb_;
};

class TAO_Persistent_Context_Index;

/**
 * @class TAO_Persistent_Naming_Context
 *
 * @brief This class plays a role of a 'ConcreteImplementor' in the
 * Bridge pattern architecture of the CosNaming::NamingContext implementation.
 *
 * This class provides a persistent implementation of the
 * NamingContext functionality, i.e., the state is preserved across
 * process boundaries.  Derives from TAO_Hash_Naming_Context and
 * uses TAO_Persistent_Bindings_Map to store name to object bindings.
 */
class TAO_Naming_Serv_Export TAO_Persistent_Naming_Context : public TAO_Hash_Naming_Context
{
public:
  /// Underlying data structure - typedef for ease of use.
  typedef TAO_Persistent_Bindings_Map::HASH_MAP HASH_MAP;

  // = Initialization and termination methods.

  /// Allocate the underlying data structure from persistent storage.
  /// Returns 0 on success and -1 on failure.
  int init (size_t hash_table_size = ACE_DEFAULT_MAP_SIZE);

  /**
   * Constructor that takes in preallocated data structure and takes
   * ownership of it.  This constructor is for 'recreating' servants
   * from persistent state. If no map is provided, it MUST be followed
   * up by <init> to allocate the underlying data structure from
   * persistent storage!
   */
  TAO_Persistent_Naming_Context (PortableServer::POA_ptr poa,
                                 const char *poa_id,
                                 TAO_Persistent_Context_Index *context_index,
                                 HASH_MAP * map = 0,
                                 ACE_UINT32 *counter = 0);

  /// Destructor.
  virtual ~TAO_Persistent_Naming_Context (void);

  // = Utility methods.
  /**
   * This utility method factors out the code needed to create a new
   * Persistent Naming Context servant and activate it under the
   * specified POA with the specified id.  This function is static so
   * that the code can be used, both from inside the class (e.g., <new_context>),
   * and from outside (e.g., Naming_Utils.cpp).
   */
  static CosNaming::NamingContext_ptr make_new_context (PortableServer::POA_ptr poa,
                                                        const char *poa_id,
                                                        size_t context_size,
                                                        TAO_Persistent_Context_Index *ind);

  // = Methods not implemented in TAO_Hash_Naming_Context.

  /**
   * This operation returns a new naming context implemented by the
   * same naming server in which the operation was invoked.  The
   * context is not bound.
   */
  virtual CosNaming::NamingContext_ptr new_context (void);

  /**
   * Returns at most the requested number of bindings <how_many> in
   * <bl>.  If the naming context contains additional bindings, they
   * are returned with a BindingIterator.  In the naming context does
   * not contain any additional bindings <bi> returned as null.
   */
  virtual void list (CORBA::ULong how_many,
                     CosNaming::BindingList_out &bl,
                     CosNaming::BindingIterator_out &bi);

protected:

  /**
   * Set <destroyed_> flag (inherited from TAO_Hash_Naming_Context) to
   * <level>.  Legal values for <destroyed_> are 0, 1, and 2.  The
   * values specify the extent of cleanup that should take place in the
   * context's destructor:
   * '0' - no cleanup (e.g., if the context goes out of scope, but
   * it's state is to remain in persistent storage);
   * '1' - free up the underlying data structure in persistent storage
   * (e.g., if the initialization of this context was only partially completed
   * due to some failures, and we need to roll back);
   * '2' - free up the underlying data structure, and deregister this
   * naming context from its <index_> (e.g., if the context had
   * <destroy> method invoked and needs to be completely removed from existence).
   */
  void set_cleanup_level (int level);

  /// Counter used for generation of POA ids for children Naming
  /// Contexts.
  ACE_UINT32 *counter_;

  /**
   * A pointer to the underlying data structure used to store name
   * bindings. While our superclass (TAO_Hash_Naming_Context) also
   * maintains a pointer to the data structure, keeping this pointer
   * around saves us from the need to downcast when invoking
   * non-virtual methods.
   */
  TAO_Persistent_Bindings_Map *persistent_context_;

  /**
   * A pointer to the index object of this naming service: it keeps
   * track of all the naming contexts created.  Every time we make a
   * new context or destroy one, we need to make an entry there.
   * Also, we get the allocator needed to initialize us from this guy.
   */
  TAO_Persistent_Context_Index *index_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_PERSISTENT_NAMING_CONTEXT_H */
