// -*- C++ -*-

//=============================================================================
/**
 *  @file   Flag_Manip.h
 *
 *  $Id$
 *
 *  This class includes the functions used for the Flag Manipulation.
 *
 *  @author Priyanka Gontla <pgontla@doc.ece.uci.edu>
 */
//=============================================================================

#ifndef ACE_FLAG_MANIP_H
#define ACE_FLAG_MANIP_H

#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/Global_Macros.h"
#include "ace/os_include/os_fcntl.h"   /* For values passed to these methods */

class ACE_Export ACE_Flag_Manip
{
 public:

  // = Set/get/clear various flags related to I/O HANDLE.
  /// Set flags associated with <handle>.
  static int set_flags (ACE_HANDLE handle,
                        int flags);

  /// Clear flags associated with <handle>.
  static int clr_flags (ACE_HANDLE handle,
                        int flags);

  /// Return the current setting of flags associated with <handle>.
  static int get_flags (ACE_HANDLE handle);
};

#if defined (__ACE_INLINE__)
#include "ace/Flag_Manip.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"
#endif  /* ACE_FLAG_MANIP_H */
