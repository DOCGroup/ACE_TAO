/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    times.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


PACE_INLINE
clock_t
pace_times (struct tms * buffer)
{
  return times (buffer);
}
