// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_CH_Event.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_LF_CH_EVENT_H
#define TAO_LF_CH_EVENT_H

#include /**/ "ace/pre.h"

#include "tao/LF_Event.h"
#include "tao/orbconf.h"
#include "ace/Hash_Map_Manager_T.h"
#include "ace/Null_Mutex.h"
#include "ace/Thread_Mutex.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_LF_Multi_Event;

/**
 * @class TAO_LF_CH_Event
 *
 * @brief Use the Leader/Follower loop to wait for one specific event
 * in the invocation path.
 *
 * Concrete event types and manipulation class which is used for
 * connection handling purposes.
 */
class TAO_Export TAO_LF_CH_Event: public TAO_LF_Event
{
public:
  /**
   * The TAO_LF_Multi_Event class is another specialization of
   * TAO_LF_Event, used for aggregating many connection handlers into
   * a single event object. It requires friendship so that it can
   * check the is_state_final() flag on each of its contained
   * connection handlers.
   */
  friend class TAO_LF_Multi_Event;

  /// Constructor
  TAO_LF_CH_Event (void);

  /// Destructor
  virtual ~TAO_LF_CH_Event (void);

  //@{
  /// Return 1 if the condition was satisfied successfully, 0 if it
  /// has not
  int successful (void) const;

  /// Return 1 if an error was detected while waiting for the
  /// event
  int error_detected (void) const;

protected:

  /// Check whether we have reached the final state..
  virtual int is_state_final (void);

  //@}
private:
  /// Validate and change the state
  /*
   * This concrete class uses the following states declared in the
   * class TAO_LF_Event to transition states
   *
   * LFS_IDLE                  - The event is created, and is in
   *                             initial  state.
   *
   * LFS_CONNECTION_WAIT       - The event is waiting for connection
   *                             completion and it can transition to
   *                             any of the following states, all the
   *                             states are final.
   *
   * LFS_SUCCESS               - The event, connection establishment, has
   *                             completed successfully.
   *
   * LFS_TIMEOUT               - The event has timed out.
   *
   * LFS_CONNECTION_CLOSED     - The connection was closed since
   *                             an error occured while trying to
   *                             establish connection
   *
   *  Event State Diagram
   *  -------------------
   *                              |----> CLOSED
   *                              |        ^
   *                              |        |
   *   IDLE ---> CONNECTION_WAIT--|        |
   *                              |        |
   *                              |        |
   *                              |----> SUCESS
   *
   * Timeouts can occur while waiting for connections.
   *
   */
  virtual void state_changed_i (int new_state);

  /// Set the state irrespective of anything.
  virtual void set_state (int new_state);

  virtual int bind (TAO_LF_Follower *follower);
  virtual int unbind (TAO_LF_Follower *follower);

private:

  /// The previous state that the LF_CH_Event was in
  int prev_state_;

  void validate_state_change (int new_state);

  typedef ACE_Hash_Map_Manager_Ex <TAO_LF_Follower *, int,
                                   ACE_Hash<void *>,
                                   ACE_Equal_To<TAO_LF_Follower *>,
                                   TAO_SYNCH_MUTEX>                  HASH_MAP;
  HASH_MAP followers_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LF_CH_EVENT_H */
