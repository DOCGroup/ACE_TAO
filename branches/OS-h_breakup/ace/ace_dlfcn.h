/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_dlfcn.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_DLFCN_H
# define ACE_ACE_DLFCN_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

/* Set the proper handle type for dynamically-loaded libraries. */
/* Also define a default 'mode' for loading a library - the names and values */
/* differ between OSes, so if you write code that uses the mode, be careful */
/* of the platform differences. */
# if defined (ACE_HAS_SVR4_DYNAMIC_LINKING)

#   if defined (ACE_HAS_DLFCN_H_BROKEN_EXTERN_C)
extern "C" {
#   endif /* ACE_HAS_DLFCN_H_BROKEN_EXTERN_C */
#   include /**/ <dlfcn.h>
#   if defined (ACE_HAS_DLFCN_H_BROKEN_EXTERN_C)
}
#   endif /* ACE_HAS_DLFCN_H_BROKEN_EXTERN_C */
    typedef void *ACE_SHLIB_HANDLE;
#   define ACE_SHLIB_INVALID_HANDLE 0
#   if defined (__KCC) && defined(RTLD_GROUP) && defined(RTLD_NODELETE)
#     define ACE_DEFAULT_SHLIB_MODE RTLD_LAZY | RTLD_GROUP | RTLD_NODELETE
#   else
#     define ACE_DEFAULT_SHLIB_MODE RTLD_LAZY
#   endif /* KCC */

# elif defined (__hpux)
#   if defined(__GNUC__) || __cplusplus >= 199707L
#     include /**/ <dl.h>
#   else
#     include /**/ <cxxdl.h>
#   endif /* (g++ || HP aC++) vs. HP C++ */
    typedef shl_t ACE_SHLIB_HANDLE;
#   define ACE_SHLIB_INVALID_HANDLE 0
#   define ACE_DEFAULT_SHLIB_MODE BIND_DEFERRED

# elif defined (ACE_WIN32)
    // Dynamic loading-related types - used for dlopen and family.
    typedef HINSTANCE ACE_SHLIB_HANDLE;
#   define ACE_SHLIB_INVALID_HANDLE 0
#   define ACE_DEFAULT_SHLIB_MODE 0

# else
    typedef void *ACE_SHLIB_HANDLE;
#   define ACE_SHLIB_INVALID_HANDLE 0
#   define ACE_DEFAULT_SHLIB_MODE RTLD_LAZY

# endif /* ACE_HAS_SVR4_DYNAMIC_LINKING */


# if !defined (RTLD_LAZY)
#   define RTLD_LAZY 1
# endif /* !RTLD_LAZY */

# if !defined (RTLD_NOW)
#   define RTLD_NOW 2
# endif /* !RTLD_NOW */

# if !defined (RTLD_GLOBAL)
#   define RTLD_GLOBAL 3
# endif /* !RTLD_GLOBAL */

#endif /* ACE_ACE_DLFCN_H */
