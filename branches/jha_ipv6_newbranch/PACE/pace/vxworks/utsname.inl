/* $Id$ -*- C -*-

 * =============================================================================
 *
 * = LIBRARY
 *    pace
 *
 * = FILENAME
 *    pace/vxworks/utsname.inl
 *
 * = AUTHOR
 *    Joe Hoffert
 *
 * ============================================================================= */

#include "pace/string.h"
#include <hostLib.h>
#include <sysLib.h>

#if (PACE_HAS_POSIX_SP_UOF)
PACE_INLINE
int
pace_uname (pace_utsname * name)
{
  size_t maxnamelen = sizeof name->nodename;
  pace_strcpy (name->sysname, "VxWorks");
  pace_strcpy (name->release, "???");
  pace_strcpy (name->version, sysBspRev ());
  pace_strcpy (name->machine, sysModel ());
 
  return gethostname (name->nodename, maxnamelen);
}
#endif /* PACE_HAS_POSIX_SP_UOF */
