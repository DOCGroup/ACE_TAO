/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/setjmp.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SETJMP_H_INDIRECT
#define PACE_SETJMP_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/setjmp.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/setjmp.h"
#elif defined (PACE_WIN32)
# include "pace/win32/setjmp.h"
#endif

#endif /* PACE_SETJMP_H_INDIRECT */
