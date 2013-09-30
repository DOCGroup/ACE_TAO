/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    QoS_Signal_Handler.h
 *
 *  $Id$
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef QOS_SIGNAL_HANDLER_H
#define QOS_SIGNAL_HANDLER_H

#include "ace/Event_Handler.h"
#include "ace/QoS/QoS_Session.h"

/**
 * @class QoS_Signal_Handler
 TITLE
 * This class Handles the SIGINT signal through the Reactor.
 * Useful to gracefully release QoS sessions.
 */
class QoS_Signal_Handler : public ACE_Event_Handler
{

public:

  /// constructor.
  QoS_Signal_Handler (ACE_QoS_Session *qos_session);

  /// Override this method to implement graceful shutdown.
  int handle_signal(int signum, siginfo_t*,ucontext_t*);

private:

  /// Session to be gracefully shutdown.
  ACE_QoS_Session *qos_session_;

};

#endif /* QOS_SIGNAL_HANDLER_H */

