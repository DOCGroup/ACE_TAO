/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/utsname.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_UTSNAME_H_WIN32
#define PACE_SYS_UTSNAME_H_WIN32

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

# ifndef PACE_UTSNAME
# define PACE_UTSNAME
# define PACE_SYS_NMLN 257
  typedef struct /* utsname */ {
    char sysname[PACE_SYS_NMLN];
    char nodename[PACE_SYS_NMLN];
    char release[PACE_SYS_NMLN];
    char version[PACE_SYS_NMLN];
    char machine[PACE_SYS_NMLN];
} pace_utsname;
# endif /* PACE_UTSNAME */

#if (PACE_HAS_POSIX_SP_UOF)
  int uname (pace_utsname * name);
#endif /* PACE_HAS_POSIX_SP_UOF */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_UTSNAME_H_WIN32 */
