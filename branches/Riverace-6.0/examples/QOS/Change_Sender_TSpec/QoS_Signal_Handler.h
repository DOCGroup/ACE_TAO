/* -*- C++ -*- */
// $Id$

// =====================================================================
//
// = LIBRARY
//    ACE_wrappers/examples/QOS
//
// = FILENAME
//    QoS_Signal_Handler.h
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// =====================================================================

#ifndef QOS_SIGNAL_HANDLER_H
#define QOS_SIGNAL_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/QoS/QoS_Session.h"

class QoS_Signal_Handler : public ACE_Event_Handler
{
  // TITLE
  //   This class Handles the SIGINT signal through the Reactor.
  //   Useful to gracefully release QoS sessions.

public:

  QoS_Signal_Handler (ACE_QoS_Session *qos_session);
  // constructor.

  int handle_signal(int signum, siginfo_t*,ucontext_t*);
  // Override this method to implement graceful shutdown.

private:

  ACE_QoS_Session *qos_session_;
  // Session to be gracefully shutdown.

};

#endif /* QOS_SIGNAL_HANDLER_H */

