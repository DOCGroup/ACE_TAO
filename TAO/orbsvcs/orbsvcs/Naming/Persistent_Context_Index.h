/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//    Persistent_Context_Index.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PERSISTENT_CONTEXT_INDEX_H
#define TAO_PERSISTENT_CONTEXT_INDEX_H

#include "Persistent_Entries.h"

class TAO_ORBSVCS_Export TAO_Persistent_Context_Index
{
  // = TITLE
  //
  //
  // = DESCRIPTION
  //
public:

  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_Index_ExtId,
    TAO_Persistent_Index_IntId> INDEX;
  typedef ACE_Hash_Map_With_Allocator<TAO_Persistent_ExtId,
    TAO_Persistent_IntId> CONTEXT;

  // = Initialization and termination methods.
  TAO_Persistent_Context_Index (CORBA::ORB_ptr orb,
                                PortableServer::POA_ptr poa);
  // "Do-nothing" constructor.

  int open (LPCTSTR file_name,
            void * base_address = ACE_DEFAULT_BASE_ADDR);
  //

  int init (void);
  // go through the index of contexts and create a servant of each,
  // and register it with POA.  If no contexts are present, create one.

  ~TAO_Persistent_Context_Index (void);
  // destructor, do some cleanup :TBD: last dtor should "compress"
  // file

  int bind (const char *poa_id,
                    ACE_UINT32 *&counter,
                    CONTEXT *hash_map);
  // Create an entry - new context is created.

  int unbind (const char * poa_id);
  // Unbind an entry - a context is destroyed.

  ACE_Allocator *allocator (void);
  // accessor.

  CORBA::ORB_ptr orb (void);
  //

  char * root_ior (void);
  //

  //@@ ACE_Null_Mutex might be sufficient here.
  typedef ACE_Allocator_Adapter <ACE_Malloc <ACE_MMAP_MEMORY_POOL, ACE_SYNCH_MUTEX>
  > ALLOCATOR;

private:

  int recreate_all (void);
  //

  int create_index (void);
  // Allocate the appropriate type of map manager that stores the
  // key/value binding.

  ACE_SYNCH_MUTEX lock_;
  // Lock to prevent multiple threads from modifying entries in the
  // hash map simultanneously.

  ALLOCATOR *allocator_;
  // Pointer to the allocator

  INDEX *index_;
  // Pointer to the allocated map manager.

  LPCTSTR index_file_;
  // Name of the file used for storage.

  void *base_address_;

  CORBA::ORB_var orb_;
  //

  PortableServer::POA_var poa_;
  //

  CORBA::String_var root_ior_;
  //
};

#endif /* TAO_PERSISTENT_CONTEXT_INDEX_H */
