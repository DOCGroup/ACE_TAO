// ATM_Connector.cpp
// $Id$

#define ACE_BUILD_DLL
#include "ace/Handle_Set.h"
#include "ace/ATM_Connector.h"

ACE_RCSID(ace, ATM_Connector, "$Id$")

#if defined (ACE_HAS_ATM)

#if !defined (__ACE_INLINE__)
#include "ace/ATM_Connector.i"
#endif /* __ACE_INLINE__ */

ACE_ALLOC_HOOK_DEFINE(ACE_ATM_Connector)

void
ACE_ATM_Connector::dump (void) const
{
  ACE_TRACE ("ACE_ATM_Connector::dump");
}

ACE_ATM_Connector::ACE_ATM_Connector (void)
{
  ACE_TRACE ("ACE_ATM_Connector::ACE_ATM_Connector");
}

#endif /* ACE_HAS_ATM */
