/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    platform.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_CONFIG_PLATFORM_H
#define PACE_CONFIG_PLATFORM_H


/* Added this because Linux does not support the pragma directive. */
# if defined (linux)
# define PACE_NOT_POSIX

/* I don't think this is correct <luther@cs.wustl.edu> May 16, 2000,
# elif defined PACE_NOT_POSIX
# undef PACE_NOT_POSIX
*/

# endif /* linux */

/* WHAT? <luther@cs.wustl.edu> May 16, 2000,
# if !defined PACE_NOT_POSIX
# define PACE_LACKS_PRAGMA_ONCE
# endif
*/

/* SEE THE ABOVE COMMENTED OUT? <luther@cs.wustl.edu> May 16, 2000, */
# if defined PACE_NOT_POSIX
# define PACE_LACKS_PRAGMA_ONCE
# endif







# if !defined (PACE_MALLOC_ALIGN)
# define PACE_MALLOC_ALIGN ((int) sizeof (long))
# endif /* PACE_MALLOC_ALIGN */

# if 'a' < 'A'
# define PACE_HAS_EBCDIC
# define PACE_STANDARD_CHARACTER_SET_SIZE 256
# else
# define PACE_HAS_ASCII
# define PACE_STANDARD_CHARACTER_SET_SIZE 128
# endif /* 'a' < 'A' */

# if defined (PACE_HAS_MOSTLY_UNICODE_APIS) && !defined (UNICODE)
# error UNICODE must be defined when using ACE_HAS_MOSTLY_UNICODE_APIS, check your compiler document on how to enable UNICODE.
# endif /* PACE_HAS_MOSTLY_UNICODE_APIS && !UNICODE */

# if defined (PACE_HAS_4_4BSD_SENDMSG_RECVMSG)
    // Control message size to pass a file descriptor.
#   define PACE_BSD_CONTROL_MSG_LEN sizeof (struct cmsghdr) + sizeof (PACE_HANDLE)
#   if defined (PACE_LACKS_CMSG_DATA_MACRO)
#     if defined (PACE_LACKS_CMSG_DATA_MEMBER)
#       define CMSG_DATA(cmsg) ((unsigned char *) ((struct cmsghdr *) (cmsg) + 1))
#     else
#       define CMSG_DATA(cmsg) ((cmsg)->cmsg_data)
#     endif /* PACE_LACKS_CMSG_DATA_MEMBER */
#   endif /* PACE_LACKS_CMSG_DATA_MACRO */
# endif /* PACE_HAS_4_4BSD_SENDMSG_RECVMSG */


#endif /* PACE_CONFIG_PLATFORM_H */
