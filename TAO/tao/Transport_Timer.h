// -*- C++ -*-

//=============================================================================
/**
 *  @file Transport_Timer.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_TRANSPORT_TIMER_H
#define TAO_TRANSPORT_TIMER_H
#include "ace/pre.h"

#include "TAO_Export.h"
#include "ace/Event_Handler.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_Transport;

/**
 * @class TAO_Transport_Timer
 *
 * @brief Allows TAO_Transport instances to receive timeout
 *        notifications from the Reactor.  In other words, implements
 *        the Adapter Role, of the Adapter Pattern, where the Adaptee
 *        is a TAO_Transport and the client is the Reactor.
 *
 */
class TAO_Export TAO_Transport_Timer : public ACE_Event_Handler
{
public:
  /// Constructor
  /**
   * @param transport The adaptee
   */
  TAO_Transport_Timer (TAO_Transport *transport);

  /// Receive timeout events from the Reactor and forward them to the
  /// TAO_Transport
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act);
private:
  /// The Adaptee
  TAO_Transport *transport_;
};

#include "ace/post.h"
#endif  /* TAO_TRANSPORT_TIMER_H */
