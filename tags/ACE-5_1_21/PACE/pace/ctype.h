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

#ifndef PACE_CTYPE_H
#define PACE_CTYPE_H

#include "pace/config/defines.h"

#if (PACE_HAS_POSIX)
# include "pace/posix/ctype.h"
#elif (PACE_VXWORKS)
# include "pace/vxworks/ctype.h"
#elif (PACE_WIN32)
# include "pace/win32/ctype.h"
#endif

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  /**
     PACE's implementation of the POSIX function isalnum.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_isalnum (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function isalpha.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_isalpha (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function iscntrl.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_iscntrl (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function isdigit.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_isdigit (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function isgraph.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_isgraph (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function islower.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_islower (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function isprint.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_isprint (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function ispunct.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_ispunct (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function isspace.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_isspace (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function isupper.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_isupper (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function isxdigit.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_isxdigit (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function tolower.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_tolower (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

  /**
     PACE's implementation of the POSIX function toupper.
     See POSIX standard (Internation Standard ISO/IEC 9945-1:1996;
     IEEE Std 1003.1, 1996 Edition), Section 8.1.
   */
#if (PACE_HAS_POSIX_CLS_UOF)
  PACE_Export PACE_INLINE int pace_toupper (int c);
#endif /* PACE_HAS_POSIX_CLS_UOF */

#if defined (PACE_HAS_INLINE)
#  if (PACE_HAS_POSIX)
#    include "pace/posix/ctype.inl"
#  elif (PACE_VXWORKS)
#    include "pace/vxworks/ctype.inl"
#  elif (PACE_WIN32)
#    include "pace/win32/ctype.inl"
#  endif
#endif /* PACE_HAS_INLINE */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_CTYPE_H */
