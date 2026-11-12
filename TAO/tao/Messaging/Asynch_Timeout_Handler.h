// -*- C++ -*-

/**
 *  @file   Asynch_Timeout_Handler.h
 *
 *  Timeout Handler for AMI timeouts
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */

#ifndef TAO_ASYNCH_TIMEOUT_HANDLER_H
#define TAO_ASYNCH_TIMEOUT_HANDLER_H

#include /**/ "ace/pre.h"

#include "tao/Messaging/messaging_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Basic_Types.h"

#include "ace/Event_Handler.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Transport_Mux_Strategy;

/**
 * @class TAO_Asynch_Timeout_Handler
 *
 * @brief Handler registered with the reactor in case of AMI timeouts.
 */
class TAO_Asynch_Timeout_Handler
  : public ACE_Event_Handler
{
public:
  explicit TAO_Asynch_Timeout_Handler (ACE_Reactor *reactor);

  ~TAO_Asynch_Timeout_Handler () override = default;

  TAO_Asynch_Timeout_Handler (TAO_Asynch_Timeout_Handler const &) = delete;
  TAO_Asynch_Timeout_Handler &operator= (TAO_Asynch_Timeout_Handler const &) = delete;
  TAO_Asynch_Timeout_Handler (TAO_Asynch_Timeout_Handler &&) = delete;
  TAO_Asynch_Timeout_Handler &operator= (TAO_Asynch_Timeout_Handler &&) = delete;

  /// Schedule a timer
  long schedule_timer (TAO_Transport_Mux_Strategy *tms,
                       CORBA::ULong request_id,
                       ACE_Time_Value const &max_wait_time);

  /// Invoked by the reactor on timeout
  int handle_timeout (ACE_Time_Value const &current_time,
                      void const *act) override;

  /// Cancel this timer, remove it from the reactor
  virtual void cancel ();

  /// Borrowed while the timeout is scheduled.
  TAO_Transport_Mux_Strategy *tms_ {};

  /// Remember the ID of the request.
  CORBA::ULong request_id_ {};

  /// Non-owning reactor pointer; the reactor outlives this handler.
  ACE_Reactor *reactor_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_ASYNCH_TIMEOUT_HANDLER_H */
