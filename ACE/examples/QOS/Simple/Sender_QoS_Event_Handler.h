/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    Sender_QoS_Event_Handler.h
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


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
  /// Constructor.
  Sender_QoS_Event_Handler ();

  /// Constructor.
  Sender_QoS_Event_Handler (const ACE_SOCK_Dgram_Mcast_QoS &dgram_mcast_qos,
                            ACE_QoS_Session *qos_session);

  /// Destructor.
  ~Sender_QoS_Event_Handler ();

  /// Override this to return the handle of the Dgram_Mcast
  /// that we are using.
  virtual ACE_HANDLE get_handle () const;

  /// Handles a QoS event. Right now, just
  /// prints a message.
  virtual int handle_qos (ACE_HANDLE fd);

private:
  ACE_SOCK_Dgram_Mcast_QoS dgram_mcast_qos_;
  ACE_QoS_Session *qos_session_;
};

#endif  /* SENDER_QOS_EVENT_HANDLER_H */
