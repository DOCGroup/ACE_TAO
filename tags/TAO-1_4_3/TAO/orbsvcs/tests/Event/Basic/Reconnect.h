/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Reconnect.h
 *
 *  $Id$
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================


#ifndef EC_RECONNECT_H
#define EC_RECONNECT_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class EC_Reconnect
 *
 * @brief Test the EC reconnection feature
 *
 * The EC can be configured to allow re-connection of suppliers
 * and consumers, this test verifies that:
 * + The EC does *not* allow reconnections if the feature is
 * disabled (the default)
 * + The EC does allow reconnections if the feature is enabled
 * and:
 * - There are no memory leaks
 * - Compares the time required for a reconnection vs a complete
 * connect/disconnect cycle, specially as the number of
 * suppliers and consumers increases.
 */
class EC_Reconnect : public EC_Driver
{
public:
  /// Constructor
  EC_Reconnect (void);

  // = The EC_Driver methods
  /// add some command line args to enable/disable reconnections
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);

  /// set the reconnection flags
  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);

  /// Don't run the suppliers, just test connect and disconnect calls.
  void execute_test (ACE_ENV_SINGLE_ARG_DECL);

  /// Don't dump the EC_Driver results, they are meaningless.
  void dump_results (void);

  /// Separate the suppliers and consumers.
  void execute_consumer_test (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);
  void execute_supplier_test (ACE_ENV_SINGLE_ARG_DECL_WITH_DEFAULTS);

private:
  /// What aspect of reconnection are we going to test?
  int allow_consumer_reconnect_;
  int allow_supplier_reconnect_;

  /// The number of disconnections
  int disconnections_;

  ACE_Throughput_Stats consumer_reconnect_;
  ACE_Throughput_Stats supplier_reconnect_;
};

#endif /* EC_RECONNECT_H */
