// This may look like C, but it's really -*- C++ -*-

//=============================================================================
/**
 *  @file    Wait_Strategy.h
 *
 *  $Id$
 *
 *  @author  Alexander Babu Arulanthu <alex@cs.wustl.edu>
 */
//=============================================================================


#ifndef TAO_WAIT_STRATEGY_H
#define TAO_WAIT_STRATEGY_H
#include "ace/pre.h"

#include "tao/corbafwd.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class ACE_Time_Value;
class TAO_ORB_Core;
class TAO_Transport;
class TAO_Synch_Reply_Dispatcher;

/**
 * @class TAO_Wait_Strategy
 *
 * @brief Strategy for waiting for the reply.
 *
 */
class TAO_Export TAO_Wait_Strategy
{

public:
  /// Constructor.
  TAO_Wait_Strategy (TAO_Transport *transport);

  /// Destructor.
  virtual ~TAO_Wait_Strategy (void);

  /**
   * The user is going to send a request, prepare any internal
   * variables because the reply may arrive *before* the user calls
   * wait.
   */
  virtual int sending_request (TAO_ORB_Core *orb_core,
                               int two_way);

  /// Base class virtual method. Wait till the <reply_received> flag is
  /// true or the time expires.
  virtual int wait (ACE_Time_Value *max_wait_time,
                    TAO_Synch_Reply_Dispatcher &rd) = 0;

  /// Register the handler needs with the reactor provided that it makes
  /// sense for the  strategy.
  virtual int register_handler (void) = 0;

  /// Returns a value to indicate whether the transport needs to set
  /// the socket on which it is waiting to non-blocking mode or not.
  virtual int non_blocking (void) = 0;

  /// Get/Set method for the flag
  int is_registered (void);
  void is_registered (int flag);

protected:
  /// Transport object.
  TAO_Transport *transport_;

  /// Flag to indicate whether the service handler that created the
  /// above transport has been registered with the reactor or not. The
  /// default is 0.
  int is_registered_;
};


#if defined (__ACE_INLINE__)
# include "Wait_Strategy.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_WAIT_STRATEGY_H */
