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
#if defined (PACE_LINUX)
# define PACE_HAS_POSIX PACE_LINUX
#elif defined (PACE_LYNXOS)
# define PACE_HAS_POSIX PACE_LYNXOS
#elif defined (PACE_SUNOS)
# define PACE_HAS_POSIX PACE_SUNOS

/* These platforms are NOT POSIX compliant. */
#elif defined (PACE_VXWORKS)
# define PACE_LACKS_POSIX PACE_VXWORKS
#elif defined (PACE_WIN32)
# define PACE_LACKS_POSIX PACE_WIN32
#endif

#endif /* PACE_CONFIG_PLATFORM_H */










