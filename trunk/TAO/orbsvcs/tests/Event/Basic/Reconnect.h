/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Reconnect
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_RECONNECT_H
#define EC_RECONNECT_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EC_Reconnect : public EC_Driver
{
  //
  // = TITLE
  //   Test the EC reconnection feature
  //
  // = DESCRIPTION
  //   The EC can be configured to allow re-connection of suppliers
  //   and consumers, this test verifies that:
  //   + The EC does *not* allow reconnections if the feature is
  //     disabled (the default)
  //   + The EC does allow reconnections if the feature is enabled
  //     and:
  //     - The subscriptions are changed (eventually)
  //     - The are no memory leaks
  //     - Compares the time required for a reconnection vs a complete
  //       connect/disconnect cycle, specially as the number of
  //       suppliers and consumers increases.
  //
public:
  EC_Reconnect (void);
  // Constructor

  // = The EC_Driver methods
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_usage (void);
  // add some command line args to enable/disable reconnections

  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);
  // set the reconnection flags

  void execute_test (CORBA::Environment& ACE_TRY_ENV);
  // Don't run the suppliers, just test connect and disconnect calls.

  void execute_consumer_test (CORBA::Environment& ACE_TRY_ENV);
  void execute_supplier_test (CORBA::Environment& ACE_TRY_ENV);
  // Separate the suppliers and consumers.

  virtual void consumer_push (void* consumer_cookie,
                              const RtecEventComm::EventSet& event,
                              CORBA::Environment& ACE_TRY_ENV);
  // Verify that the subscription changes

private:
  int allow_consumer_reconnect_;
  int allow_supplier_reconnect_;
  // What aspect of reconnection are we going to test?

  EC_Driver::Latency_Stats consumer_reconnect_;
  EC_Driver::Latency_Stats supplier_reconnect_;
};

#endif /* EC_RECONNECT_H */
