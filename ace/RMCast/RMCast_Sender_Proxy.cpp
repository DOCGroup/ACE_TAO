// $Id$

#include "RMCast_Sender_Proxy.h"
#include "RMCast_Module.h"
#include "ace/Message_Block.h"

#if !defined (__ACE_INLINE__)
# include "RMCast_Sender_Proxy.i"
#endif /* ! __ACE_INLINE__ */

ACE_RCSID(ace, RMCast_Sender_Proxy, "$Id$")

ACE_RMCast_Sender_Proxy::ACE_RMCast_Sender_Proxy (ACE_RMCast_Module *module)
  : module_ (module)
{
}

ACE_RMCast_Sender_Proxy::~ACE_RMCast_Sender_Proxy (void)
{
}
