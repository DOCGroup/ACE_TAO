// -*- C++ -*-

//=============================================================================
/**
 *  @file Flushing_Strategy.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_FLUSHING_STRATEGY_H
#define TAO_FLUSHING_STRATEGY_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Transport;
class TAO_Queued_Message;

/**
 * @class TAO_Flushing_Strategy
 *
 * @brief Define the interface for the flushing strategy, i.e. the
 *        algorithm that controls how does the ORB flush outgoing
 *        data.
 *
 * Please read the documentation in the TAO_Transport class to find
 * out more about the design of the outgoing data path.
 *
 * Some applications can block the current thread whenever they are
 * sending out data.  In those cases they can obtain better
 * performance by blocking in calls to write() than by participating
 * in the Leader/Followers protocol to shared the ORB Reactor.
 *
 * This strategy controls how does the ORB schedule and cancel
 * reactive I/O, if there is no reactive I/O the strategy is just a
 * no-op.
 *
 */
class TAO_Export TAO_Flushing_Strategy
{
public:
  /// Destructor
  virtual ~TAO_Flushing_Strategy (void);

  /// Schedule the transport argument to be flushed
  virtual int schedule_output (TAO_Transport *transport) = 0;

  /// Cancel all scheduled output for the transport argument
  virtual int cancel_output (TAO_Transport *transport) = 0;

  /// Wait until msg is sent out.  Potentially other messages are
  /// flushed too, for example, because there are ahead in the queue.
  virtual int flush_message (TAO_Transport *transport,
                             TAO_Queued_Message *msg,
                             ACE_Time_Value *max_wait_time) = 0;

  /// Wait until the transport has no messages queued.
  virtual int flush_transport (TAO_Transport *transport) = 0;
};

#include "ace/post.h"
#endif  /* TAO_FLUSHING_STRATEGY_H */
