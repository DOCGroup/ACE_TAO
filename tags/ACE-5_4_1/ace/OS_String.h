// -*- C++ -*-
//=============================================================================
/**
 *  @file   OS_String.h
 *
 *  $Id$
 *
 *  @brief  Contains definition of class ACE_OS_String.
 *
 *  @author Doug Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller <stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//=============================================================================

#ifndef ACE_OS_STRING_H
#define ACE_OS_STRING_H
#include /**/ "ace/pre.h"

#include "ace/ACE_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

//#include "ace/Basic_Types.h"
//#include "ace/os_include/os_stddef.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"
#include "ace/OS_NS_wchar.h"
#include "ace/OS_NS_ctype.h"

#if 0
#if defined (ACE_USES_STD_NAMESPACE_FOR_STDC_LIB) && \
            (ACE_USES_STD_NAMESPACE_FOR_STDC_LIB != 0)
using std::size_t;
using std::wint_t;
#endif /* ACE_USES_STD_NAMESPACE_FOR_STDC_LIB */
#endif

# if defined (ACE_HAS_INLINED_OSCALLS)
#   if defined (ACE_INLINE)
#     undef ACE_INLINE
#   endif /* ACE_INLINE */
#   define ACE_INLINE inline
#   include "ace/OS_String.inl"
# endif /* ACE_HAS_INLINED_OSCALLS */

#include /**/ "ace/post.h"
#endif /* ACE_OS_STRING_H */
