/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/pwd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_PWD_H_INDIRECT
#define PACE_PWD_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/pwd.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/pwd.h"
#elif defined (PACE_WIN32)
# include "pace/win32/pwd.h"
#endif

#endif /* PACE_PWD_H_INDIRECT */
