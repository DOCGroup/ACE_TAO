/* $Id$ -*- C -*-

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/assert.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#ifndef PACE_ASSERT_H
#define PACE_ASSERT_H

#if defined (PACE_HAS_POSIX)
# include "posix/assert.h"
#elif defined (PACE_VXWORKS)
# include "vxworks/assert.h"
#elif defined (PACE_WIN32)
# include "win32/assert.h"
#endif

#endif /* PACE_ASSERT_H */



