// $Id$

#define ACE_BUILD_DLL
#include "ace/ATM_Acceptor.h"

ACE_RCSID(ace, ATM_Acceptor, "$Id$")

#if defined (ACE_HAS_ATM)

// Put the actual definitions of the ACE_ATM_Request and
// ACE_ATM_Request_Queue classes here to hide them from clients...

ACE_ALLOC_HOOK_DEFINE(ACE_ATM_Acceptor)

void
ACE_ATM_Acceptor::dump (void) const
{
  ACE_TRACE ("ACE_ATM_Acceptor::dump");
}

ACE_ATM_Acceptor::ACE_ATM_Acceptor (void)
{
  ACE_TRACE ("ACE_ATM_Acceptor::ACE_ATM_Acceptor");
}

ACE_ATM_Acceptor::~ACE_ATM_Acceptor (void)
{
  ACE_TRACE ("ACE_ATM_Acceptor::~ACE_ATM_Acceptor");
}

#endif /* ACE_HAS_ATM */
