/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    utsname.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */


PACE_INLINE
int
pace_uname (struct utsname * name)
{
  return uname (name);
}
