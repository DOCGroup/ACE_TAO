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

#ifndef PACE_LOCALE_H
#define PACE_LOCALE_H

#if defined (PACE_HAS_POSIX)
# include "posix/locale.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/locale.h"
#elif defined (PACE_WIN32)
# include "win32/locale.h"
#endif

#endif /* PACE_LOCALE_H */
