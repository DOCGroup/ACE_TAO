/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/ctype.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_CTYPE_H_INDIRECT
#define PACE_CTYPE_H_INDIRECT

#include "pace/config/defines.h"

#if defined (PACE_HAS_POSIX)
# include "pace/posix/ctype.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/ctype.h"
#elif defined (PACE_WIN32)
# include "pace/win32/ctype.h"
#endif

#endif /* PACE_CTYPE_H_INDIRECT */



