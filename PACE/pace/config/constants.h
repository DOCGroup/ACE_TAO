/* $Id$

 * ============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    constants.h
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================ */


#ifndef PACE_CONFIG_CONSTANTS_H
#define PACE_CONFIG_CONSTANTS_H


/* States of a recyclable object.*/
enum ACE_Recyclable_State
{
  ACE_RECYCLABLE_IDLE_AND_PURGABLE,
  /* Idle and can be purged.*/

  ACE_RECYCLABLE_IDLE_BUT_NOT_PURGABLE,
  /* Idle but cannot be purged. */

  ACE_RECYCLABLE_PURGABLE_BUT_NOT_IDLE,
  /* Can be purged, but is not idle (mostly for debugging). */

  ACE_RECYCLABLE_BUSY,
  /* Busy (i.e., cannot be recycled or purged). */

  ACE_RECYCLABLE_CLOSED,
  /* Closed. */

  ACE_RECYCLABLE_UNKNOWN
  /* Unknown state. */
};

/* Do not change these values wantonly since GPERF depends on them.. */
#define PACE_ASCII_SIZE 128
#define PACE_EBCDIC_SIZE 256

/* The maximum length for a fully qualified Internet name. */
# if !defined (PACE_MAX_FULLY_QUALIFIED_NAME_LEN)
#   define PACE_MAX_FULLY_QUALIFIED_NAME_LEN 256
# endif /* PACE_MAX_FULLY_QUALIFIED_NAME_LEN */

/* Maximum host namelength. */
# if !defined(PACE_MAX_HOST_NAME_LEN)
# define PACE_MAX_HOST_NAME_LEN  256
# endif /* PACE_MAXHOSTNAMELEN */

#endif /* PACE_CONFIG_CONSTANTS_H */
