/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/sys/utsname.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_UTSNAME_H
#define PACE_SYS_UTSNAME_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/utsname.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/utsname.h"
#elif (PACE_WIN32)
# include "pace/win32/utsname.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#if (PACE_HAS_POSIX_SP_UOF)
  PACE_Export PACE_INLINE int pace_uname (pace_utsname * name);
#endif /* PACE_HAS_POSIX_SP_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/utsname.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/utsname.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/utsname.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_UTSNAME_H */
