// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Synch.h"
#include "Adaptive_Lock_Performance_Test_Base.h"

ACE_RCSID(Synch_Benchmarks, adaptive_recursive_lock_test, "$Id$")

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Adaptive_Recursive_Lock_Test : public Adaptive_Lock_Performance_Test_Base
{
public:
  virtual int init (int, char *[]);
};

int
Adaptive_Recursive_Lock_Test::init (int, char *[])
{
  ACE_Lock *lock;
  ACE_NEW_RETURN (lock,
                  ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex> (),
                  -1);

  return this->set_lock (lock);
}

ACE_SVC_FACTORY_DECLARE (Adaptive_Recursive_Lock_Test)
ACE_SVC_FACTORY_DEFINE  (Adaptive_Recursive_Lock_Test)

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Lock_Adapter<ACE_Recursive_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
#endif /* ACE_HAS_THREADS */
