/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/pwd.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_PWD_H_VXWORKS
#define PACE_PWD_H_VXWORKS

#include "pace/sys/types.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_PASSWD
#define PACE_PASSWD
  typedef struct passwd pace_passwd;
#endif /* PACE_PASSWD */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_PWD_H_VXWORKS */
