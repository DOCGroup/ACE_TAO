/* -*- C++ -*- */
/**
 *  @file   EC_Reactive_Timeout_Generator.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 *
 * Based on previous work by Tim Harrison (harrison@cs.wustl.edu) and
 * other members of the DOC group. More details can be found in:
 *
 * http://doc.ece.uci.edu/~coryan/EC/index.html
 */

#ifndef TAO_EC_REACTIVE_TIMEOUT_GENERATOR_H
#define TAO_EC_REACTIVE_TIMEOUT_GENERATOR_H
#include "ace/pre.h"

#include "EC_Timeout_Generator.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class TAO_EC_Reactive_Timeout_Generator
 *
 * @brief A simple implementation of the Timeout_Generator based on the
 * Reactor.
 *
 * Implements the Timeout_Generator using an ACE_Reactor. Usually
 * the same reactor that is used by the ORB where the EC runs.
 */
class TAO_RTEvent_Export TAO_EC_Reactive_Timeout_Generator : public TAO_EC_Timeout_Generator
{
public:
  /// Constructor.
  /// If <reactor> is 0 then we use the reactor in the ORB singleton.
  TAO_EC_Reactive_Timeout_Generator (ACE_Reactor *reactor = 0);

  /// destructor
  virtual ~TAO_EC_Reactive_Timeout_Generator (void);

  // = The TAO_EC_Timeout_Generator methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual int schedule_timer (TAO_EC_Timeout_Filter* filter,
                              const ACE_Time_Value& delta,
                              const ACE_Time_Value& interval);
  virtual int cancel_timer (const TAO_EC_QOS_Info& info,
                            int id);

private:
  /// The reactor
  ACE_Reactor *reactor_;

  /// An adapter to receive the timeout events.
  TAO_EC_Timeout_Adapter event_handler_;
};

#if defined (__ACE_INLINE__)
#include "EC_Reactive_Timeout_Generator.i"
#endif /* __ACE_INLINE__ */

#include "ace/post.h"
#endif /* TAO_EC_REACTIVE_TIMEOUT_GENERATOR_H */
