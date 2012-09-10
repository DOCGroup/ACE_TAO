/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Receiver_QoS_Event_Handler.cpp
 *
 *  $Id$
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#include "Receiver_QoS_Event_Handler.h"
#include "ace/Log_Msg.h"
#include "ace/SString.h"
#include "Fill_ACE_QoS.h"

// Constructor.
Receiver_QoS_Event_Handler::Receiver_QoS_Event_Handler (void)
{
}

Receiver_QoS_Event_Handler::Receiver_QoS_Event_Handler (const ACE_SOCK_Dgram_Mcast_QoS
                                                        &dgram_mcast_qos,
                                                        ACE_QoS_Session *qos_session)
  : dgram_mcast_qos_ (dgram_mcast_qos),
    qos_session_ (qos_session)
{
}

// Destructor.
Receiver_QoS_Event_Handler::~Receiver_QoS_Event_Handler (void)
{
}

// Return the handle of the Dgram_Mcast. This method is called
// internally by the reactor.
ACE_HANDLE
Receiver_QoS_Event_Handler::get_handle (void) const
{
  return this->dgram_mcast_qos_.get_handle ();
}

// Called when there is a READ activity on the dgram_mcast_qos handle.
int
Receiver_QoS_Event_Handler::handle_input (ACE_HANDLE)
{
  char buf[BUFSIZ];

  iovec iov;
  iov.iov_base = buf;
  iov.iov_len = BUFSIZ;

  ACE_OS::memset (iov.iov_base,
                  0,
                  BUFSIZ);

  ACE_DEBUG ((LM_DEBUG,
              "Inside handle_input () of Receiver_QoS_Event_Handler ()\n"));

  // Receive message from multicast group.
  ssize_t result =
    this->dgram_mcast_qos_.recv (&iov,
                                 1,
                                 this->remote_addr_);

  if (result != -1)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Message Received : %s",
                  iov.iov_base));
      return 0;
    }
  else
    return -1;
}

// Called when there is a QoS Event.
int
Receiver_QoS_Event_Handler::handle_qos (ACE_HANDLE fd)
{
  ACE_UNUSED_ARG (fd);

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


return 0;

}






