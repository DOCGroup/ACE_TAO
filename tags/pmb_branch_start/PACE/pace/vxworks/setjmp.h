/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/setjmp.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_SETJMP_H_VXWORKS
#define PACE_SETJMP_H_VXWORKS

#include <types/vxTypesOld.h>
#include <setjmp.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

  typedef jmp_buf pace_jmp_buf;
  typedef sigjmp_buf pace_sigjmp_buf;

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SETJMP_H_VXWORKS */
