/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    Receiver_QOS_Event_Handler.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef RECEIVER_QOS_EVENT_HANDLER_H
#define RECEIVER_QOS_EVENT_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/SOCK_Dgram_Mcast.h"
#include "ace/QoS_Session.h"

ACE_RCSID(Receiver_QOS_Event_Handler, Receiver_QOS_Event_Handler, "$Id$")

class ACE_QOS_Event_Handler : public ACE_Event_Handler
{
public:
  // = Initialization and Termination methods.
  ACE_QOS_Event_Handler (void);
  // Constructor.

  ACE_QOS_Event_Handler::ACE_QOS_Event_Handler (const ACE_SOCK_Dgram_Mcast &dgram_mcast,
												ACE_QoS_Session *qos_session);
  // Constructor.

  ~ACE_QOS_Event_Handler (void);
  // Destructor. 

  virtual ACE_HANDLE get_handle (void) const;
  // Override this to return the handle of the Dgram_Mcast
  // that we are using.

  virtual int handle_qos (ACE_HANDLE fd);
  // Handles a QOS event. Right now, just 
  // prints a message.

  virtual int handle_input (ACE_HANDLE fd);
  // Handles a READ event.

private:
  ACE_SOCK_Dgram_Mcast dgram_mcast_;
  ACE_INET_Addr remote_addr_;
  ACE_QoS_Session *qos_session_;
};

#endif  /* RECEIVER_QOS_EVENT_HANDLER_H */
