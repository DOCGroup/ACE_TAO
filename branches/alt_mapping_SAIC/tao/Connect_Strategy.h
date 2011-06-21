// -*- C++ -*-

//=============================================================================
/**
 *  @file Connect_Strategy.h
 *
 *  $Id$
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================

#ifndef TAO_CONNECT_STRATEGY_H
#define TAO_CONNECT_STRATEGY_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include /**/ "tao/Versioned_Namespace.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Synch_Options;
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_ORB_Core;
class TAO_Connector;
class TAO_Connection_Handler;
class TAO_Transport;
class TAO_LF_Multi_Event;
class TAO_LF_Event;

/**
 * @class TAO_Connect_Strategy
 *
 * @brief Define the interface for the connect strategy, i.e. the
 *  algorithm that controls how does the ORB establishes remote
 *  connections.
 *
 * The thread that establishes remote connections can either make a
 * blocking or a non-blocking connect. The strategy to wait for the
 * connection completion can also be different.
 *
 * This strategy controls how does the ORB schedules and waits for
 * connection completion.
 */

class TAO_Export TAO_Connect_Strategy
{
public:

  /// Constructor
  TAO_Connect_Strategy (TAO_ORB_Core *orb);

  /// Destructor
  virtual ~TAO_Connect_Strategy (void);

  /// Obtain the synch options that can be passed to ACE_Connector
  /**
   * Return the synch option for the connector, based on the timeout
   * and the strategy in place. ACE_Connectors behavior can be altered
   * by passing the right ACE_Synch_Options to the connect () call.
   */
  virtual void synch_options (ACE_Time_Value *val,
                              ACE_Synch_Options &opt) = 0;

  /// Wait for the connection to be completed till a timeout occurs.
  /**
   * If the connection establishment fails the state within the
   * connection handler is set appropriately.
   */
  int wait (TAO_Connection_Handler *ch, ACE_Time_Value *val);

  int wait (TAO_Transport *t, ACE_Time_Value *val);

  /// Wait for one of many connections to complete. Returns when one
  /// succeeds or all fail.
  int wait (TAO_LF_Multi_Event *ev, ACE_Time_Value *val);

  /// Do a quick check to see if any connections are complete. This
  /// does the same as the wait with an explicit time value of 0.
  int poll (TAO_LF_Multi_Event *ev);

protected:
  /// This is the method that does all the real interesting stuff.
  virtual int wait_i (TAO_LF_Event *ev,
                      TAO_Transport *t,
                      ACE_Time_Value *val) = 0;

  /// Cached copy of the ORB core pointer
  TAO_ORB_Core * const orb_core_;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /*TAO_CONNECT_STRATEGY_H*/
