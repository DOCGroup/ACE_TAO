/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE
//
// = FILENAME
//    Persistent_Context_Index.h
//
// = AUTHOR
//    Prashant Jain (pjain@cs.wustl.edu)
//
// ============================================================================

#ifndef TAO_PERSISTENT_CONTEXT_INDEX_H
#define TAO_PERSISTENT_CONTEXT_INDEX_H


#define TAO_NAME_CONTEXTS_INDEX "Naming_Service_Index"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Persistent_Entries.h"

class TAO_Index
{
  // = TITLE
  //   Base class.  For now it is here to avoid templatizing
  //   TAO_Persistent_Naming_Context.

public:

typedef ACE_Shared_Hash_Map<TAO_Persistent_Index_ExtId,
  TAO_Persistent_Index_IntId> INDEX;
typedef ACE_Shared_Hash_Map<TAO_Persistent_ExtId,
  TAO_Persistent_IntId> CONTEXT;


  virtual int bind (const char *poa_id,
                    ACE_UINT32* & counter,
                    CONTEXT *hash_map) = 0;
  // Create an entry - new context is created.

  virtual int unbind (const char * poa_id) = 0;
  // Unbind an entry - a context is destroyed.

  virtual ACE_Allocator *allocator (void) = 0;
  // accessor.

  virtual CORBA::ORB_ptr orb (void) = 0;
  //
};

template <ACE_MEM_POOL_1, class ACE_LOCK>
class TAO_Persistent_Context_Index : public TAO_Index
{
  // = TITLE
  //
  //
  // = DESCRIPTION
  //   ACE_LOCK should either be ACE_Null_Mutex or ACE_Thread_Mutex.
  //   ACE_MEM_POOL_1 for now is ACE_MMAP_Memory_Pool, later may make
  //   sense with other pools.
public:
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

  virtual int bind (const char *poa_id,
                    ACE_UINT32 *&counter,
                    CONTEXT *hash_map);
  // Create an entry - new context is created.

  virtual int unbind (const char * poa_id);
  // Unbind an entry - a context is destroyed.

  virtual ACE_Allocator *allocator (void);
  // accessor.

  virtual CORBA::ORB_ptr orb (void);
  //

  char * root_ior (void);
  //

  typedef ACE_Allocator_Adapter <ACE_Malloc <ACE_MEM_POOL_2, ACE_LOCK>
  > ALLOCATOR;

private:

  int recreate_all (void);
  //

  int create_index (void);
  // Allocate the appropriate type of map manager that stores the
  // key/value binding.

  ACE_LOCK *lock_;
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
