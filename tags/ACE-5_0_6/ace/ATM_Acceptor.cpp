// $Id$

#define ACE_BUILD_DLL
#include "ace/ATM_Acceptor.h"

ACE_RCSID(ace, ATM_Acceptor, "$Id$")

#if defined (ACE_HAS_ATM)

#if !defined (__ACE_INLINE__)
#include "ace/ATM_Acceptor.i"
#endif /* __ACE_INLINE__ */

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
