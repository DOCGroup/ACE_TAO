/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/mman.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */

#ifndef PACE_SYS_MMAN_H_WIN32
#define PACE_SYS_MMAN_H_WIN32

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

# define MAP_PRIVATE 1
# define MAP_SHARED  2
# define MAP_FIXED   4
# if !defined (MAP_FAILED)
#   undef MAP_FAILED
#   define MAP_FAILED ((void *) -1)
# endif

# define PACE_MAP_FAILED MAP_FAILED
# define PACE_MAP_FIXED MAP_FIXED
# define PACE_MAP_PRIVATE MAP_PRIVATE
# define PACE_MAP_SHARED MAP_SHARED
# define PACE_MCL_CURRENT MCL_CURRENT
# define PACE_MS_ASYNC MS_ASYNC
# define PACE_MS_INVALIDATE
# define PACE_MS_SYNC MS_SYNC
# define PACE_PROT_EXEC PROT_EXEC
# define PACE_PROT_NONE PROT_NONE
# define PACE_PROT_READ PROT_READ
# define PACE_PROT_WRITE PROT_WRITE

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_SYS_MMAN_H */
