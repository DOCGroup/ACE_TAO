// -*- C++ -*-

//=============================================================================
/**
 *  @file   OS_NS_math.h
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

#ifndef ACE_OS_NS_MATH_H
# define ACE_OS_NS_MATH_H

# include /**/ "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ace/os_include/os_math.h"

#include /**/ "ace/ACE_export.h"

#if defined (ACE_EXPORT_MACRO)
#  undef ACE_EXPORT_MACRO
#endif
#define ACE_EXPORT_MACRO ACE_Export

/*
 * We inline and undef some functions that may be implemented
 * as macros on some platforms. This way macro definitions will
 * be usable later as there is no way to save the macro definition
 * using the pre-processor.
 *
 */

inline double ace_log2_helper (double x)
{
#if defined (log2)
  return log2 (x);
#undef log2
#else
#  if !defined (ACE_LACKS_LOG2)
  return ACE_STD_NAMESPACE::log2 (x);
#  else
  ACE_UNUSED_ARG (x);
  unsigned long const ACE_NaN[2]= { 0xffffffff, 0x7fffffff };
  return *reinterpret_cast<double const *> (ACE_NaN);
#  endif /* !ACE_LACKS_LOG2 */
#endif /* defined (log2) */
}

ACE_BEGIN_VERSIONED_NAMESPACE_DECL

namespace ACE_OS
{
  /// This method computes the largest integral value not greater than x.
  ACE_NAMESPACE_INLINE_FUNCTION
  double floor (double x);

  /// This method computes the smallest integral value not less than x.
  ACE_NAMESPACE_INLINE_FUNCTION
  double ceil (double x);

#if !defined (ACE_LACKS_LOG2)
  /// This method computes base-2 logarithm of x
  ACE_NAMESPACE_INLINE_FUNCTION
  double log2 (double x);
#endif /* !ACE_LACKS_LOG2 */

} /* namespace ACE_OS */

ACE_END_VERSIONED_NAMESPACE_DECL

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_NS_math.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

# include /**/ "ace/post.h"
#endif /* ACE_OS_NS_MATH_H */
