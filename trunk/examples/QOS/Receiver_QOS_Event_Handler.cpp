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
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#include "Receiver_QOS_Event_Handler.h"

// Constructor.
ACE_QOS_Event_Handler::ACE_QOS_Event_Handler (void)
{
}

ACE_QOS_Event_Handler::ACE_QOS_Event_Handler (const ACE_SOCK_Dgram_Mcast &dgram_mcast)
  : dgram_mcast_ (dgram_mcast)
{
}

// Destructor.
ACE_QOS_Event_Handler::~ACE_QOS_Event_Handler (void)
{
}

// Return the handle of the Dgram_Mcast. This method is called
// internally by the reactor.

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
  char buf[BUFSIZ];
	
  iovec iov;
  iov.iov_base = buf;
  iov.iov_len = BUFSIZ;
	
  ACE_OS::memset (iov.iov_base,
                  0,
                  BUFSIZ);

  ACE_DEBUG ((LM_DEBUG,
              "Inside handle_input () of ACE_Read_Handler ()\n"));

  // Receive message from multicast group.
  ssize_t result =
    this->dgram_mcast_.recv (&iov,
                             1,
                             this->remote_addr_);

  if (result != -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Message Received %s",
                  iov.iov_base));
      return 0;
    }
  else
    return -1;
}

