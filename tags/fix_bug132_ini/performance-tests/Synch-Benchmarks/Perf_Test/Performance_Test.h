// -*- C++ -*-
// $Id$

#ifndef ACE_PERFORMANCE_TEST_H
#define ACE_PERFORMANCE_TEST_H

#include "Synch_Lib/Benchmark_Base.h"

class ACE_Svc_Export Performance_Test : public Benchmark_Method_Base
{
public:
  Performance_Test (void);
  virtual int init (int argc, char *argv[]);
  virtual int pre_run_test (Benchmark_Base *bp);
  virtual int run_test (void);
  virtual int post_run_test (void);
  virtual int valid_test_object (Benchmark_Base *);
private:
  int n_lwps_;
  int orig_n_lwps_;
};

ACE_SVC_FACTORY_DECLARE (Performance_Test)

#endif /* ACE_PERFORMANCE_TEST_H */
