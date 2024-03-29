// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Strategy_Null.h
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_STRATEGY_NULL_H
#define TAO_LF_STRATEGY_NULL_H
#include /**/ "ace/pre.h"

#include "tao/Strategies/strategies_export.h"
#include "tao/LF_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class TAO_LF_Strategy_Null
 *
 * @brief A concrete TAO_LF_Strategy for ORB configurations that do
 * not use the Leader/Followers event loop.
 */
class TAO_Strategies_Export TAO_LF_Strategy_Null : public TAO_LF_Strategy
{
public:
  /// Constructor
  TAO_LF_Strategy_Null () = default;

  //@{
  /** @name Virtual Methods
   *
   * Please check the documentation in TAO_LF_Strategy
   */
  ~TAO_LF_Strategy_Null () override = default;

  void set_upcall_thread (TAO_Leader_Follower &) override;
  int set_event_loop_thread (ACE_Time_Value *max_wait_time,
                             TAO_Leader_Follower &) override;
  void reset_event_loop_thread (int call_reset,
                                TAO_Leader_Follower &) override;
  //@}
};

TAO_END_VERSIONED_NAMESPACE_DECL

#include /**/ "ace/post.h"
#endif  /* TAO_LF_STRATEGY_NULL_H */
