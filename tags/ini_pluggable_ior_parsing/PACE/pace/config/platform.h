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

/* These platforms are POSIX compliant. */
#if defined (linux)
# define PACE_LINUX linux
# define PACE_HAS_POSIX PACE_LINUX
#elif defined (__Lynx__)
# define PACE_LYNX __Lynx__
# define PACE_HAS_POSIX PACE_LYNX
#elif defined (__osf__)
# define PACE_OSF __osf__
# define PACE_HAS_POSIX PACE_OSF
#elif defined (sunos)
# define PACE_SUNOS sunos
# define PACE_HAS_POSIX PACE_SUNOS

/* These platforms are NOT POSIX compliant. */
#elif defined (VXWORKS)
# define PACE_VXWORKS VXWORKS
# define PACE_LACKS_POSIX PACE_VXWORKS
#elif defined (WIN32)
# define PACE_WIN32 WIN32
# define PACE_LACKS_POSIX PACE_WIN32
#endif

#endif /* PACE_CONFIG_PLATFORM_H */










