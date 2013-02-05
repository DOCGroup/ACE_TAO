// -*- C++ -*-

//=============================================================================
/**
 *  @file    Persistent_Context_Index.h
 *
 *  $Id$
 *
 *  @author Marina Spivak <marina@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_PERSISTENT_CONTEXT_INDEX_H
#define TAO_PERSISTENT_CONTEXT_INDEX_H
#include /**/ "ace/pre.h"

#include "orbsvcs/Naming/Persistent_Entries.h"
#include "orbsvcs/Naming/naming_serv_export.h"
#include "orbsvcs/Naming/nsconf.h"
#include "tao/PortableServer/PortableServer.h"

#include "ace/Malloc_T.h"
#include "ace/MMAP_Memory_Pool.h"

class TAO_Persistent_Naming_Context;
class TAO_Persistent_Naming_Context_Factory;

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_Persistent_Context_Index
 *
 * @brief This class facilitates implementation of Persistent
 * Naming Service.  It keeps track, centrally, of several pieces of
 * info for each Persistent Naming Context, allowing to perform the
 * initialization necessary for each Naming Context to
 * restore the state of the Naming Service from persistent storage
 * on server start-up.
 *
 * This class creates a memory-mapped file, allocates a hash
 * table from that file, and uses the hash table to store POA id,
 * and table and counter pointers for each Persistent Naming
 * Context.  There are methods for adding and deleting entries
 * from this hash table as new Persistent Naming Contexts are
 * created and old ones are destroyed.  This hash table
 * facilitates Persistent Naming Context servant initialization
 * upon Naming Server start-up.
 */
class TAO_Naming_Serv_Export TAO_Persistent_Context_Index
{
public:
  // = Some typedefs for convenience.

  /// Hash map in which we will store info about each Persistent Naming Context.
  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_Index_ExtId,
    TAO_Persistent_Index_IntId> CONTEXT_INDEX;

  /// Hash map used by Persistent Naming Context to keep its state.
  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId,
    TAO_Persistent_IntId> CONTEXT;

  /// Allocator we will be using to make the Naming Service persistent.
  typedef ACE_Allocator_Adapter <ACE_Malloc <ACE_MMAP_MEMORY_POOL, TAO_SYNCH_MUTEX>
  > ALLOCATOR;

  // = Initialization and termination methods.

  /// Constructor.
  TAO_Persistent_Context_Index (
    CORBA::ORB_ptr orb,
    PortableServer::POA_ptr poa,
    TAO_Persistent_Naming_Context_Factory *context_impl_factory);

  /**
   * Create ACE_Allocator, open/create memory-mapped file with the
   * specified file name/base address.  Find or allocate <index_>.
   * Return 0 on success or -1 on failure.
   */
  int open (const ACE_TCHAR *file_name,
            void * base_address = TAO_NAMING_BASE_ADDR);

  /**
   * If <index_> contains no entries (i.e., was just created), create
   * a root Persistent Naming Context servant with table of size
   * <context_size>, and make an entry for it
   * in the <index_>.  If <index_> contains entries, create a
   * Persistent Naming Context servant for each entry.  Return 0 on
   * success and -1 on failure.
   */
  int init (size_t context_size);

  /**
   * Create a naming context implementation to be used for this index
   * using the naming context factory that was provided in the ctor
   * for the index.
   */
  TAO_Persistent_Naming_Context *create_naming_context_impl (
    PortableServer::POA_ptr poa,
    const char *poa_id);

  /**
   * Destructor.  The memory mapped file that was opened/created is
   * not deleted, since we want it to keep the state of the Naming
   * Service until the next run.
   */
  ~TAO_Persistent_Context_Index (void);

  // = Methods for adding/removing entries.

  /**
   * Create an entry for a Persistent Naming Context in <index_>,
   * i.e., a context with <poa_id>, <counter> and <hash_map> has just
   * been created, and is registering with us.
   */
  int bind (const char *poa_id, ACE_UINT32 *&counter, CONTEXT *hash_map);

  /// Remove an entry for the Persistent Naming Context with <poa_id>
  /// from <index_> (i.e., this context has just been destroyed).
  int unbind (const char *poa_id);

  // = Accessors.

  /// Return allocator.
  ACE_Allocator *allocator (void);

  /// Return orb pointer.
  CORBA::ORB_ptr orb (void);

  /// Return a pointer to the root Naming Context (returns a copy - must be
  /// deallocated by the user).
  CosNaming::NamingContext_ptr root_context (void);

private:

  /// Helper for the <init> method.  Iterates over <index_>, and
  /// creates a servant for each entry.
  int recreate_all (void);

  /// Helper for the <open> method.
  int create_index (void);

  /// Helper for <create_index> method: places hash table into an
  /// allocated space.
  int create_index_helper (void *buffer);

  /// Lock to prevent multiple threads from modifying entries in the
  /// <index_> simultanneously.
  TAO_SYNCH_MUTEX lock_;

  /**
   * Allocator that deals out memory from a memory-mapped file.  We
   * use it here, and in TAO_Persistent_Naming_Context, whenever we
   * deal with data that should be kept in persistent store.
   */
  ALLOCATOR *allocator_;

  /// Hash map where we keep entries for all Persistent Naming
  /// Contexts.
  CONTEXT_INDEX *index_;

  /// Name of the memory-mapped file used by <allocator_>.
  const ACE_TCHAR *index_file_;

  /// Base address for the memory-mapped file.
  void *base_address_;

  /// ORB.  We use it for several object_to_string conversions, and
  /// keep it around for Persistent Naming Contexts' use.
  CORBA::ORB_var orb_;

  /// POA under which to register Persistent Naming Context servants
  /// during start-up.
  PortableServer::POA_var poa_;

  /// The reference to the root Naming Context.
  CosNaming::NamingContext_var root_context_;

  /// The factory for constructing naming contexts within the index
  TAO_Persistent_Naming_Context_Factory *context_impl_factory_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_PERSISTENT_CONTEXT_INDEX_H */
