/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/string.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STRING_H_INDIRECT
#define PACE_STRING_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/string.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/string.h"
#elif defined (PACE_WIN32)
# include "pace/win32/string.h"
#endif

#endif /* PACE_STRING_H_INDIRECT */
