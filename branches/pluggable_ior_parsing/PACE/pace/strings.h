/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/strings.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_STRINGS_H_INDIRECT
#define PACE_STRINGS_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/strings.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/strings.h"
#elif defined (PACE_WIN32)
# include "pace/win32/strings.h"
#endif

#endif /* PACE_STRINGS_H_INDIRECT */
