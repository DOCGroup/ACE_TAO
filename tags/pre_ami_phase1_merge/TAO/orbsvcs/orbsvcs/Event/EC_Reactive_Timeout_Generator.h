/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel
//
// = FILENAME
//   EC_Reactive_Timeout_Generator
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// = DESCRIPTION
//   A new implementation of the Real Time Event Services.
//   Based on previous work by Tim Harrison (harrison@cs.wustl.edu)
//   and other members of the DOC group.
//   More details can be found in:
//   http://www.cs.wustl.edu/~schmidt/oopsla.ps.gz
//   http://www.cs.wustl.edu/~schmidt/JSAC-98.ps.gz
//
//
// ============================================================================

#ifndef TAO_EC_REACTIVE_TIMEOUT_GENERATOR_H
#define TAO_EC_REACTIVE_TIMEOUT_GENERATOR_H

#include "EC_Timeout_Generator.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class TAO_ORBSVCS_Export TAO_EC_Reactive_Timeout_Generator : public TAO_EC_Timeout_Generator
{
  // = TITLE
  //   A simple implementation of the Timeout_Generator based on the
  //   Reactor.
  //
  // = DESCRIPTION
  //   Implements the Timeout_Generator using an ACE_Reactor. Usually
  //   the same reactor that is used by the ORB where the EC runs.
  //
public:
  TAO_EC_Reactive_Timeout_Generator (ACE_Reactor *reactor = 0);
  // Constructor.
  // If <reactor> is 0 then we use the reactor in the ORB singleton.

  virtual ~TAO_EC_Reactive_Timeout_Generator (void);
  // destructor

  // = The TAO_EC_Timeout_Generator methods.
  virtual void activate (void);
  virtual void shutdown (void);
  virtual int schedule_timer (TAO_EC_Timeout_Filter* filter,
                              const ACE_Time_Value& delta,
                              const ACE_Time_Value& interval);
  virtual int cancel_timer (const TAO_EC_QOS_Info& info,
                            int id);

private:
  ACE_Reactor *reactor_;
  // The reactor

  TAO_EC_Timeout_Adapter event_handler_;
  // An adapter to receive the timeout events.
};

#if defined (__ACE_INLINE__)
#include "EC_Reactive_Timeout_Generator.i"
#endif /* __ACE_INLINE__ */

#endif /* TAO_EC_REACTIVE_TIMEOUT_GENERATOR_H */
