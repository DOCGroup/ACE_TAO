/* -*- C++ -*- */
//=============================================================================
/**
 *  @file   Throughput.h
 *
 *  $Id$
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
 *
 */
class EC_Throughput : public EC_Driver
{
public:
  /// Constructor
  EC_Throughput (void);

  // = The EC_Driver methods
  /// add some command line args to enable/disable throughputions
  virtual int parse_args (int& argc, ACE_TCHAR* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);

  /// set the throughpution flags
  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);

  /// Don't run the suppliers, just test connect and disconnect calls.
  void execute_test (void);

  /// Don't dump the EC_Driver results, they are meaningless.
  void dump_results (void);

private:
};

#endif /* EC_THROUGHPUT_H */
