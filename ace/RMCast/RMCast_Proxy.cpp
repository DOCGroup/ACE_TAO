//
// $Id$
//

#include "RMCast_Proxy.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Proxy.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Proxy, "$Id$")

ACE_RMCast_Proxy::~ACE_RMCast_Proxy (void)
{
}

ACE_UINT32
ACE_RMCast_Proxy::highest_in_sequence (void) const
{
  return this->highest_in_sequence_;
}

ACE_UINT32
ACE_RMCast_Proxy::highest_received (void) const
{
  return this->highest_received_;
}
