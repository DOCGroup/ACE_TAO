// $Id$

// ========================================================================

// FILENAME
//   ACE_Flag_Manip.h
//
// DESCRIPTION
//   This class includes the functions used for the Flag Manipulation.
//
// AUTHOR
//   Priyanka Gontla <pgontla@ece.uci.edu>

// =======================================================================

#ifndef ACE_FLAG_MANIP_H
#define ACE_FLAG_MANIP_H
#include "ace/pre.h"

#include "OS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Export ACE_Flag_Manip
{
 public:

  // = Set/get/clear various flags related to I/O HANDLE.
  static int set_flags (ACE_HANDLE handle,
                        int flags);
  // Set flags associated with <handle>.

  static int clr_flags (ACE_HANDLE handle,
                        int flags);
  // Clear flags associated with <handle>.

  static int get_flags (ACE_HANDLE handle);
  // Return the current setting of flags associated with <handle>.


};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Flag_Manip.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"
#endif  /* ACE_FLAG_MANIP_H */
