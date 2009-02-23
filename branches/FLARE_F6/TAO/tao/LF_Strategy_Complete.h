// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Strategy_Complete.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_STRATEGY_COMPLETE_H
#define TAO_LF_STRATEGY_COMPLETE_H

#include /**/ "ace/pre.h"

#include "tao/LF_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @brief A concrete TAO_LF_Strategy for ORB configurations that use
 * the Leader/Followers event loop.
 */
class TAO_Export TAO_LF_Strategy_Complete : public TAO_LF_Strategy
{
public:
  //@{
  /** @name Virtual Methods
   *
   * Please check the documentation in TAO_LF_Strategy
   */
  virtual ~TAO_LF_Strategy_Complete (void);

  virtual void set_upcall_thread (TAO_Leader_Follower &);
  virtual int set_event_loop_thread (ACE_Time_Value *max_wait_time,
                                     TAO_Leader_Follower &);
  virtual void reset_event_loop_thread (int call_reset, TAO_Leader_Follower &);
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"

#endif  /* TAO_LF_STRATEGY_COMPLETE_H */
