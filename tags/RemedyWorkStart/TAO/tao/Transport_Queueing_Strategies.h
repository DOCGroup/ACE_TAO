// -*- C++ -*-

//=============================================================================
/**
 *  @file    Transport_Queueing_Strategies.h
 *
 *  $Id$
 *
 *  Queueing strategies for the ORB Messaging layer.
 *
 *  @author  Irfan Pyarali
 */
//=============================================================================


#ifndef TAO_TRANSPORT_QUEUEING_STRATEGIES_H
#define TAO_TRANSPORT_QUEUEING_STRATEGIES_H

#include /**/ "ace/pre.h"

#include /**/ "tao/TAO_Export.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "tao/orbconf.h"
#include "tao/Basic_Types.h"

ACE_BEGIN_VERSIONED_NAMESPACE_DECL
class ACE_Time_Value;
ACE_END_VERSIONED_NAMESPACE_DECL

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_Stub;

namespace TAO
{
  /// Define the interface for the Queueing Strategy
  /**
   * The low-level I/O components in the ORB use this strategy to
   * determine when messages must be queued, immediately sent or
   * flushed.
   *
   * The strategy isolates this low-level components from the higher
   * level strategies used by the application developer.
   */
  class TAO_Export Transport_Queueing_Strategy
  {
  public:
    /// Destructor
    virtual ~Transport_Queueing_Strategy (void);

    /// Return true if a message must be queued
    virtual bool must_queue (bool queue_empty) const = 0;

    /// Return true if it is time to start
    /**
     * @param stub The object used to make the request, this is used to
     *        obtain the policies currently in effect for the request
     * @param msg_count The number of messages currently queued
     * @param total_bytes Number of bytes currently queued
     * @param set_timer Returns true if a timer should be set to drain the
     *        queue
     * @param interval If set_timer returns 1, this parameter contains
     *        the timer interval
     * @param must_flush Is set to true if things must be flushed at this
     *        moment
     */
    virtual bool buffering_constraints_reached (
      TAO_Stub *stub,
      size_t msg_count,
      size_t total_bytes,
      bool &must_flush,
      const ACE_Time_Value &current_deadline,
      bool &set_timer,
      ACE_Time_Value &interval) const = 0;
  };

  /**
   * This strategy doesn't not queue by default, but when a message is queued
   * we always flush it to the transport. This is used for oneways with
   * SYNC_WITH_TRANSPORT, SYNC_WITH_SERVER and SYNC_WITH_TARGET
   */
  class Flush_Transport_Queueing_Strategy : public Transport_Queueing_Strategy
  {
  public:
    virtual bool must_queue (bool queue_empty) const;

    virtual bool buffering_constraints_reached (
      TAO_Stub *stub,
      size_t msg_count,
      size_t total_bytes,
      bool &must_flush,
      const ACE_Time_Value &current_deadline,
      bool &set_timer,
      ACE_Time_Value &interval) const;
  };
}

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif /* TAO_TRANSPORT_QUEUEING_STRATEGY_H */
