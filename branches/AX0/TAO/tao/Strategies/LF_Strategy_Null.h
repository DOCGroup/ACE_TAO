// -*- C++ -*-

//=============================================================================
/**
 *  @file LF_Strategy_Null.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan <coryan@uci.edu>
 */
//=============================================================================

#ifndef TAO_LF_STRATEGY_NULL_H
#define TAO_LF_STRATEGY_NULL_H
#include "ace/pre.h"

#include "strategies_export.h"
#include "tao/LF_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @brief A concrete TAO_LF_Strategy for ORB configurations that do
 * not use the Leader/Followers event loop.
 */
class TAO_Strategies_Export TAO_LF_Strategy_Null : public TAO_LF_Strategy
{
public:
  /// Constructor
  TAO_LF_Strategy_Null (void);

  //@{
  /** @name Virtual Methods
   *
   * Please check the documentation in TAO_LF_Strategy
   */
  virtual ~TAO_LF_Strategy_Null (void);

  virtual void set_upcall_thread (TAO_Leader_Follower &);
  virtual int set_event_loop_thread (ACE_Time_Value *max_wait_time,
                                     TAO_Leader_Follower &);
  virtual void reset_event_loop_thread (int call_reset,
                                        TAO_Leader_Follower &);
};

#if defined (__ACE_INLINE__)
# include "LF_Strategy_Null.inl"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif  /* TAO_LF_STRATEGY_NULL_H */
