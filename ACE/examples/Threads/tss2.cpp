// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = FILENAME
//    TSS_Test.cpp
//
// = DESCRIPTION
//     This program tests various features of ACE_Thread and the
//     thread-specific storage variant of <ACE_Singleton>.
//
// = AUTHOR
//    Prashant Jain and Doug Schmidt
//
// ============================================================================

#include "ace/OS_main.h"
#include "ace/Thread.h"
#include "ace/Log_Msg.h"
#include "ace/Atomic_Op.h"
#include "TSS_Data.h"
#include "TSS_Obj.h"
#include "TSS_Task.h"



#if defined (ACE_HAS_THREADS)

const int MAX_TASKS = 4;
const int MAX_ITERATIONS = 10;

ACE_Atomic_Op<ACE_Token, int> Test_Task::count_ (0);
ACE_Atomic_Op<ACE_Token, int> Test_Task::wait_count_ (0);
ACE_Atomic_Op<ACE_Token, int> Test_Task::max_count_ (0);
int num_tasks = 0;

// ACE synchronization object.
static ACE_Token token;

ACE_Atomic_Op<ACE_Thread_Mutex, int> TSS_Obj::count_ = 0;

TSS_Obj::TSS_Obj (void)
{
  TSS_Obj::count_++;
  ACE_DEBUG ((LM_DEBUG, "(%t) TSS_Obj+: %d\n", TSS_Obj::count_.value ()));
}

TSS_Obj::~TSS_Obj (void)
{
  TSS_Obj::count_--;
  ACE_DEBUG ((LM_DEBUG, "(%t) TSS_Obj-: %d\n", TSS_Obj::count_.value ()));
}

Test_Task::Test_Task (void)
{
  Test_Task::count_++;
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Test_Task+: %d\n", Test_Task::count_.value ()));
}

Test_Task::~Test_Task (void)
{
  Test_Task::count_--;

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Test_Task-: %d\n", Test_Task::count_.value ()));
  Test_Task::wait_count_--;
}

void *
Test_Task::svc (void *arg)
{
  // When the thread exits this thread-specific object will be deleted
  // automatically.
  ACE_TSS<TSS_Obj> tss (new TSS_Obj);

  TSS_DATA::instance ()->data (arg);

  Test_Task::wait_count_++;
  Test_Task::max_count_++;

  ACE_DEBUG ((LM_DEBUG, "(%t) svc: waiting (data = %u)\n",
              arg));

  // Do a bunch of set operations on the TSS data just to make sure
  // that it's truly in TSS (it it weren't, the assertion would fail).

  while (Test_Task::max_count_ < num_tasks)
    {
      TSS_DATA::instance ()->data (arg);
      ACE_Thread::yield ();
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) svc: waiting finished (data = %u)\n",
              arg));

#if 0
  ACE_ASSERT (TSS_DATA::instance ()->data () == arg);
#endif

  delete (Test_Task *) arg;

  return 0;
}

int
Test_Task::open (void *arg)
{
  if (ACE_Thread::spawn ((ACE_THR_FUNC) Test_Task::svc, arg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Thread::spawn"), 0);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  num_tasks = argc > 1 ? ACE_OS::atoi (argv[1]) : MAX_TASKS;

  Test_Task **task_arr = 0;

  ACE_NEW_RETURN (task_arr, Test_Task *[num_tasks], -1);

  for (int i = 0; i < MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) ********* iteration %d **********\n"
                  "Test_Task::max_count_ %d\n",
                  i,
                  Test_Task::max_count_.value ()));
      Test_Task::max_count_ = 0;

      for (int j = 0; j < num_tasks; j++)
        {
          ACE_NEW_RETURN (task_arr[j], Test_Task, -1);
          task_arr[j]->open (task_arr[j]);
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) waiting for first thread started\n"));

      for (;;)
        {
          ACE_Thread::yield ();

          if (Test_Task::max_count_ != 0 )
            break;
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) First thread started\n"
                  "Waiting for all threads finished\n"));

      for (;;)
        {
          if (!(Test_Task::max_count_ == num_tasks
                && Test_Task::wait_count_ == 0))
            {
              ACE_Thread::yield ();
              continue;
            }
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) Test_Task::max_count_ = %d,"
                      " Test_Task::wait_count_ = %d",
                      Test_Task::max_count_.value (),
                      Test_Task::wait_count_.value ()));
          break;
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) all threads finished\n"));
    }

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_TSS_Singleton<TSS_Data, ACE_SYNCH_MUTEX> *
  ACE_TSS_Singleton<TSS_Data, ACE_SYNCH_MUTEX>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */

#else

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
