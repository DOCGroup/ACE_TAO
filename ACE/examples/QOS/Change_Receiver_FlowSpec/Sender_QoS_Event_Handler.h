/* -*- C++ -*- */
// $Id$

// ============================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    Sender_QoS_Event_Handler.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef SENDER_QOS_EVENT_HANDLER_H
#define SENDER_QOS_EVENT_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/Reactor.h"
#include "ace/INET_Addr.h"
#include "ace/QoS/SOCK_Dgram_Mcast_QoS.h"
#include "ace/QoS/QoS_Session.h"

//#define MY_DEFPORT 5001
//#define DEFAULT_MULTICASTGROUP "234.5.6.7"

class Sender_QoS_Event_Handler : public ACE_Event_Handler
{
public:
  // = Initialization and Termination methods.
  Sender_QoS_Event_Handler (void);
  // Constructor.

  Sender_QoS_Event_Handler (const ACE_SOCK_Dgram_Mcast_QoS &dgram_mcast_qos,
                            ACE_QoS_Session *qos_session);
  // Constructor.

  ~Sender_QoS_Event_Handler (void);
  // Destructor.

  virtual ACE_HANDLE get_handle (void) const;
  // Override this to return the handle of the Dgram_Mcast
  // that we are using.

  virtual int handle_qos (ACE_HANDLE fd);
  // Handles a QoS event. Right now, just
  // prints a message.

private:

  ACE_SOCK_Dgram_Mcast_QoS dgram_mcast_qos_;
  ACE_QoS_Session *qos_session_;
};

#endif  /* SENDER_QOS_EVENT_HANDLER_H */
