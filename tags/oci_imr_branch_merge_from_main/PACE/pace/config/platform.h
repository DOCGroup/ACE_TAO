/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/config/platform.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_CONFIG_PLATFORM_H
#define PACE_CONFIG_PLATFORM_H

/* Be careful not to define these macros 2x. */
/* IE: makefile or other config file. */
/* This could create compiler warnings. */

/* These platforms are POSIX compliant. */

#if defined (PACE_LINUX)
# define PACE_HAS_POSIX PACE_LINUX
# define PACE_LYNXOS 0
# define PACE_SUNOS 0
# define PACE_VXWORKS 0
# define PACE_WIN32 0
#elif defined (PACE_LYNXOS)
# define PACE_HAS_POSIX PACE_LYNXOS
# define PACE_LINUX 0
# define PACE_SUNOS 0
# define PACE_VXWORKS 0
# define PACE_WIN32 0
#elif defined (PACE_OSF1)
# define PACE_HAS_POSIX DIGITAL_UNIX
# define PACE_LINUX 0
# define PACE_LYNXOS 0
# define PACE_VXWORKS 0
# define PACE_WIN32 0
#elif defined (PACE_SUNOS)
# define PACE_HAS_POSIX PACE_SUNOS
# define PACE_LINUX 0
# define PACE_LYNXOS 0
# define PACE_VXWORKS 0
# define PACE_WIN32 0
#elif defined (PACE_QNX)
# define PACE_HAS_POSIX PACE_QNX
# define PACE_LINUX 0
# define PACE_SUNOS 0
# define PACE_LYNXOS 0
# define PACE_VXWORKS 0
# define PACE_WIN32 0

/* These platforms are NOT POSIX compliant. */

/* This is really kludgy but not sure what else to do at this point.
   It appears that if PACE_HAS_POSIX is not defined (i.e., no #define
   at all of any kind) and the same goes for PACE_VXWORKS then in
   pace/config/config.h #if PACE_HAS_POSIX == PACE_LYNXOS will evaluate
   to true. I am assuming both get default values of 0.

   PACE_WIN32 should be defined as 0 though because pace/config/utility.h
   checks it.
   */

#elif defined (PACE_VXWORKS)
# define PACE_LACKS_POSIX PACE_VXWORKS
# define PACE_LINUX -1
# define PACE_LYNXOS -1
# define PACE_SUNOS -1
# define PACE_WIN32 0
/* Doesn't use the makefile! */
#elif defined (WIN32)
# undef PACE_WIN32
# define PACE_WIN32 2000
# define PACE_LACKS_POSIX PACE_WIN32
# define PACE_LINUX 0
# define PACE_LYNXOS 0
# define PACE_SUNOS 0
# define PACE_VXWORKS 0
#endif

#endif /* PACE_CONFIG_PLATFORM_H */
