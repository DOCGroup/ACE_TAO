/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/defines.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_DEFINES_H_INDIRECT
#define PACE_DEFINES_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/defines.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/defines.h"
#elif defined (PACE_WIN32)
# include "pace/win32/defines.h"
#endif

#endif /* PACE_DEFINES_H_INDIRECT */
