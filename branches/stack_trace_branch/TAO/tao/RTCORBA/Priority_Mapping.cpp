// $Id$

#include "tao/orbconf.h"

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

#include "tao/RTCORBA/Priority_Mapping.h"

ACE_RCSID(RTCORBA,
          Priority_Mapping,
          "$Id$")

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Priority_Mapping::~TAO_Priority_Mapping (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
