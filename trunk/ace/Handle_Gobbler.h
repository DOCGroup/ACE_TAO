// $Id$

// ============================================================================
//
// = LIBRARY
//    ace
//
// = FILENAME
//    Handle_Gobbler.h
//
// = AUTHOR
//    Kirthika Parameswaran <kirthika@cs.wustl.edu>
//    Irfan Pyarali <irfan@cs.wustl.edu>
//
// ============================================================================

#ifndef ACE_HANDLE_GOBBLER_H
#include "ace/pre.h"
#define ACE_HANDLE_GOBBLER_H

#include "ace/OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Containers_T.h"

class ACE_Handle_Gobbler
{
  // = TITLE
  //     This class gobbles up handles.
  //
  // = DESCRIPTION
  //     This is useful when we need to control the number of handles
  //     available for a process.  This class is mostly used for
  //     testing purposes.
public:

  inline ~ACE_Handle_Gobbler (void);
  // Destructor.  Cleans up any remaining handles.

  inline int consume_handles (size_t n_handles_to_keep_available);
  // Handles are opened continously until the process runs out of
  // them, and then <n_handles_to_keep_available> handles are closed
  // (freed) thereby making them usable in the future.

  inline int free_handles (size_t n_handles);
  // Free up <n_handles>.

  inline void close_remaining_handles (void);
  // All remaining handles are closed.

private:

  typedef ACE_Unbounded_Set<ACE_HANDLE> HANDLE_SET;

  HANDLE_SET handle_set_;
  // The container which holds the open descriptors.
};

#include "ace/Handle_Gobbler.i"

#include "ace/post.h"
#endif /* ACE_HANDLE_GOBBLER_H */
