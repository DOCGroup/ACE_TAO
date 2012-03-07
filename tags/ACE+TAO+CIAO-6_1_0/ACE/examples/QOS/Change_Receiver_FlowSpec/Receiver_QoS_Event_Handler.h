/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Receiver_QoS_Event_Handler.h
 *
 *  $Id$
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
  // = Initialization and Termination methods.
  /// Constructor.
  Receiver_QoS_Event_Handler (void);

  /// Constructor.
  Receiver_QoS_Event_Handler (const ACE_SOCK_Dgram_Mcast_QoS &dgram_mcast_qos,
                              ACE_QoS_Session *qos_session);

  /// Destructor.
  ~Receiver_QoS_Event_Handler (void);

  /// Override this to return the handle of the Dgram_Mcast
  /// that we are using.
  virtual ACE_HANDLE get_handle (void) const;

  /// Handles a READ event.
  virtual int handle_input (ACE_HANDLE fd);

  /// Handles a QoS event.
  virtual int handle_qos (ACE_HANDLE fd);

private:
  ACE_SOCK_Dgram_Mcast_QoS dgram_mcast_qos_;
  ACE_QoS_Session *qos_session_;
  ACE_INET_Addr remote_addr_;
};

#endif  /* RECEIVER_QOS_EVENT_HANDLER_H */



