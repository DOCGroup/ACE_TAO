/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    Receiver_QOS_Event_Handler.cpp
//
// = AUTHOR
//    Vishal Kachroo
//
// ============================================================================

#include "Receiver_QOS_Event_Handler.h"

// Constructor.
ACE_QOS_Event_Handler::ACE_QOS_Event_Handler ()
{}

ACE_QOS_Event_Handler::ACE_QOS_Event_Handler (const ACE_SOCK_Dgram_Mcast &dgram_mcast)
{
  this->dgram_mcast_ = dgram_mcast;
}

// Destructor.
ACE_QOS_Event_Handler::~ACE_QOS_Event_Handler ()
{
}

// Return the handle of the Dgram_Mcast. This method is 
// called internally by the reactor.

ACE_HANDLE
ACE_QOS_Event_Handler::get_handle (void) const
{
  return this->dgram_mcast_.get_handle ();
}

int
ACE_QOS_Event_Handler::handle_qos (ACE_HANDLE)
{

  ACE_DEBUG ((LM_DEBUG,
              "\nReceived a QOS event. Inside handle_qos ()\n"));

  return 0;
}

// Called when there is a READ activity on the dgram_mcast handle.
int
ACE_QOS_Event_Handler::handle_input (ACE_HANDLE)
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

