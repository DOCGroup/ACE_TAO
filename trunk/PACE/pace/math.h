/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = Filename
 *    pace/math.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_MATH_H_INDIRECT
#define PACE_MATH_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/math.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/math.h"
#elif defined (PACE_WIN32)
# include "pace/win32/math.h"
#endif

#endif /* PACE_MATH_H_INDIRECT */




