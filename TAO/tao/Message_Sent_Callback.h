// -*- C++ -*-

//=============================================================================
/**
 *  @file Message_Sent_Callback.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_MESSAGE_SENT_CALLBACK_H
#define TAO_MESSAGE_SENT_CALLBACK_H
#include "ace/pre.h"

#include "corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_Message_Sent_Callback
 *
 * @brief Encapsulate the signaling mechanism used to wake up threads
 *        waiting for a message to be sent out.
 *
 * Please read the documentation in the TAO_Transport class to find
 * out more about the design of the outgoing data path.
 *
 */
class TAO_Export TAO_Message_Sent_Callback
{
public:
  /// Constructor
  TAO_Message_Sent_Callback (void);

  /// Destructor
  virtual ~TAO_Message_Sent_Callback (void);

  /// The message has been successfully sent
  virtual void send_completed (void) = 0;

  /// The message has failed
  virtual void send_failed (void) = 0;

  /// The message has timedout
  virtual void send_timeout (void) = 0;

  /// The connection was closed before the message was sent
  virtual void connection_closed (void) = 0;
};

#if defined (__ACE_INLINE__)
# include "Message_Sent_Callback.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_MESSAGE_SENT_CALLBACK_H */
