// DEV.cpp
// $Id$


#define ACE_BUILD_DLL
#include "ace/DEV.h"

ACE_ALLOC_HOOK_DEFINE(ACE_DEV)

void
ACE_DEV::dump (void) const
{
  ACE_TRACE ("ACE_DEV::dump");
}

// Return the local endpoint address.

int 
ACE_DEV::get_local_addr (ACE_DEV_Addr &addr) const
{
  ACE_TRACE ("ACE_DEV::get_local_addr");

  addr = this->addr_;
  return 0;
}

// Return the address of the remotely connected peer (if there is
// one).

int 
ACE_DEV::get_remote_addr (ACE_DEV_Addr &addr) const
{
  ACE_TRACE ("ACE_DEV::get_remote_addr");
  addr = this->addr_;
  return 0;
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
