// -*- C++ -*-

//==========================================================================
/**
 *  @file   config-all.h
 *
 *  $Id$
 *
 *  @author (Originally in OS.h)Doug Schmidt <schmidt@cs.wustl.edu>
 *  @author Jesper S. M|ller<stophph@diku.dk>
 *  @author and a cast of thousands...
 */
//==========================================================================

#ifndef ACE_CONFIG_ALL_H
#define ACE_CONFIG_ALL_H

#include /**/ "ace/pre.h"

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

// ============================================================================
// UNICODE macros (to be added later)
// ============================================================================

// Get the unicode (i.e. ACE_TCHAR) defines
# include "ace/ace_wchar.h"

# if defined (VXWORKS)
#   if defined (ghs)
    // GreenHills 1.8.8 needs the stdarg.h #include before the #include of
    // vxWorks.h.
    // Also, be sure that these #includes come _after_ the key_t typedef, and
    // before the #include of time.h.
#     include "ace/os_include/os_stdarg.h"
#   endif /* ghs */

#   include /**/ <vxWorks.h>
# endif /* VXWORKS */

// This is used to indicate that a platform doesn't support a
// particular feature.
#if defined ACE_HAS_VERBOSE_NOTSUP
  // Print a console message with the file and line number of the
  // unsupported function.
# if defined (ACE_HAS_STANDARD_CPP_LIBRARY) && (ACE_HAS_STANDARD_CPP_LIBRARY != 0)
#   include /**/ <cstdio>
# else
#   include "ace/os_include/os_stdio.h"
# endif
# define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP; fprintf (stderr, ACE_LIB_TEXT ("ACE_NOTSUP: %s, line %d\n"), __FILE__, __LINE__); return FAILVALUE; } while (0)
# define ACE_NOTSUP do { errno = ENOTSUP; fprintf (stderr, ACE_LIB_TEXT ("ACE_NOTSUP: %s, line %d\n"), __FILE__, __LINE__); return; } while (0)
#else /* ! ACE_HAS_VERBOSE_NOTSUP */
# define ACE_NOTSUP_RETURN(FAILVALUE) do { errno = ENOTSUP ; return FAILVALUE; } while (0)
# define ACE_NOTSUP do { errno = ENOTSUP; return; } while (0)
#endif /* ! ACE_HAS_VERBOSE_NOTSUP */

// These includes are here to avoid circular dependencies.
// Keep this at the bottom of the file.  It contains the main macros.
#include "ace/OS_main.h"

#include /**/ "ace/post.h"

#endif /* ACE_CONFIG_ALL_H */
