// -*- C++ -*-

//=============================================================================
/**
 *  @file   CEC_Pulling_Strategy.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef TAO_CEC_PULLING_STRATEGY_H
#define TAO_CEC_PULLING_STRATEGY_H

#include /**/ "ace/pre.h"

#include "orbsvcs/CosEvent/event_serv_export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/Basic_Types.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_CEC_Pulling_Strategy
 *
 * @brief Define the interface for the pulling strategies.
 *
 * The EC may be configured with different pulling strategies,
 * for instance, it can use a pool of threads to dispatch the
 * events, or a set of queues with threads at different priorities
 * for each queue or can simply push the event to the consumer in
 * FIFO order.
 */
class TAO_Event_Serv_Export TAO_CEC_Pulling_Strategy
{
public:
  /// destructor...
  virtual ~TAO_CEC_Pulling_Strategy (void);

  /// Initialize all the data structures, activate any internal threads,
  /// etc.
  virtual void activate (void) = 0;

  /**
   * Deactivate any internal threads and cleanup internal data
   * structures, it should only return once the threads have finished
   * their jobs.
   */
  virtual void shutdown (void) = 0;

#if 0
  /// Some strategies may want to keep track of connected consumers.
  virtual void connected (TAO_CEC_ProxyPullConsumer *) = 0;
  virtual void reconnected (TAO_CEC_ProxyPullConsumer *) = 0;
  virtual void diconnected (TAO_CEC_ProxyPullConsumer *) = 0;
#endif /* 0 */
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif /* TAO_CEC_PULLING_STRATEGY_H */
