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
//    Vishal Kachroo
//
// ============================================================================

#include "Sender_QOS_Event_Handler.h"

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

  WSAEVENT hevent = WSACreateEvent ();
  ACE_OVERLAPPED ace_overlapped;
  ace_overlapped.hEvent = hevent;

  SOCKADDR_IN sendto_addr;
  ZeroMemory((PVOID)&sendto_addr, sizeof(sendto_addr));
  sendto_addr.sin_family = PF_INET;							// protocol_info.iAddressFamily;
  sendto_addr.sin_addr.s_addr = inet_addr("234.5.6.7");		// options.szHostname);
  sendto_addr.sin_port = htons(5001);							// options.port);

  const SOCKADDR_IN c_sendto_addr = sendto_addr;

  const iovec iov = {5,"Hello"};
  size_t bytes_sent;

  if (ACE_OS::sendto (this->dgram_mcast_.get_handle (),
                      &iov,
                      1,
                      bytes_sent,
                      0,
                      (const struct sockaddr *) &c_sendto_addr,
                      sizeof (c_sendto_addr),
                      &ace_overlapped,
                      NULL) != 0)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in Sendto\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Using ACE_OS::sendto () : Bytes sent : %d",
                bytes_sent));

  return 0;
}

