/* $Id$ -*- C -*-

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/locale.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#ifndef PACE_LOCALE_H_INDIRECT
#define PACE_LOCALE_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/locale.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/locale.h"
#elif defined (PACE_WIN32)
# include "pace/win32/locale.h"
#endif

#endif /* PACE_LOCALE_H_INDIRECT */
