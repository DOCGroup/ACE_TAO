// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file   Asynch_Timeout_Handler.h
 *
 *  $Id$
 *
 *  Timeout Handler for AMI timeouts
 *
 *
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================


#ifndef TAO_ASYNCH_TIMEOUT_HANDLER_H
#define TAO_ASYNCH_TIMEOUT_HANDLER_H
#include "ace/pre.h"

#include "tao/orbconf.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
#  pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#if (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1)

#include "ace/Event_Handler.h"
#include "tao/TAO_Export.h"
#include "tao/corbafwd.h"

class TAO_Asynch_Reply_Dispatcher_Base;
class TAO_Transport_Mux_Strategy;


class TAO_Export TAO_Asynch_Timeout_Handler
  : public ACE_Event_Handler
{
  // = TITLE
  //    TAO_Asynch_Timeout_Handler
  //
  // = DESCRIPTION
  //    Handler registered with the reactor in case of AMI
  //    timeouts.
  //
public:
  TAO_Asynch_Timeout_Handler (TAO_Asynch_Reply_Dispatcher_Base *rd,
                              ACE_Reactor *reactor);

  ~TAO_Asynch_Timeout_Handler ();

  /// Schedule a timer
  long schedule_timer (TAO_Transport_Mux_Strategy *tms,
                       CORBA::ULong request_id,
                       const ACE_Time_Value &max_wait_time);

  /// Invoked by the reactor on timeout
  virtual int handle_timeout (const ACE_Time_Value &current_time,
                              const void *act);

  /// Cancel this timer, remove it from the reactor
  virtual void cancel ();

public:
  /// The reply dispatcher expecting the reply
  TAO_Asynch_Reply_Dispatcher_Base *rd_;

  /// The transport mux strategy dispatching the reply
  TAO_Transport_Mux_Strategy *tms_;

  /// Remember the ID of the request.
  CORBA::ULong request_id_;

  /// Our reactor
  ACE_Reactor *reactor_;
};

#endif /* (TAO_HAS_AMI_CALLBACK == 1) || (TAO_HAS_AMI_POLLER == 1) == 0 */

#include "ace/post.h"
#endif /* TAO_ASYNCH_TIMEOUT_HANDLER_H */
