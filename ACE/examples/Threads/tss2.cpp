
//=============================================================================
/**
 *  @file    tss2.cpp
 *
 *   This program tests various features of ACE_Thread and the
 *   thread-specific storage variant of <ACE_Singleton>.
 *
 *  @author Prashant Jain and Doug Schmidt
 */
//=============================================================================


#include "ace/OS_main.h"
#include "ace/Thread.h"
#include "ace/Log_Msg.h"
#include "ace/Token.h"
#include "TSS_Data.h"
#include "TSS_Obj.h"
#include "TSS_Task.h"
#include <memory>


#if defined (ACE_HAS_THREADS)

static constexpr int MAX_TASKS = 4;
static constexpr int MAX_ITERATIONS = 10;

std::atomic<int> Test_Task::count_ {0};
std::atomic<int> Test_Task::wait_count_ {0};
std::atomic<int> Test_Task::max_count_ {0};
int num_tasks = 0;

// ACE synchronization object.
static ACE_Token token;

std::atomic<int> TSS_Obj::count_ {0};

TSS_Obj::TSS_Obj ()
{
  TSS_Obj::count_.fetch_add (1, std::memory_order_relaxed);
  ACE_DEBUG ((LM_DEBUG, "(%t) TSS_Obj+: %d\n",
              TSS_Obj::count_.load (std::memory_order_relaxed)));
}

TSS_Obj::~TSS_Obj ()
{
  TSS_Obj::count_.fetch_sub (1, std::memory_order_relaxed);
  ACE_DEBUG ((LM_DEBUG, "(%t) TSS_Obj-: %d\n",
              TSS_Obj::count_.load (std::memory_order_relaxed)));
}

Test_Task::Test_Task ()
{
  Test_Task::count_.fetch_add (1, std::memory_order_relaxed);
  ACE_DEBUG ((LM_DEBUG,
              "(%t) Test_Task+: %d\n",
              Test_Task::count_.load (std::memory_order_relaxed)));
}

Test_Task::~Test_Task ()
{
  Test_Task::count_.fetch_sub (1, std::memory_order_relaxed);

  ACE_DEBUG ((LM_DEBUG,
              "(%t) Test_Task-: %d\n",
              Test_Task::count_.load (std::memory_order_relaxed)));
}

void *
Test_Task::svc (void *arg)
{
  // The task array in main retains ownership until all worker threads finish.
  // When the thread exits this thread-specific object will be deleted
  // automatically.
  ACE_TSS<TSS_Obj> tss (new TSS_Obj);

  TSS_DATA::instance ()->data (arg);

  Test_Task::wait_count_.fetch_add (1, std::memory_order_relaxed);
  Test_Task::max_count_.fetch_add (1, std::memory_order_relaxed);

  ACE_DEBUG ((LM_DEBUG, "(%t) svc: waiting (data = %u)\n",
              arg));

  // Do a bunch of set operations on the TSS data just to make sure
  // that it's truly in TSS (it it weren't, the assertion would fail).

  while (Test_Task::max_count_.load (std::memory_order_relaxed) < num_tasks)
    {
      TSS_DATA::instance ()->data (arg);
      ACE_Thread::yield ();
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) svc: waiting finished (data = %u)\n",
              arg));

#if 0
  ACE_ASSERT (TSS_DATA::instance ()->data () == arg);
#endif

  Test_Task::wait_count_.fetch_sub (1, std::memory_order_relaxed);
  return nullptr;
}

int
Test_Task::open (void *arg)
{
  if (ACE_Thread::spawn (Test_Task::svc, arg) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "ACE_Thread::spawn"), 0);

  return 0;
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  num_tasks = argc > 1 ? ACE_OS::atoi (argv[1]) : MAX_TASKS;

  std::unique_ptr<Test_Task> *task_array_storage {};
  ACE_NEW_RETURN (task_array_storage,
                  std::unique_ptr<Test_Task>[num_tasks],
                  -1);
  std::unique_ptr<std::unique_ptr<Test_Task>[]> task_arr {task_array_storage};

  for (int i = 0; i < MAX_ITERATIONS; i++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t) ********* iteration %d **********\n"
                  "Test_Task::max_count_ %d\n",
                  i,
                  Test_Task::max_count_.load (std::memory_order_relaxed)));
      Test_Task::max_count_.store (0, std::memory_order_relaxed);

      for (int j = 0; j < num_tasks; j++)
        {
          Test_Task *task {};
          ACE_NEW_RETURN (task, Test_Task, -1);
          task_arr[j].reset (task);
        }

      for (int j = 0; j < num_tasks; j++)
        {
          if (task_arr[j]->open (task_arr[j].get ()) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "failed to start Test_Task thread\n"), -1);
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) waiting for first thread started\n"));

      for (;;)
        {
          ACE_Thread::yield ();

          if (Test_Task::max_count_.load (std::memory_order_relaxed) != 0)
            break;
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) First thread started\n"
                  "Waiting for all threads finished\n"));

      for (;;)
        {
          if (!(Test_Task::max_count_.load (std::memory_order_relaxed) == num_tasks
                && Test_Task::wait_count_.load (std::memory_order_relaxed) == 0))
            {
              ACE_Thread::yield ();
              continue;
            }
          ACE_DEBUG ((LM_DEBUG,
                      "(%t) Test_Task::max_count_ = %d,"
                      " Test_Task::wait_count_ = %d",
                      Test_Task::max_count_.load (std::memory_order_relaxed),
                      Test_Task::wait_count_.load (std::memory_order_relaxed)));
          break;
        }

      ACE_DEBUG ((LM_DEBUG, "(%t) all threads finished\n"));
    }

  return 0;
}

ACE_SINGLETON_TEMPLATE_INSTANTIATE(ACE_TSS_Singleton, TSS_Data, ACE_SYNCH_MUTEX);


#else

int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
