// -*- C++ -*-

//=============================================================================
/**
 *  @file   Handle_Ops.h
 *
 *  $Id$
 *
 * This class consolidates the operations on the Handles.
 *
 *
 *  @author Priyanka Gontla <pgontla@ece.uci.edu>
 */
//=============================================================================

#ifndef ACE_HANDLE_OPS_H
#define ACE_HANDLE_OPS_H

#include "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"

class ACE_Time_Value;

class ACE_Export ACE_Handle_Ops
{
public:
  // = Operations on HANDLEs.

  /**
   * Wait up to <timeout> amount of time to actively open a device.
   * This method doesn't perform the <connect>, it just does the timed
   * wait...
   */
  static ACE_HANDLE handle_timed_open (ACE_Time_Value *timeout,
                                       const ACE_TCHAR *name,
                                       int flags,
                                       int perms,
                                       LPSECURITY_ATTRIBUTES sa = 0);
};

#if !defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/Handle_Ops.i"
#endif /* ACE_LACKS_INLINE_FUNCTIONS */

#include "ace/post.h"

#endif /* ACE_HANDLE_OPS_H */
