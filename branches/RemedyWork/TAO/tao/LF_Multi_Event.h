// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Multi_Event.h
 *
 *  $Id$
 *
 *  @author Phil Mesnier <mesnier_p@ociweb.com>
 */
//=============================================================================

#ifndef TAO_LF_MULTI_EVENT_H
#define TAO_LF_MULTI_EVENT_H

#include /**/ "ace/pre.h"

#include "tao/LF_Event.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

// Forward decls
class TAO_Connection_Handler;
class TAO_Transport;

/**
 * @class TAO_LF_Multi_Event
 *
 * @brief Use the Leader/Follower loop to wait for one specific event
 * in the invocation path.
 *
 * Used by the parallel connection strategy for waiting on one of many
 * connections.
 */
class TAO_Export TAO_LF_Multi_Event: public TAO_LF_Event
{
public:
  /// Constructor
  TAO_LF_Multi_Event (void);

  /// Destructor
  virtual ~TAO_LF_Multi_Event (void);

  /// Propagate the follower to all the events in the collection.
  virtual int bind (TAO_LF_Follower *follower);

  /// Unbind the follower from all the collected events.
  virtual int unbind (TAO_LF_Follower *follower);

  /// Adds a handler to the collection
  void add_event (TAO_Connection_Handler *ch);

  /// Returns the connection handler that caused the successful status
  /// to be returned.
  TAO_Connection_Handler *winner(void);

  /// Returns the transport associated with the first entry in the collection.
  TAO_Transport *base_transport(void);

  //@{
  /// Return 1 if the condition was satisfied successfully, 0 if it
  /// has not - This iterates over the list of attached events and
  /// returns 1 if any of them return 1 from successful.
  int successful (void) const;

  /// Return 1 if an error was detected while waiting for the
  /// event - This iterates over the list of events and returns
  /// 1 only if all of them return 1 from error_detected().
  int error_detected (void) const;

  //@}
private:
  void operator= (const TAO_LF_Multi_Event &);
  TAO_LF_Multi_Event (const TAO_LF_Multi_Event &);

protected:

  /// Validate the state change
  virtual void state_changed_i (int new_state);

  /// Check whether we have reached the final state..
  virtual int is_state_final (void);

private:

  struct Event_Node {
    TAO_Connection_Handler * ptr_;
    Event_Node *next_;
  };

  struct Event_Node *events_;

  mutable TAO_Connection_Handler * winner_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LF_Multi_EVENT_H */
