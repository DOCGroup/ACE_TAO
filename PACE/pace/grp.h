/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/grp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_GRP_H_INDIRECT
#define PACE_GRP_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/grp.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/grp.h"
#elif defined (PACE_WIN32)
# include "pace/win32/grp.h"
#endif

#endif /* PACE_GRP_H_INDIRECT */
