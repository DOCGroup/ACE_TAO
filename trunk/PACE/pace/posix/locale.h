/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/posix/locale.h
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * =========================================================================== */

#ifndef PACE_LOCALE_H_POSIX
#define PACE_LOCALE_H_POSIX

#if defined (PACE_HAS_CPLUSPLUS)
extern "C" {
#endif /* PACE_HAS_CPLUSPLUS */

#define PACE_LC_ALL LC_ALL
#define PACE_LC_COLLATE LC_COLLATE
#define PACE_LC_CTYPE LC_CTYPE
#define PACE_LC_MONETARY LC_MONETARY
#define PACE_LC_NUMERIC LC_NUMERIC
#define PACE_LC_TIME LC_TIME
#define PACE_NULL NULL

#ifndef PACE_LCONV
#define PACE_LCONV
typedef struct lconv pace_lconv;
#endif /* PACE_LCONV */

#if defined (PACE_HAS_CPLUSPLUS)
}
#endif /* PACE_HAS_CPLUSPLUS */

#endif /* PACE_LOCALE_H_POSIX */
