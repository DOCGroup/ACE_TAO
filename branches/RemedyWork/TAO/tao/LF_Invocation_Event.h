// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Invocation_Event.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_INVOCATION_EVENT_H
#define TAO_LF_INVOCATION_EVENT_H

#include /**/ "ace/pre.h"

#include "tao/LF_Event.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LF_Invocation_Event
 *
 * @brief Use the Leader/Follower loop to wait for one specific event
 * in the invocation path.
 *
 * Concrete event types and manipulation class through which the
 * invocation data path would flow. Typically state changes of
 * interest include whether a message has arrived, or timedout waiting
 * for a message or if the connection is closed waiting for a
 * message. Details of the states are documented within the class.
 *
 */
class TAO_Export TAO_LF_Invocation_Event: public TAO_LF_Event
{
public:
  /// Constructor
  TAO_LF_Invocation_Event (void);

  /// Destructor
  virtual ~TAO_LF_Invocation_Event (void);

  /// Return 1 if the condition was satisfied successfully, 0 if it
  /// has not
  int successful (void) const;

  /// Return 1 if an error was detected while waiting for the
  /// event
  int error_detected (void) const;
  //@}

protected:

  /// Validate and perform the state change
  /**
   * This concrete class uses the following states declared in the
   * class TAO_LF_Event
   *
   * LFS_IDLE    - The event is created, and is in initial state.
   * LFS_ACTIVE  - The event is active and it can transition to any of
   *               the following states, all the states are final.
   * LFS_SUCCESS - The event has completed successfully.
   * LFS_FAILURE - A failure has been detected while the event was
   *               active.
   * LFS_TIMEOUT - The event has timed out.
   * LFS_CONNECTION_CLOSED - The connection was closed when the state
   *                         was active.
   */
  virtual void state_changed_i (int new_state);

private:
  /// Check whether we have reached the final state..
  int is_state_final (void);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LF_INVOCATION_EVENT_H */
