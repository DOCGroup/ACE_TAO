/* -*- C++ -*- */
// $Id$


// ============================================================================
//
// = LIBRARY
//    CORBA Benchmark
//
// = FILENAME
//    driver.h
//
// = AUTHOR
//    Aniruddha Gokhale
//
// ============================================================================

#ifndef _CORBA_BENCHMARK_DRIVER_H_
#define _CORBA_BENCHMARK_DRIVER_H_

#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

template <class PROXY>
class CORBA_Benchmark_Driver
{
  // = TITLE
  //    CORBA Benchmark Driver.
  //
  // = DESCRIPTION
  //    This class encapsulates all the driver common to the CORBA benchmark
  //    tests. It provides a template to run the benchmarking code. The real
  //    work will be done by the PROXY class.
public:

  CORBA_Benchmark_Driver (PROXY *p);
  // Constructor.

  ~CORBA_Benchmark_Driver (void);
  // Destructor

  int run (int argc, char *argv[]);
  // This is the starting point of all CORBA benchmark tests.
  // Returns 0 on success, -1 on error.

private:
  PROXY *proxy_;
  // the actual object that will do the work

  PROXY::RESULTS results_;
  // Storage of test results.

  PROXY::OPTIONS options_;
  // CORBA test command line options
};

#endif /* CORBA_BENCHMARK_DRIVER_H */
