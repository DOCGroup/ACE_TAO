/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel tests
//
// = FILENAME
//   Atomic_Reconnect.h
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_ATOMIC_RECONNECT_H
#define EC_ATOMIC_RECONNECT_H

#include "Counting_Consumer.h"
#include "orbsvcs/Channel_Clients.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Consumer : public EC_Counting_Consumer
{
  // = TITLE
  //   Simple consumer object
  //
  // = DESCRIPTION
  //
public:
  Consumer (const char* name,
            int event_base_type);
  // Constructor

  void dump_results (int expected_count,
                     int tolerance);
  void dump_results (int base_count,
                     int extra_count,
                     int tolerance);

  // = The RtecEventComm::PushConsumer methods

  virtual void push (const RtecEventComm::EventSet& events
                     TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  CORBA::ULong event_base_count;
  // Number of events of type <event_base_type_> received.

private:
  int event_base_type_;
  // Base event type

  TAO_SYNCH_MUTEX lock_;
  // Synchronize access to the counter
};

#endif /* EC_ATOMIC_RECONNECT_H */
