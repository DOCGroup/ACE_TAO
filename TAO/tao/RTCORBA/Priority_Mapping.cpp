// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "Priority_Mapping.h"

ACE_RCSID(RTCORBA,
          Priority_Mapping,
          "$Id$")

TAO_Priority_Mapping::~TAO_Priority_Mapping (void)
{
}
#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
