// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Event.h
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_EVENT_H
#define TAO_LF_EVENT_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LF_Follower;
class TAO_Leader_Follower;

/**
 * @class TAO_LF_Event
 *
 * @brief Use the Leader/Follower loop to wait for one specific event.
 *
 * The Leader/Follower event loop is used to wait for incoming
 * responses, as well as to wait for all the data to be flushed.
 * This class encapsulates this event loop. It uses Template Method to
 * parametrize the 'waited for' predicate (i.e. reply received or
 * message sent or connection establishment etc.)
 *
 * @todo Implementing the Leader/Followers loop in this class, as
 * well as the callbacks to communicate that an event has completed
 * leads to excessive coupling.  A better design would use a separate
 * class to signal the events, that would allow us to remove the
 * Leader/Followers logic from the ORB.  However, that requires other
 * major changes and it somewhat complicates the design.
 */
class TAO_Export TAO_LF_Event
{
public:

  friend class TAO_Leader_Follower;

  /// Constructor
  TAO_LF_Event (void);

  /// Destructor
  virtual ~TAO_LF_Event (void);

  /// Bind a follower
  /**
   * An event can be waited on by at most one follower thread, this
   * method is used to bind the waiting thread to the event, in order
   * to let the event signal any important state changes.
   *
   * This is virtual to allow the LF_Multi_Event derived type share
   * the follower with all the subordinate LF_CH_Events.
   *
   * @return -1 if the LF_Event is already bound, 0 otherwise
   */
  virtual int bind (TAO_LF_Follower *follower);

  /// Unbind the follower
  virtual int unbind (TAO_LF_Follower *follower);

  //@{
  /** @name State management
   *
   * A Leader/Followers event goes through several states during its
   * lifetime. We use an enum to represent those states and state
   * changes are validated according to the rules defined in the
   * concrete classes. We treat the states as finite states in a
   * FSM. The possible sequence of states through which the FSM
   * migrates is defined in the concrete classes.
   */
  enum LFS_STATE {
    /// The event is created, and is in initial state
    LFS_IDLE = 0,
    /// The event is active
    LFS_ACTIVE,
    /// The event is waiting for connection completion.
    LFS_CONNECTION_WAIT,
    /// The event has completed successfully
    LFS_SUCCESS,
    /// A failure has been detected while the event was active
    LFS_FAILURE,
    /// The event has timed out
    LFS_TIMEOUT,
    /// The connection was closed.
    LFS_CONNECTION_CLOSED
  };

  /// Accessor to change the state. The state isn't changed unless
  /// certain conditions are satisfied.
  void state_changed (LFS_STATE new_state, TAO_Leader_Follower &lf);

  /// Return true if the condition was satisfied successfully, false if it
  /// has not
  bool successful (TAO_Leader_Follower &lf) const;

  /// Return true if an error was detected while waiting for the
  /// event
  bool error_detected (TAO_Leader_Follower &lf) const;

  /// Check if we should keep waiting.
  bool keep_waiting (TAO_Leader_Follower &lf) const;
  //@}

  /// Reset the state, irrespective of the previous states
  void reset_state (LFS_STATE new_state);

  static const char *state_name (LFS_STATE st);

protected:

  /// Validate the state change
  virtual void state_changed_i (LFS_STATE new_state) = 0;

  /// Check if we should keep waiting.
  bool keep_waiting_i (void) const;

  /// Return true if the condition was satisfied successfully, false if it
  /// has not
  virtual bool successful_i (void) const = 0 ;

  /// Return true if an error was detected while waiting for the
  /// event
  virtual bool error_detected_i (void) const = 0;

  /// Check whether we have reached the final state..
  virtual bool is_state_final (void) const = 0;

private:

  /// Set the state irrespective of anything.
  virtual void set_state (LFS_STATE new_state);

protected:
  /// The current state
  LFS_STATE state_;

  /// The bounded follower
  TAO_LF_Follower *follower_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#if defined (__ACE_INLINE__)
# include "tao/LF_Event.inl"
#endif /* __ACE_INLINE__ */

#include /**/ "ace/post.h"

#endif  /* TAO_LF_EVENT_H */
