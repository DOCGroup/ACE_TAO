// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_errno.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 *
 *  Originally in OS.h.
 */
//=============================================================================

#ifndef ACE_OS_NS_ERRNO_H
# define ACE_OS_NS_ERRNO_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_errno.h"
#include "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

namespace ACE_OS {

  ACE_NAMESPACE_INLINE_FUNCTION
  int last_error (void);

  ACE_NAMESPACE_INLINE_FUNCTION
  void last_error (int);

  ACE_NAMESPACE_INLINE_FUNCTION
  int set_errno_to_last_error (void);

  ACE_NAMESPACE_INLINE_FUNCTION
  int set_errno_to_wsa_last_error (void);

} /* namespace ACE_OS */

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_errno.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_ERRNO_H */
