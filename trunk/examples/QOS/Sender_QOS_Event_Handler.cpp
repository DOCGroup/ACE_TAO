/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    Sender_QOS_Event_Handler.cpp
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#include "Sender_QOS_Event_Handler.h"

// Constructor.
ACE_QOS_Event_Handler::ACE_QOS_Event_Handler (void)
{
}

// Constructor.
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

// Handle the QoS Event. In this case send data to the receiver 
// using WSASendTo() that uses overlapped I/O.

int
ACE_QOS_Event_Handler::handle_qos (ACE_HANDLE)
{

  ACE_DEBUG ((LM_DEBUG,
              "\nReceived a QOS event. Inside handle_qos ()\n"));
 
  ACE_OVERLAPPED ace_overlapped;

#if (defined (ACE_HAS_WINSOCK2) && (ACE_HAS_WINSOCK2 != 0))
  const iovec iov = {5,"Hello"};
#else
  const iovec iov = {"Hello", 5};
#endif

  // For some really weird reason if I do not define the following 
  // sockaddr_in, the send () call fails.
  
  sockaddr_in s;
  ACE_UNUSED_ARG (s);

  ACE_INET_Addr send_to_addr (MY_DEFPORT, DEFAULT_MULTICASTGROUP);

  size_t bytes_sent;

  if (this->dgram_mcast_.send (&iov,
                               1,
                               bytes_sent,
                               0,
                               send_to_addr,
                               &ace_overlapped,
                               NULL) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in dgram_mcast.send ()\n"),
                      -1);

  else
    ACE_DEBUG ((LM_DEBUG,
                "Using ACE_OS::sendto () : Bytes sent : %d",
                bytes_sent));

  return 0;


}

