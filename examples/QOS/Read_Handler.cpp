/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS/
//
// = FILENAME
//    Read_Handler.cpp
//
// = AUTHOR
//    Vishal Kachroo
//
// ============================================================================

#include "Read_Handler.h"

// Constructor.
ACE_Read_Handler::ACE_Read_Handler ()
{};

// Destructor.
ACE_Read_Handler::~ACE_Read_Handler ()
{};

// Return the handle of the Dgram_Mcast. This method is 
// called internally by the reactor.

ACE_HANDLE
ACE_Read_Handler::get_handle (void) const
{
  return this->dgram_mcast_.get_handle ();
}

// Called when there is a READ activity on the dgram_mcast handle.
int
ACE_Read_Handler::handle_input (ACE_HANDLE)
{
  char buf[128];
	
  const int iovcnt = 1;
  iovec iov[iovcnt];
  iov[0].iov_base = buf;
  iov[0].iov_len = 128;
	
  ACE_OS::memset (iov[0].iov_base, 0, 128);

  ACE_DEBUG ((LM_DEBUG,
              "Inside handle_input () of ACE_Read_Handler ()\n"));

  // Receive message from multicast group.
  ssize_t retcode =
    this->dgram_mcast_.recv (iov, 1, this->remote_addr_);

  if (retcode != -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Message Received %s",
                  iov[0].iov_base));

      return 0;
    }
  else
    return -1;
}

// Set for a dgram_mcast;
void
ACE_Read_Handler::dgram_mcast (const ACE_SOCK_Dgram_Mcast &dgram_mcast)
{
  this->dgram_mcast_ = dgram_mcast;
}
