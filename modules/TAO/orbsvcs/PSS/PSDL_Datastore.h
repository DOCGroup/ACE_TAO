/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    PSDL_Datastore.h
 *
 *  $Id$
 *
 *  @author Priyanka Gontla <gontla_p@ociweb.com>
 */
//=============================================================================


#ifndef TAO_PSDL_DATASTORE_H
#define TAO_PSDL_DATASTORE_H
#include /**/ "ace/pre.h"

#include "PSDL_Code_Gen.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// The name under which the index of naming contexts is stored in
// persistent naming service.
#if !defined (TAO_PERSISTENT_NAME_OBJ_MAP)
#  define TAO_PERSISTENT_NAME_OBJ_MAP "Persistent_Name_Obj_Map"
#endif /* ! TAO_PERSISTENT_NAME_OBJ_MAP */

#include "ace/Malloc.h"
#include "ace/Hash_Map_With_Allocator_T.h"

/**
 * @class TAO_PSDL_Datastore
 *
 * @brief This class facilitates implementation of Persistent
 * Service. Helps perform the bind and find to the hash_map
 **/

// Forward declarations.
class TAO_PSDL_OctetSeq;
class TAO_PSDL_String;

class TAO_PSDL_Export TAO_PSDL_Datastore
{
public:
  // = Some typedefs for convenience.

  /// Hash map used by Persistent Naming Context to keep its state.
  typedef ACE_Hash_Map_With_Allocator<TAO_PSDL_String, TAO_PSDL_OctetSeq> NAME_OBJ_REF_MAP;

  typedef ACE_Hash_Map_Entry <TAO_PSDL_String, TAO_PSDL_OctetSeq> NAME_OBJ_REF_ENTRY;

  /// Allocator we will be using to make the Naming Service persistent.
  typedef ACE_Allocator_Adapter <ACE_Malloc <ACE_MMAP_MEMORY_POOL, TAO_SYNCH_MUTEX>
  > ALLOCATOR;

  // = Initialization and termination methods.

  /// Constructor.
  TAO_PSDL_Datastore ();

  /**
   * Create ACE_Allocator, open/create memory-mapped file with the
   * specified file name/base address.  Find or allocate <index_>.
   * Return 0 on success or -1 on failure.
   */
  int open (const ACE_TCHAR *file_name,
            void * base_address = ACE_DEFAULT_BASE_ADDR);

  /**
   * If <index_> contains no entries (i.e., was just created), create
   * a root Persistent servant with table of size
   * <context_size>, and make an entry for it
   * in the <index_>.  If <index_> contains entries, create a
   * Persistent servant for each entry.  Return 0 on
   * success and -1 on failure.
   */
  int init (size_t context_size);

  /**
   * Destructor.  The memory mapped file that was opened/created is
   * not deleted, since we want it to keep the state of the Database
   * until the next run.
   */
  ~TAO_PSDL_Datastore (void);

  // = Accessors.
  int bind (const char *name,
            const CORBA::OctetSeq &octetseq);

  int unbind (const char *name);

  int find (const char *name,
            CORBA::OctetSeq &octet_seq);

  /// Return allocator.
  ACE_Allocator *allocator (void);

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
   * use it whenever we
   * deal with data that should be kept in persistent store.
   */
  ALLOCATOR *allocator_;

  /// Hash map where we keep entries for all Persistent entries
  NAME_OBJ_REF_MAP *obj_ref_map_;

  /// Name of the memory-mapped file used by <allocator_>.
  const ACE_TCHAR *index_file_;

  /// Base address for the memory-mapped file.
  void *base_address_;
};

#include /**/ "ace/post.h"
#endif /* TAO_PSDL_DATASTORE_H */
