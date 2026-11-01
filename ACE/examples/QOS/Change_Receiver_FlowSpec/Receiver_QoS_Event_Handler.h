/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Receiver_QoS_Event_Handler.h
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef RECEIVER_QOS_EVENT_HANDLER_H
#define RECEIVER_QOS_EVENT_HANDLER_H

#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/Event_Handler.h"
#include "ace/QoS/QoS_Session.h"
#include "ace/QoS/SOCK_Dgram_Mcast_QoS.h"

class Receiver_QoS_Event_Handler : public ACE_Event_Handler
{
public:
  /// Constructor.
  Receiver_QoS_Event_Handler () = default;

  /// Constructor.
  Receiver_QoS_Event_Handler (ACE_SOCK_Dgram_Mcast_QoS const &dgram_mcast_qos,
                              ACE_QoS_Session *qos_session);
  Receiver_QoS_Event_Handler (Receiver_QoS_Event_Handler const &) = delete;
  Receiver_QoS_Event_Handler (Receiver_QoS_Event_Handler &&) = delete;
  Receiver_QoS_Event_Handler &operator= (Receiver_QoS_Event_Handler const &) = delete;
  Receiver_QoS_Event_Handler &operator= (Receiver_QoS_Event_Handler &&) = delete;

  /// Destructor.
  ~Receiver_QoS_Event_Handler () override = default;

  /// Override this to return the handle of the Dgram_Mcast
  /// that we are using.
  ACE_HANDLE get_handle () const override;

  /// Handles a READ event.
  int handle_input (ACE_HANDLE fd) override;

  /// Handles a QoS event.
  int handle_qos (ACE_HANDLE fd) override;

private:
  ACE_SOCK_Dgram_Mcast_QoS dgram_mcast_qos_;
  ACE_QoS_Session *qos_session_ {};
  ACE_INET_Addr remote_addr_;
};

#endif  /* RECEIVER_QOS_EVENT_HANDLER_H */


