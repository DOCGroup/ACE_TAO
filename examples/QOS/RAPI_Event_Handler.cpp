/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    RAPI_Event_Handler.cpp
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#include "RAPI_Event_Handler.h"

// Constructor.
ACE_RAPI_Event_Handler::ACE_RAPI_Event_Handler (void)
{
}

// Constructor.
ACE_RAPI_Event_Handler::ACE_RAPI_Event_Handler (ACE_QoS_Session *qos_session)
  : qos_session_ (qos_session)
{
  
}

// Destructor.
ACE_RAPI_Event_Handler::~ACE_RAPI_Event_Handler (void)
{
}

// Return the handle of the Dgram_Mcast. This method is called
// internally by the reactor.

ACE_HANDLE
ACE_RAPI_Event_Handler::get_handle (void) const
{
  ACE_DEBUG ((LM_DEBUG,
              "\nInside get_handle : returning %d\n",
              this->qos_session_->rsvp_events_handle ()));

  return this->qos_session_->rsvp_events_handle ();
}

int
ACE_RAPI_Event_Handler::handle_qos (ACE_HANDLE)
{

  ACE_DEBUG ((LM_DEBUG,
              "Inside handle_qos ()\n"));

  return 0;
  
}

int
ACE_RAPI_Event_Handler::handle_input (ACE_HANDLE)
{
  ACE_DEBUG ((LM_DEBUG,
              "\nReceived a RAPI event. Inside handle_input ()\n"));
  
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


