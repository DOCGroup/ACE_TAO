// $Id$

#include "tao/orbconf.h"

#include "Network_Priority_Mapping.h"

#if !defined (__ACE_INLINE__)
# include "Network_Priority_Mapping.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(tao, Network_Priority_Mapping, "$Id$")

#if defined (TAO_HAS_CORBA_MESSAGING) && TAO_HAS_CORBA_MESSAGING != 0

TAO_Network_Priority_Mapping::~TAO_Network_Priority_Mapping (void)
{
}

#endif /* TAO_HAS_CORBA_MESSAGING && TAO_HAS_CORBA_MESSAGING != 0 */
