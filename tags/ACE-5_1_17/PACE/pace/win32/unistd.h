/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/unistd.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_UNISTD_H_WIN32
#define PACE_UNISTD_H_WIN32

#include "pace/sys/types.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

# ifndef PACE_F_OK
# define PACE_F_OK F_OK
# endif /* PACE_F_OK */

# ifndef PACE_NULL
# define PACE_NULL NULL
# endif /* PACE_NULL */

# ifndef PACE_R_OK
# define PACE_R_OK R_OK
# endif /* PACE_R_OK */

# ifndef PACE_SEEK_CUR
# define PACE_SEEK_CUR SEEK_CUR
# endif /* PACE_SEEK_CUR */

# ifndef PACE_SEEK_END
# define PACE_SEEK_END SEEK_END
# endif /* PACE_SEEK_END */

# ifndef PACE_SEEK_SET
# define PACE_SEEK_SET SEEK_SET
# endif /* PACE_SEEK_SET */

# ifndef PACE_STDERR_FILENO
# define PACE_STDERR_FILENO STDERR_FILENO
# endif /* PACE_STDERR_FILENO */

# ifndef PACE_STDIN_FILENO
# define PACE_STDIN_FILENO STDIN_FILENO
# endif /* PACE_STDIN_FILENO */

# ifndef PACE_STDOUT_FILENO
# define PACE_STDOUT_FILENO STDOUT_FILENO
# endif /* PACE_STDOUT_FILENO */

# ifndef PACE_W_OK
# define PACE_W_OK W_OK
# endif /* PACE_W_OK */

# ifndef PACE_X_OK
# define PACE_X_OK X_OK
# endif /* PACE_X_OK */

/* _POSIX _PC _SC Macros go here */

#ifndef PACE_SIZE_T
#define PACE_SIZE_T 1
typedef size_t pace_size_t;
#endif /* PACE_SIZE_T */

#ifndef PACE_SSIZE_T
#define PACE_SSIZE_T 1
typedef ssize_t pace_ssize_t;
#endif /* PACE_SSIZE_T */

#if (PACE_HAS_POSIX_DI_UOF)
  int pace_win32_close (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_FM_UOF)
  PACE_HANDLE pace_win32_dup (PACE_HANDLE fildes);
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_NONUOF_FUNCS)
  int pace_win32_ftruncate (PACE_HANDLE fildes, pace_off_t length);
#endif /* PACE_HAS_POSIX_NONUOF_FUNCS */

#if (PACE_HAS_POSIX_FM_UOF)
  pace_off_t pace_win32_lseek (PACE_HANDLE fildes,
                               pace_off_t offset,
                               int whence);
#endif /* PACE_HAS_POSIX_FM_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
  pace_ssize_t pace_win32_write (PACE_HANDLE fildes,
                                 const void * buf,
                                 pace_size_t nbyte);
#endif /* PACE_HAS_POSIX_DI_UOF */

#if (PACE_HAS_POSIX_DI_UOF)
  pace_ssize_t pace_win32_read (PACE_HANDLE fildes,
                                void * buf,
                                pace_size_t nbyte);
#endif /* PACE_HAS_POSIX_DI_UOF */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_UNISTD_H_WIN32 */




