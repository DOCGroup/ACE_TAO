/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    fcntl.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


PACE_INLINE
int
pace_creat (const char * path, mode_t mode)
{
  return creat (path, mode);
}
