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

#ifndef PACE_ASSERT_H_INDIRECT
#define PACE_ASSERT_H_INDIRECT

#if defined (PACE_HAS_POSIX)
# include "pace/posix/assert.h"
#elif defined (PACE_VXWORKS)
# include "pace/vxworks/assert.h"
#elif defined (PACE_WIN32)
# include "pace/win32/assert.h"
#endif

#endif /* PACE_ASSERT_H_INDIRECT */



