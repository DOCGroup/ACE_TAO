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

#ifndef ACE_BASED_POINTER_REPOSITORY_H
#define ACE_BASED_POINTER_REPOSITORY_H

#include "ace/OS.h"                  // Need ACE_Export

// Forward decl., using the "Cheshire Cat" technique.
class ACE_Based_Pointer_Repository_Rep;

class ACE_Export ACE_Based_Pointer_Repository
{
  // = TITLE
  //   Maps pointers to the base address of the region to which each
  //   pointer belongs.
public:
  // = Use <ACE_Null_Mutex> to allow locking while iterating.

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
  ACE_Based_Pointer_Repository_Rep *rep_;
  // Use the "Cheshire-Cat" technique to hide the implementation in
  // order to avoid circular #include dependencies.
};

#include "ace/Singleton.h"

// Provide a Singleton access point to the based pointer repository.
typedef ACE_Singleton<ACE_Based_Pointer_Repository, ACE_SYNCH_RW_MUTEX>
        ACE_BASED_POINTER_REPOSITORY;

#endif /* ACE_BASED_POINTER_REPOSITORY_H */
