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

#ifndef PACE_SYS_UTSNAME_H_INDIRECT
#define PACE_SYS_UTSNAME_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/utsname.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/utsname.h"
#elif defined (PACE_WIN32)
# include "pace/win32/utsname.h"
#endif

#endif /* PACE_SYS_UTSNAME_H_INDIRECT */
