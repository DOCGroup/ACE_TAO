// $Id$

#define  ACE_BUILD_SVC_DLL
#include "ace/Guard_T.h"
#include "ace/Log_Msg.h"
#include "Performance_Test_Options.h"
#include "Benchmark_Performance.h"

#if defined (ACE_HAS_THREADS)

class ACE_Svc_Export Guard_Test : public Benchmark_Performance
{
public:
  enum
  {                             // svc.conf options
    TEST_ACE_GUARD,             // -g
    TEST_ACE_THREAD_MUTEX_GUARD // -t
  };

  virtual int svc (void);
  virtual int init (int, ACE_TCHAR *[]);

  void test_guard (int);
#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
  void test_thread_guard (int);
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */
private:
  static int guard_type_;
  static ACE_Thread_Mutex mutex_;
};

int Guard_Test::guard_type_ = Guard_Test::TEST_ACE_GUARD;
ACE_Thread_Mutex Guard_Test::mutex_;

int
Guard_Test::init (int argc, ACE_TCHAR *argv[])
{
  //FUZZ: disable check_for_lack_ACE_OS
  ACE_Get_Opt getopt (argc, argv, ACE_TEXT("gt"));
  int c;

  while ((c = getopt()) != -1)
    {
  //FUZZ: enable check_for_lack_ACE_OS
      switch (c)
        {
#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
        case 't':
          Guard_Test::guard_type_ = Guard_Test::TEST_ACE_THREAD_MUTEX_GUARD;
          break;
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */
        case 'g':
          Guard_Test::guard_type_ = Guard_Test::TEST_ACE_GUARD;
          break;
        default:
          ACE_ERROR_RETURN ((LM_ERROR,
                             "Invalid option\n"), -1);
        }
    }
  return 0;
}

int
Guard_Test::svc (void)
{
  // Extract out the unique thread-specific value to be used as an
  // index...
  int ni = this->thr_id ();
  synch_count = 2;

  switch (Guard_Test::guard_type_)
    {
    case Guard_Test::TEST_ACE_GUARD:
      this->test_guard (ni);
      break;
#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
    case Guard_Test::TEST_ACE_THREAD_MUTEX_GUARD:
      this->test_thread_guard (ni);
      break;
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */
    default:
      return -1;
    }
  return 0;
}

void
Guard_Test::test_guard (int ni)
{
  while (!this->done ())
    {
      {
        ACE_GUARD (ACE_Thread_Mutex, _ace_mon, Guard_Test::mutex_);

        performance_test_options.thr_work_count[ni]++;
        buffer++;
      }
    }
}

#if defined (ACE_USES_OBSOLETE_GUARD_CLASSES)
# define ACE_THREAD_GUARD(OBJ,LOCK) \
  ACE_Thread_Mutex_Guard OBJ (LOCK); \
    if (OBJ.locked () == 0) return;

void
Guard_Test::test_thread_guard (int ni)
{
  while (!this->done ())
    {
      {
        ACE_THREAD_GUARD (_ace_mon, Guard_Test::mutex_);

        performance_test_options.thr_work_count[ni]++;
        buffer++;
      }
    }
}
#endif /* ACE_USES_OBSOLETE_GUARD_CLASSES */
ACE_SVC_FACTORY_DECLARE (Guard_Test)
ACE_SVC_FACTORY_DEFINE  (Guard_Test)

// ACE_Service_Object_Type mut (&mutex_test, "Guard_Test");
#endif /* ACE_HAS_THREADS */
