/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    ace_sys_types.h
 *
 *  $Id$
 *
 *  @author Douglas C. Schmidt (schmidt@cs.wustl.edu)
 *  @author etc
 */
//=============================================================================

#ifndef ACE_ACE_SYS_TYPES_H
# define ACE_ACE_SYS_TYPES_H
# include "ace/pre.h"

# include "ace/config-all.h"

# if !defined (ACE_LACKS_PRAGMA_ONCE)
#   pragma once
# endif /* ACE_LACKS_PRAGMA_ONCE */

# if !defined (ACE_LACKS_SYS_TYPES_H)
#   include /**/ <sys/types.h>
# else /* !ACE_LACKS_SYS_TYPES_H */
// For right now, assume that if you don't have sys/types, you
// don't have these either.
// Might need to add 1 0 to these if we have problems.
#   define ACE_LACKS_U_CHAR
#   define ACE_LACKS_UCHAR_T
#   define ACE_LACKS_U_SHORT
#   define ACE_LACKS_USHORT_T
#   define ACE_LACKS_U_INT
#   define ACE_LACKS_UINT_T
#   define ACE_LACKS_U_LONG
#   define ACE_LACKS_ULONG_T
# endif /* !ACE_LACKS_SYS_TYPES_H && !ACE_LACKS_UNIX_SYS_HEADERS */

# if defined (ACE_HAS_TYPES_H)
#   include /**/ <types.h>
# endif /* ACE_HAS_TYPES_H */

# if defined (ACE_LACKS_U_CHAR)
  typedef unsigned char u_char;
# endif /* ACE_LACKS_U_CHAR */

# if defined (ACE_LACKS_UCHAR_T)
  typedef u_char uchar_t;
# endif /* ACE_LACKS_UCHAR_T */

# if defined (ACE_LACKS_U_SHORT)
  typedef unsigned short u_short;
# endif /* ACE_LACKS_U_CHAR */

# if defined (ACE_LACKS_USHORT_T)
  typedef u_short ushort_t;
# endif /* ACE_LACKS_UCHAR_T */

# if defined (ACE_LACKS_U_INT)
  typedef unsigned int u_int;
# endif /* ACE_LACKS_U_INT */

# if defined (ACE_LACKS_UINT_T)
  typedef u_int uint_t;
# endif /* ACE_LACKS_UINT_T */

# if defined (ACE_LACKS_U_LONG)
  typedef unsigned long u_long;
# endif /* ACE_LACKS_U_LONG */

# if defined (ACE_LACKS_ULONG_T)
  typedef u_long ulong_t;
# endif /* ACE_LACKS_ULONG_T */

# if defined (ACE_LACKS_CADDR_T)
typedef char* caddr_t;
# endif /* ACE_LACKS_CDDR_T */

# if defined (ACE_LACKS_CLOCK_T)
typedef long int clock_t;
# endif /* ACE_LACKS_CLOCK_T */

# if defined (ACE_LACKS_COMP_T)
typedef u_short comp_t;
# endif /* ACE_LACKS_COMP_T */

# if defined (ACE_LACKS_DEV_T)
typedef u_int dev_t;
# endif /* ACE_LACKS_DEV_T */

# if defined (ACE_LACKS_FD_SET_T) || defined (ACE_FD_SET_TYPE)
#   if !defined (ACE_FD_SET_TYPE)
#     define ACE_FD_SET_TYPE fd_set
#   endif /* ACE_FD_SET_TYPE */  
typedef ACE_FD_SET_TYPE fd_set;
# endif /* ACE_LACKS_FD_SET_T */

// This is for select calls, set to int in your config-*.h file if
// your os doesn't use fd_set in select calls.
# if !defined (ACE_SELECT_FD_SET_TYPE)
#   define ACE_SELECT_FD_SET_TYPE fd_set
# endif /* ACE_FD_SET_TYPE */

# if defined (ACE_LACKS_FPOS_T)
// another struct
//typedef int fpos_t;
# endif /* ACE_LACKS_FPOS_T */

# if defined (ACE_LACKS_GID_T) || defined (ACE_GUID_T_TYPE)
#   if !defined (ACE_GID_T_TYPE)
#     define ACE_GID_T_TYPE u_int
#   endif /* !ACE_GID_T_TYPE */
typedef ACE_GID_T_TYPE gid_t;
# endif /* ACE_LACKS_GID_T */

# if defined (ACE_LACKS_INO_T)
typedef u_long ino_t;
# endif /* ACE_LACKS_INO_T */

# if defined (ACE_LACKS_KEY_T) || defined (ACE_KEY_T_TYPE)
#   if !defined (ACE_KEY_T_TYPE)
#     define ACE_KEY_T_TYPE long int
#   endif /* ACE_KEY_T_TYPE */
typedef ACE_KEY_T_TYPE key_t;
# endif /* ACE_LACKS_KEY_T */

# if defined (ACE_LACKS_MODE_T) || defined (ACE_MODE_T_TYPE)
#   if !defined (ACE_MODE_T_TYPE)
#     define ACE_MODE_T_TYPE u_int
#   endif /* !ACE_MODE_T_TYPE */
typedef ACE_MODE_T_TYPE mode_t;
# endif /* ACE_LACKS_MODE_T */

# if defined (ACE_LACKS_NLINK_T) || defined (ACE_NLINK_T_TYPE)
#   if !defined (ACE_NLINK_T_TYPE)
#     define ACE_NLINK_T_TYPE u_int
#   endif /* !ACE_NLINK_T_TYPE */
typedef ACE_NLINK_T_TYPE nlink_t;
# endif /* ACE_LACKS_NLINK_T */

# if defined (ACE_LACKS_OFF_T)
typedef long int off_t;
# endif /* ACE_LACKS_OFF_T */

# if defined (ACE_LACKS_OFF_T)
typedef long long int loff_t;
# endif /* ACE_LACKS_OFF_T */

# if defined (ACE_LACKS_PID_T) || defined (ACE_PID_T_TYPE)
#   if !defined (ACE_PID_T_TYPE)
#     define ACE_PID_T_TYPE int
#   endif /* !ACE_PID_T_TYPE */
typedef ACE_PID_T_TYPE pid_t;
# endif /* ACE_LACKS_PID_T */

# if defined (ACE_LACKS_PTRDIFF_T)
typedef int ptrdiff_t;
# endif /* ACE_LACKS_PTRDIFF_T */

# if defined (ACE_LACKS_RLIM_T)
typedef u_long rlim_t;
# endif /* ACE_LACKS_RLIM_T */

# if defined (ACE_LACKS_SIG_ATOMIC_T)
typedef int sig_atomic_t;
# endif /* ACE_LACKS_SIG_ATOMIC_T */

# if defined (ACE_LACKS_SIZE_T)
typedef u_int size_t;
# endif /* ACE_LACKS_SIZE_T */

# if defined (ACE_LACKS_SSIZE_T)
typedef int ssize_t;
# endif /* ACE_LACKS_SSIZE_T */

# if defined (ACE_LACKS_TIME_T)
typedef long int time_t;
# endif /* ACE_LACKS_TIME_T */

# if defined (ACE_LACKS_UID_T) || defined (ACE_UID_T_TYPE)
#   if !defined (ACE_UID_T_TYPE)
#     define ACE_UID_T_TYPE u_int
#   endif /* !ACE_UID_T_TYPE */
typedef ACE_UID_T_TYPE uid_t;
# endif /* ACE_LACKS_UID_T */

# if defined (ACE_LACKS_WCHAR_T)
typedef u_long wchar_t;
# endif /* ACE_LACKS_WCHAR_T */

#endif /* ACE_ACE_SYS_TYPES_H */
