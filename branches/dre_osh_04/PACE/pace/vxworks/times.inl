/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/times.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_MP_UOF)
PACE_INLINE
pace_clock_t
pace_times (pace_tms * buffer)
{
  PACE_TRACE("pace_times");

  PACE_UNUSED_ARG(buffer);
  PACE_ERRNO_NO_SUPPORT_RETURN (-1);
}
#endif /* PACE_HAS_POSIX_MP_UOF */
