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

  ACE_QoS ace_get_qos;
  u_long dwBytes;

  if (ACE_OS::ioctl (this->dgram_mcast_.get_handle (),
                     ACE_SIO_GET_QOS,
                     ace_get_qos,
                     &dwBytes) == -1)
    ACE_ERROR ((LM_ERROR,
                "Error in Qos get ACE_OS::ioctl ()\n"
                "Bytes Returned = %d\n",
                dwBytes));
  else
    ACE_DEBUG ((LM_DEBUG,
                "Getting QOS using ACE_OS::ioctl () succeeds.\n"));

  ACE_DEBUG ((LM_DEBUG,
              "\nReceiving Flowspec :\t\t\tSending Flowspec :\n\n"
              "\tToken Rate = %d\t\t\tToken Rate = %d\n"
              "\tToken Bucket Size = %d\t\t\tToken Bucket Size = %d\n"
              "\tPeak Bandwidth = %d\t\t\tPeak Bandwidth = %d\n"
              "\tLatency = %d\t\t\t\tLatency = %d\n"
              "\tDelay Variation = %d\t\t\tDelay Variation = %d\n"
              "\tService Type = %d\t\t\tService Type = %d\n"
              "\tMax SDU Size = %d\t\t\tMax SDU Size = %d\n"
              "\tMinimum Policed Size = %d\t\tMinimum Policed Size = %d\n\n",
              ace_get_qos.receiving_flowspec ().token_rate (),
              ace_get_qos.sending_flowspec ().token_rate (),
              ace_get_qos.receiving_flowspec ().token_bucket_size (),
              ace_get_qos.sending_flowspec ().token_bucket_size (),
              ace_get_qos.receiving_flowspec ().peak_bandwidth (),
              ace_get_qos.sending_flowspec ().peak_bandwidth (),
              ace_get_qos.receiving_flowspec ().latency (),
              ace_get_qos.sending_flowspec ().latency (),
              ace_get_qos.receiving_flowspec ().delay_variation (),
              ace_get_qos.sending_flowspec ().delay_variation (),
              ace_get_qos.receiving_flowspec ().service_type (),
              ace_get_qos.sending_flowspec ().service_type (),
              ace_get_qos.receiving_flowspec ().max_sdu_size (),
              ace_get_qos.sending_flowspec ().max_sdu_size (),
              ace_get_qos.receiving_flowspec ().minimum_policed_size (),
              ace_get_qos.sending_flowspec ().minimum_policed_size ()));

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

