/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/dirent.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_DIRENT_H_WIN32
#define PACE_DIRENT_H_WIN32

#include "pace/sys/types.h"
#include "windows.h"

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#ifndef PACE_DIRENT_T
#define PACE_DIRENT_T

  typedef struct /* dirent */ {
    unsigned short d_ino;
    unsigned short d_off;
    unsigned short d_reclen;
    char * d_name;
  } pace_dirent;

#endif /* PACE_DIRENT_T */

#ifndef PACE_DIR_T
#define PACE_DIR_T

  typedef WIN32_FIND_DATA PACE_TEXT_WIN32_FIND_DATA;

  typedef struct /* DIR */ {
    char * directory_name_;
    // The name of the directory we are looking into
    HANDLE current_handle_;
    // Remember the handle between calls.
    pace_dirent dirent_;
    // The struct for the results
    PACE_TEXT_WIN32_FIND_DATA fdata_;
    // The struct for intermediate results.
    int started_reading_;
    // A flag to remember if we started reading already.
  } PACE_DIR;

#endif /* PACE_DIR_T */

#if (PACE_HAS_POSIX_FS_UOF)
  int closedir (PACE_DIR * dirp);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
  PACE_DIR * opendir (const char * dirname);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if (PACE_HAS_POSIX_FS_UOF)
  pace_dirent * readdir (PACE_DIR * dirp);
#endif /* PACE_HAS_POSIX_FS_UOF */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_DIRENT_H_WIN32 */
