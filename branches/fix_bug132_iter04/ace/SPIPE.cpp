// SPIPE.cpp
// $Id$

#include "ace/SPIPE.h"

#if defined (ACE_LACKS_INLINE_FUNCTIONS)
#include "ace/SPIPE.i"
#endif

ACE_RCSID(ace, SPIPE, "$Id$")

ACE_ALLOC_HOOK_DEFINE(ACE_SPIPE)

// This is the do-nothing constructor. 

ACE_SPIPE::ACE_SPIPE (void)
{
  // ACE_TRACE ("ACE_SPIPE::ACE_SPIPE");
}

void
ACE_SPIPE::dump (void) const
{
  ACE_TRACE ("ACE_SPIPE::dump");
}

// Close down a ACE_SPIPE. 

int
ACE_SPIPE::get_local_addr (ACE_SPIPE_Addr &local_sap) const
{
  ACE_TRACE ("ACE_SPIPE::get_local_addr");
  local_sap = this->local_addr_;
  return 0;
}

// Close down the STREAM pipe without removing the rendezvous point.

int
ACE_SPIPE::close (void)
{
  ACE_TRACE ("ACE_SPIPE::close");
  int result = 0;

  if (this->get_handle () != ACE_INVALID_HANDLE)
    {
      result = ACE_OS::close (this->get_handle ());
      this->set_handle (ACE_INVALID_HANDLE);
    }
  return result;
}

// Close down the STREAM pipe and remove the rendezvous point from the
// file system.

int
ACE_SPIPE::remove (void)
{
  ACE_TRACE ("ACE_SPIPE::remove");
  int result = this->close ();
  return ACE_OS::unlink (this->local_addr_.get_path_name ()) == -1 || result == -1 ? -1 : 0;
}

