/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//    Persistent_Context_Index_T.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef TAO_PERSISTENT_CONTEXT_INDEX_T_H
#define TAO_PERSISTENT_CONTEXT_INDEX_T_H

#include "Persistent_Context_Index.h"

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

#if defined (ACE_TEMPLATES_REQUIRE_SOURCE)
#include "Persistent_Context_Index_T.cpp"
#endif /* ACE_TEMPLATES_REQUIRE_SOURCE */

#if defined (ACE_TEMPLATES_REQUIRE_PRAGMA)
#pragma implementation ("Persistent_Context_Index_T.cpp")
#endif /* ACE_TEMPLATES_REQUIRE_PRAGMA */

#endif /* TAO_PERSISTENT_CONTEXT_INDEX_T_H */
