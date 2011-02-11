/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    Sender_QoS_Event_Handler.cpp
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#include "Sender_QoS_Event_Handler.h"
#include "ace/Log_Msg.h"

#include "Fill_ACE_QoS.h"

// Constructor.
Sender_QoS_Event_Handler::Sender_QoS_Event_Handler (void)
{
}

// Constructor.
Sender_QoS_Event_Handler::Sender_QoS_Event_Handler (const ACE_SOCK_Dgram_Mcast_QoS
                                                    &dgram_mcast_qos,
                                                    ACE_QoS_Session *qos_session)
  : dgram_mcast_qos_ (dgram_mcast_qos),
    qos_session_ (qos_session)
{
}

// Destructor.
Sender_QoS_Event_Handler::~Sender_QoS_Event_Handler (void)
{
}

// Return the handle of the Dgram_Mcast. This method is called
// internally by the reactor.

ACE_HANDLE
Sender_QoS_Event_Handler::get_handle (void) const
{
  return this->dgram_mcast_qos_.get_handle ();
}

// Handle the QoS Event. In this case send data to the receiver
// using WSASendTo() that uses overlapped I/O.

int
Sender_QoS_Event_Handler::handle_qos (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nReceived a QOS event. Inside handle_qos ()\n"));

  // We have received an RSVP event. The following update_qos () call
  // calls rapi_dispatch () in case of RAPI and WSAIoctl (GET_QOS) in
  // case of W2K. It then does the QoS parameter translation and updates
  // the QoS session object with the latest QoS. This call replaces the
  // direct call that was being made to WSAIoctl (GET_QOS) here for the
  // Win2K example.

  if (this->qos_session_->update_qos () == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in updating QoS\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                " Updating QOS succeeds.\n"));

  // Now proactively query the QoS object for QoS.
  ACE_QoS ace_get_qos = this->qos_session_->qos ();

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
              ace_get_qos.receiving_flowspec ()->token_rate (),
              ace_get_qos.sending_flowspec ()->token_rate (),
              ace_get_qos.receiving_flowspec ()->token_bucket_size (),
              ace_get_qos.sending_flowspec ()->token_bucket_size (),
              ace_get_qos.receiving_flowspec ()->peak_bandwidth (),
              ace_get_qos.sending_flowspec ()->peak_bandwidth (),
              ace_get_qos.receiving_flowspec ()->latency (),
              ace_get_qos.sending_flowspec ()->latency (),
              ace_get_qos.receiving_flowspec ()->delay_variation (),
              ace_get_qos.sending_flowspec ()->delay_variation (),
              ace_get_qos.receiving_flowspec ()->service_type (),
              ace_get_qos.sending_flowspec ()->service_type (),
              ace_get_qos.receiving_flowspec ()->max_sdu_size (),
              ace_get_qos.sending_flowspec ()->max_sdu_size (),
              ace_get_qos.receiving_flowspec ()->minimum_policed_size (),
              ace_get_qos.sending_flowspec ()->minimum_policed_size ()));

  // This is SPECIFIC TO WIN2K and should be done in the qos_update function.

//    ACE_QoS ace_get_qos;
//    u_long dwBytes;

//    if (ACE_OS::ioctl (this->dgram_mcast_qos_.get_handle (),
//                       ACE_SIO_GET_QOS,
//                       ace_get_qos,
//                       &dwBytes) == -1)
//      ACE_ERROR ((LM_ERROR,
//                  "Error in Qos get ACE_OS::ioctl ()\n"
//                  "Bytes Returned = %d\n",
//                  dwBytes));
//    else
//      ACE_DEBUG ((LM_DEBUG,
//                  "Getting QOS using ACE_OS::ioctl () succeeds.\n"));

  const char* msg = "Hello sent on a QoS enabled session !!\n";
  iovec iov[1];
  iov[0].iov_base = const_cast<char *>(msg);
  iov[0].iov_len = ACE_OS::strlen(msg);

  size_t bytes_sent = 0;

  // Send "Hello" to the QoS session address to which the receiver has
  // subscribed.
  if (this->dgram_mcast_qos_.send (iov,
                                   1,
                                   bytes_sent,
                                   0,
                                   this->qos_session_->dest_addr (),
                                   0,
                                   0) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       "Error in dgram_mcast.send ()\n"),
                      -1);
  else
    ACE_DEBUG ((LM_DEBUG,
                "Using ACE_OS::sendto () : Bytes sent : %d",
                bytes_sent));

  return 0;
}
