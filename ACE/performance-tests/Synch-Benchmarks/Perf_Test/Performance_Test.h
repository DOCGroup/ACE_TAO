// -*- C++ -*-
#ifndef ACE_PERFORMANCE_TEST_H
#define ACE_PERFORMANCE_TEST_H

#include "ace/svc_export.h"

#if defined (ACE_HAS_THREADS)

#include "Synch_Lib/Benchmark_Base.h"

class ACE_Svc_Export Performance_Test : public Benchmark_Method_Base
{
public:
  Performance_Test ();
  Performance_Test (Performance_Test const &) = delete;
  Performance_Test (Performance_Test &&) = delete;
  Performance_Test &operator= (Performance_Test const &) = delete;
  Performance_Test &operator= (Performance_Test &&) = delete;
  int init (int argc, ACE_TCHAR *argv[]) override;
  int pre_run_test (Benchmark_Base *bp) override;
  int run_test () override;
  int post_run_test () override;
  int valid_test_object (Benchmark_Base *) override;
private:
  int n_lwps_ {};
  int orig_n_lwps_ {};
};

ACE_SVC_FACTORY_DECLARE (Performance_Test)

#endif  /* ACE_HAS_THREADS */

#endif /* ACE_PERFORMANCE_TEST_H */
