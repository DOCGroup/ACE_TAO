// $Id$

// ========================================================================
// LIBRARY
//   ACE.so
//
// DESCRIPTION
//   This class consolidates the operations on the Handles.
//
// AUTHOR
//   Priyanka Gontla <pgontla@ece.uci.edu>
//
// =========================================================================

#ifndef ACE_ACE_HANDLE_OPS_H
#define ACE_ACE_HANDLE_OPS_H
#include "ace/pre.h"

#include "ace/OS.h"

class ACE_Export ACE_Handle_Ops
{
 public:
    // = Operations on HANDLEs.

  static ACE_HANDLE handle_timed_open (ACE_Time_Value *timeout,
                                       const ACE_TCHAR *name,
                                       int flags,
                                       int perms);
  // Wait up to <timeout> amount of time to actively open a device.
  // This method doesn't perform the <connect>, it just does the timed
  // wait...
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/ACE_Handle_Ops.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif /* ACE_ACE_HANDLE_OPS_H */
