/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/utsname.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_UTSNAME_H
#define PACE_SYS_UTSNAME_H

#if defined (PACE_HAS_POSIX)
# include "posix/utsname.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/utsname.h"
#elif defined (PACE_WIN32)
# include "win32/utsname.h"
#endif

#endif /* PACE_SYS_UTSNAME_H */
