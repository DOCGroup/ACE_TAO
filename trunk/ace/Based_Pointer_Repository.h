/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Based_Pointer_Repository.h
//
// = AUTHOR
//    Dietrich Quehl <Dietrich.Quehl@med.siemens.de> and
//    Douglas C. Schmidt <schmidt@.cs.wustl.edu>
//
// ============================================================================

#if !defined (ACE_BASED_POINTER_REPOSITORY_H)
#define ACE_BASED_POINTER_REPOSITORY_H

#include "ace/Singleton.h"
#include "ace/Map_Manager.h"

class ACE_Export ACE_Based_Pointer_Repository 
{
  // = TITLE
  //   Maps pointers to the base address of the region to which each
  //   pointer belongs.
  // 
  // = DESCRIPTION
  //   Every memory pool in ACE binds it's mapping base address and
  //   the mapped size to this repository every time it maps/remaps a
  //   new chunk of memory successfully.
public:
  // = Use <ACE_Null_Mutex> to allow locking while iterating.
  typedef ACE_Map_Manager <void *, size_t *, ACE_Null_Mutex> 
          MAP_MANAGER;
  typedef ACE_Map_Iterator < void *, size_t *, ACE_Null_Mutex> 
          MAP_ITERATOR;
  typedef ACE_Map_Entry <void *, size_t *> 
          MAP_ENTRY; 

  // = Initialization and termination methods.
  ACE_Based_Pointer_Repository (void);
  ~ACE_Based_Pointer_Repository (void);

  // = Search structure methods.
  int find (void *addr, 
            void *&base_addr);
  // Return the appropriate <base_addr> region that contains <addr>.
  // Returns 1 on success and 0 if the <addr> isn't contained in any
  // <base_addr> region.

  int bind (void *addr,
            size_t size);
  // Bind a new entry to the repository or update the size of an
  // existing entry.  Returns 0 on success and -1 on failure.

  int unbind (void *addr);
  // Unbind from the repository the <base_addr> that <addr> is
  // contained within.

private:
  MAP_MANAGER addr_map_;
  // Keeps track of the mapping between addresses and their associated
  // values.

  ACE_SYNCH_MUTEX lock_;
  // Synchronize concurrent access to the map.
};

// Provide a Singleton access point to the based pointer repository.
typedef ACE_Singleton<ACE_Based_Pointer_Repository, ACE_SYNCH_RW_MUTEX>
        ACE_BASED_POINTER_REPOSITORY;

#endif /* ACE_BASED_POINTER_REPOSITORY_H */
