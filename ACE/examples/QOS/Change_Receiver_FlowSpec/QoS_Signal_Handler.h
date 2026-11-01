/* -*- C++ -*- */

//=============================================================================
/**
 *  @file    QoS_Signal_Handler.h
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
 *
 * This class Handles the SIGINT signal through the Reactor.
 * Useful to gracefully release QoS sessions.
 */
class QoS_Signal_Handler : public ACE_Event_Handler
{
public:
  /// constructor.
  explicit QoS_Signal_Handler (ACE_QoS_Session *qos_session);
  QoS_Signal_Handler (QoS_Signal_Handler const &) = delete;
  QoS_Signal_Handler (QoS_Signal_Handler &&) = delete;
  QoS_Signal_Handler &operator= (QoS_Signal_Handler const &) = delete;
  QoS_Signal_Handler &operator= (QoS_Signal_Handler &&) = delete;

  /// Override this method to implement graceful shutdown.
  int handle_signal (int signum, siginfo_t *, ucontext_t *) override;

private:
  /// Session to be gracefully shutdown.
  ACE_QoS_Session *qos_session_ {};
};

#endif /* QOS_SIGNAL_HANDLER_H */

