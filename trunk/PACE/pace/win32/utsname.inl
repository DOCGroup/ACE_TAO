/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/win32/utsname.inl
 *
 * = AUTHOR
 *    Luther Baker
 *
 * ============================================================================= */

#if (PACE_HAS_POSIX_SP_UOF)
PACE_INLINE
int
pace_uname (struct utsname * name)
{
  return uname (name);
}
#endif /* PACE_HAS_POSIX_SP_UOF */
