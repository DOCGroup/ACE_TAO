// -*- C++ -*-
// $Id$

#if !defined (ACE_PERFORMANCE_TEST_H)
#defined (ACE_PERFORMANCE_TEST_H)

#include "ace/Service_Repository.h"
#include "ace/Synch.h"
#include "Options.h"
#include "Benchmark_Base.h"

class ACE_Svc_Export Performance_Test : public Benchmark_Method_Base
{
public:
  Performance_Test (void);
  int init (int argc, char **argv);

private:
  int n_lwps_;
  int orig_n_lwps_;
};

#endif /* ACE_PERFORMANCE_TEST_H */
