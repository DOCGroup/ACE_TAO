// -*- C++ -*-
// $Id$

#ifndef ACE_BASELINE_TEST_H
#define ACE_BASELINE_TEST_H

#include "Benchmark_Base.h"

class ACE_Svc_Export Benchmark_Baseline_Test_Base : public Benchmark_Base
{
  // = TITLE
  //     This class identifies itself as Benmarking Performance Test class.
public:
  virtual int acquire () = 0;
  virtual int release () = 0;
  //

protected:
  Benchmark_Baseline_Test_Base (void);
};

class ACE_Svc_Export Benchmark_Baseline : public Benchmark_Method_Base
{
public:
  Benchmark_Baseline (void);
  int init (int argc, char **argv);
  virtual int pre_run_test (Benchmark_Base *bp);
  virtual int run_test (void);
  virtual int post_run_test (void);
  virtual int valid_test_object (Benchmark_Base *);
private:
};

ACE_SVC_FACTORY_DECLARE (Baseline_Test)

#endif /* ACE_PERFORMANCE_TEST_H */
