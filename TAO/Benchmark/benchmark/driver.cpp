// $Id$

#include "benchmark/driver.h"

ACE_RCSID(benchmark, driver, "$Id$")

template <class T>
CORBA_Benchmark_Driver<T>::CORBA_Benchmark_Driver (T *proxy)
  : proxy_ (proxy)
{
}

template <class T>
CORBA_Benchmark_Driver<T>::~CORBA_Benchmark_Driver (void)
{
  delete proxy_;
}

template <class T>
int
CORBA_Benchmark_Driver<T>::run (int argc, char *argv[])
{
  // The run method delegates the task of parsing the command line options to
  // the options data member
  if (this->options_.parse_args (argc, argv) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "CORBA_Benchmark_Driver cannot parse arguments\n"));
      return -1;
    }

#if 0
  // get a handle to the proxy
  this->proxy_ = this->options_.make_proxy ();
  if (this->proxy_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "CORBA_Benchmark_Driver cannot make proxy\n"));
      return -1;
    }
#endif

  // The driver delegates the task of running the test to the newly created
  // proxy 
  if (this->proxy_->run (this->options_, this->results_) == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "CORBA_Benchmark_Driver cannot run\n"));
      return -1;
    }

  // The driver delegates the task of printing the results to the TTCP_Results
  // class 
  if (this->results_.print_results () == -1)
    {
      ACE_DEBUG ((LM_DEBUG, "CORBA_Benchmark_Driver cannot print results\n"));
      return -1;
    }

  return 0;
}


