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

  PACE_TRACE("pace_uname");

  pace_strcpy (name->sysname, "VxWorks");
#if (PACE_VXWORKS == 531)
  pace_strcpy (name->release, "5.3.1");
#elif (PACE_VXWORKS == 540)
  pace_strcpy (name->release, "5.4.0");
#endif /* PACE_VXWORKS != 531 */
  pace_strcpy (name->version, sysBspRev ());
  pace_strcpy (name->machine, sysModel ());
 
  return gethostname (name->nodename, maxnamelen);
}
#endif /* PACE_HAS_POSIX_SP_UOF */
