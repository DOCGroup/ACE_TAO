// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/SV_Semaphore_Simple.h"
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export SYSVSema_Test : public Benchmark_Performance
{
public:
  virtual int init (int, ACE_TCHAR *[]);
  virtual int fini (void);
  virtual int svc (void);

private:
  static ACE_SV_Semaphore_Simple *sema;
};


ACE_SV_Semaphore_Simple *SYSVSema_Test::sema = 0;

int
SYSVSema_Test::init (int, ACE_TCHAR *[])
{
#if defined (ACE_HAS_SYSV_IPC)
  ACE_NEW_RETURN (SYSVSema_Test::sema, ACE_SV_Semaphore_Simple ((key_t) 1234), -1);
  return 0;
#else
  ACE_ERROR_RETURN ((LM_ERROR, "SysV Semaphore not supported on this platform.\n"), -1);
#endif /* ACE_HAS_SYSV_IPC */
}

int
SYSVSema_Test::fini (void)
{
  delete SYSVSema_Test::sema;
  return 0;
}

int
SYSVSema_Test::svc (void)
{
  int ni      = this->thr_id ();
  synch_count = 2;

  while (!this->done ())
    {
      SYSVSema_Test::sema->acquire ();
      performance_test_options.thr_work_count[ni]++;
      buffer++;
      SYSVSema_Test::sema->release ();
    }

  SYSVSema_Test::sema->remove ();
  /* NOTREACHED */
  return 0;
}

ACE_SVC_FACTORY_DECLARE (SYSVSema_Test)
ACE_SVC_FACTORY_DEFINE  (SYSVSema_Test)

// ACE_Service_Object_Type st (&sysvsema_test, "SYSVSema_Test");
#endif /* ACE_HAS_THREADS */
