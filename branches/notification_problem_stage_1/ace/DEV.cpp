// DEV.cpp
// $Id$


#include "ace/DEV.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/DEV.i"
#endif

ACE_RCSID(ace, DEV, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_DEV)

void
ACE_DEV::dump (void) const
{
  ACE_TRACE ("ACE_DEV::dump");
}

// This is the do-nothing constructor. 

ACE_DEV::ACE_DEV (void)
{
  ACE_TRACE ("ACE_DEV::ACE_DEV");
}

// Close the device

int
ACE_DEV::close (void)
{
  ACE_TRACE ("ACE_DEV::close");
  int result = ACE_OS::close (this->get_handle ());
  this->set_handle (ACE_INVALID_HANDLE);
  return result;
}
