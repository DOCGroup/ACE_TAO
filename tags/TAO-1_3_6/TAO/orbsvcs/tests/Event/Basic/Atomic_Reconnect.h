/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Atomic_Reconnect.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_ATOMIC_RECONNECT_H
#define EC_ATOMIC_RECONNECT_H

#include "Counting_Consumer.h"
#include "orbsvcs/Channel_Clients.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class Consumer
 *
 * @brief Simple consumer object
 *
 */
class Consumer : public EC_Counting_Consumer
{
public:
  /// Constructor
  Consumer (const char* name,
            int event_base_type);

  void dump_results (int expected_count,
                     int tolerance);
  void dump_results (int base_count,
                     int extra_count,
                     int tolerance);

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  /// Number of events of type <event_base_type_> received.
  CORBA::ULong event_base_count;

private:
  /// Base event type
  int event_base_type_;

  /// Synchronize access to the counter
  TAO_SYNCH_MUTEX lock_;
};

#endif /* EC_ATOMIC_RECONNECT_H */
