/* -*- C++ -*- */
// $Id$
//
// ============================================================================
//
// = LIBRARY
//   ORBSVCS Real-time Event Channel testsuite
//
// = FILENAME
//   Throughput
//
// = AUTHOR
//   Carlos O'Ryan (coryan@cs.wustl.edu)
//
// ============================================================================

#ifndef EC_THROUGHPUT_H
#define EC_THROUGHPUT_H

#include "Driver.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class EC_Throughput : public EC_Driver
{
  //
  // = TITLE
  //   Test the EC throughput
  //
  // = DESCRIPTION
  //
public:
  EC_Throughput (void);
  // Constructor

  // = The EC_Driver methods
  virtual int parse_args (int& argc, char* argv[]);
  virtual void print_args (void) const;
  virtual void print_usage (void);
  // add some command line args to enable/disable throughputions

  virtual void modify_attributes (TAO_EC_Event_Channel_Attributes& attr);
  // set the throughpution flags

  void execute_test (ACE_ENV_SINGLE_ARG_DECL);
  // Don't run the suppliers, just test connect and disconnect calls.

  void dump_results (void);
  // Don't dump the EC_Driver results, they are meaningless.

private:
};

#endif /* EC_THROUGHPUT_H */
