// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Synch.h"
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

ACE_RCSID(Synch_Benchmarks, adaptive_lock_test, "$Id$")

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Adaptive_Lock_Test : public Benchmark_Performance
{
public:
  virtual int init (int, char *[]);
  virtual int fini (void);
  virtual int svc (void);

private:
  static ACE_Lock *lock_;
};

ACE_Lock *Adaptive_Lock_Test::lock_ = 0;

int
Adaptive_Lock_Test::init (int argc, char *argv[])
{
  ACE_Get_Opt getopt (argc, argv, "mr");
  int opt;

  while ((opt = getopt ()) != -1)
    {
      switch (opt)
        {
        case 'm':
          delete lock_;
          ACE_NEW_RETURN (lock_,
                          ACE_Lock_Adapter<ACE_Thread_Mutex> (),
                          0);
          break;
        case 'r':
          delete lock_;
          ACE_NEW_RETURN (lock_,
                          ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex> (),
                          0);
          break;
        default:
          break;
        }
    }
  return (lock_ != 0 ? 0 : -1);
}

int
Adaptive_Lock_Test::fini (void)
{
  delete lock_;
  lock_ = 0;
  return 0;
}

int
Adaptive_Lock_Test::svc (void)
{
  // Extract out the unique thread-specific value to be used as an
  // index...
  int ni = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      lock_->acquire ();
      options.thr_work_count[ni]++;
      buffer++;
      lock_->release ();
    }
  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (Adaptive_Lock_Test)
ACE_SVC_FACTORY_DEFINE  (Adaptive_Lock_Test)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Lock_Adapter<ACE_Thread_Mutex>;
template class ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Lock_Adapter<ACE_Thread_Mutex>
#pragma instantiate ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
// ACE_Service_Object_Type mut (&adaptive_lock_test, "Adaptive_Lock_Test");
#endif /* ACE_HAS_THREADS */
