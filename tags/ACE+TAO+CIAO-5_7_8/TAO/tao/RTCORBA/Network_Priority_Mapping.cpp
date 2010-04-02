// $Id$

#include "tao/orbconf.h"

#include "tao/RTCORBA/Network_Priority_Mapping.h"

ACE_RCSID(RTCORBA,
          Network_Priority_Mapping,
          "$Id$")

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Network_Priority_Mapping::~TAO_Network_Priority_Mapping (void)
{
}

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
