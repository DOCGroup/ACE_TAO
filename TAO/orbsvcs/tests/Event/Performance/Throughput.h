/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Throughput.h
 *
 *  @author Carlos O'Ryan (coryan@cs.wustl.edu)
 */
//=============================================================================

#ifndef EC_THROUGHPUT_H
#define EC_THROUGHPUT_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

/**
 * @class EC_Throughput
 *
 * @brief Test the EC throughput
 */
class EC_Throughput : public EC_Driver
{
public:
  /// Constructor
  EC_Throughput () = default;
  EC_Throughput (EC_Throughput const&) = delete;
  EC_Throughput (EC_Throughput&&) = delete;
  EC_Throughput& operator= (EC_Throughput const&) = delete;
  EC_Throughput& operator= (EC_Throughput&&) = delete;

  // = The EC_Driver methods
  /// add some command line args to enable/disable throughputions
  int parse_args (int& argc, ACE_TCHAR* argv[]) override;
  void print_args () const override;
  void print_usage () override;

  /// set the throughpution flags
  void modify_attributes (TAO_EC_Event_Channel_Attributes& attr) override;

  /// Don't run the suppliers, just test connect and disconnect calls.
  void execute_test () override;

  /// Don't dump the EC_Driver results, they are meaningless.
  void dump_results () override;
};

#endif /* EC_THROUGHPUT_H */
