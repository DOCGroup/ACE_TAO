/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/stat.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_STAT_H_POSIX
#define PACE_SYS_STAT_H_POSIX

#include <sys/stat.h>

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_S_IRGRP S_IRGRP
#define PACE_S_IROTH S_IROTH
#define PACE_S_IRUSR S_IRUSR
#define PACE_S_IRWXG S_IRWXG
#define PACE_S_IRWXO S_IRWXO
#define PACE_S_IRWXU S_IRWXU
#define PACE_S_ISBLK S_ISBLK
#define PACE_S_ISCHR S_ISCHR
#define PACE_S_ISDIR S_ISDIR
#define PACE_S_ISFIFO S_ISFIFO
#define PACE_S_ISGID S_SIGID
#define PACE_S_ISREG S_ISREG
#define PACE_S_ISUID S_ISUID
#define PACE_S_IWGRP S_IWGRP
#define PACE_S_IWOTH S_IWOTH
#define PACE_S_IWUSR S_IWUSR
#define PACE_S_IXGRP S_IXGRP
#define PACE_S_IXOTH S_IXOTH
#define PACE_S_IXUSR S_IXUSR
#define PACE_S_TYPEISMQ S_TYPEISMQ
#define PACE_S_TYPEISSEM S_TYPEISSEM
#define PACE_S_TYPEISSHM S_TYPEISSHM

#ifndef PACE_STAT
#define PACE_STAT
  /* Kludge since there is a func and a type of the same name */
  typedef struct stat pace_stat_s;
#endif /* PACE_STAT */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_STAT_H_POSIX */
