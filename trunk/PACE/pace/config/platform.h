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

#elif defined (PACE_VXWORKS)
# define PACE_LACKS_POSIX PACE_VXWORKS
# define PACE_LINUX 0
# define PACE_LYNXOS 0
# define PACE_SUNOS 0
# define PACE_WIN32 0
/* Doesn't use the makefile! */
#elif defined (WIN32)
# define PACE_WIN32 2000
# define PACE_LACKS_POSIX PACE_WIN32
# define PACE_LINUX 0
# define PACE_LYNXOS 0
# define PACE_SUNOS 0
# define PACE_VXWORKS 0
#endif

#endif /* PACE_CONFIG_PLATFORM_H */
