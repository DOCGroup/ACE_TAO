/* -*- C++ -*- */
// $Id$
// ============================================================================
//
// = LIBRARY
//    cos
//
// = FILENAME
//   Shared_Hash_Map.h
//
// = AUTHOR
//    Marina Spivak <marina@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_SHARED_HASH_MAP_H
#define ACE_SHARED_HASH_MAP_H

#include "ace/Hash_Map_Manager.h"

template <class EXT_ID, class INT_ID>
class ACE_Shared_Hash_Map : public ACE_Hash_Map_Manager<EXT_ID, INT_ID, ACE_Null_Mutex>
{
  // = TITLE
  //     This class serves as a Proxy that ensures our process always
  //     has the appropriate allocator in place for every operation
  //     that accesses or updates the Map Manager.
  //
  // = DESCRIPTION
  //     We need this class because otherwise the ALLOCATOR
  //     pointer will be stored in the Map_Manager that resides within
  //     shared memory.  Naturally, this will cause horrible problems
  //     since only the first process to set that pointer will be
  //     guaranteed the address of the ALLOCATOR is meaningful!
public:
  ACE_Shared_Hash_Map (ACE_Allocator *alloc);
  // Constructor.

  ACE_Shared_Hash_Map (size_t size,
                       ACE_Allocator *alloc);
  // Constructor.

  // = The following methods are Proxies to the underlying methods
  // provided by <ACE_Hash_Map_Manager>.  When they are called, they
  // acquire the lock, set the allocator to the one specific to this
  // process, and then call down to perform the intended operation.
  int bind (const EXT_ID &,
            const INT_ID &,
            ACE_Allocator *alloc);

  int unbind (const EXT_ID &,
              INT_ID &,
              ACE_Allocator *alloc);

  int rebind (const EXT_ID &,
              const INT_ID &,
              EXT_ID &,
              INT_ID &,
              ACE_Allocator *alloc);

  int find (const EXT_ID &,
            INT_ID &,
            ACE_Allocator *alloc);

  int close (ACE_Allocator *alloc);
};
#endif /* ACE_SHARED_HASH_MAP_H */
