/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/utsname.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================ */

#ifndef PACE_SYS_UTSNAME_H_VXWORKS
#define PACE_SYS_UTSNAME_H_VXWORKS

#include "pace/stddef.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_UTSNAME
#define PACE_UTSNAME
# if !defined (SYS_NMLN)
#   define SYS_NMLN 257
# endif /* SYS_NMLN */
# if !defined (_SYS_NMLN)
#   define _SYS_NMLN SYS_NMLN
# endif /* _SYS_NMLN */
typedef struct
{
  pace_tchar sysname[_SYS_NMLN];
  pace_tchar nodename[_SYS_NMLN];
  pace_tchar release[_SYS_NMLN];
  pace_tchar version[_SYS_NMLN];
  pace_tchar machine[_SYS_NMLN];
} pace_utsname;
#endif /* PACE_UTSNAME */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_UTSNAME_H_VXWORKS */
