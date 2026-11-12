/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Control.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef EC_CONTROL_H
#define EC_CONTROL_H

#include "Counting_Consumer.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Consumer
 *
 * @brief Simple consumer object
 */
class Consumer : public EC_Counting_Consumer
{
public:
  Consumer (const char* name,
            int event_count);
  Consumer (Consumer const &) = delete;
  Consumer &operator= (Consumer const &) = delete;
  Consumer (Consumer &&) = delete;
  Consumer &operator= (Consumer &&) = delete;

  // = The RtecEventComm::PushConsumer methods

  void push (const RtecEventComm::EventSet& events) override;

private:
  /// After this number of events the consumer disconnects from the
  /// event service.
  CORBA::ULong shutdown_count_;

  /// Synchronize access to the counter
  TAO_SYNCH_MUTEX lock_;
};

#endif /* EC_CONTROL_H */
