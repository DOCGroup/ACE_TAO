// $Id$

// ============================================================================
//
// = LIBRARY
//    tests
//
// = DESCRIPTION
//    Test bug 2772 regression
//
// = AUTHOR
//    Johnny Willemsen
//
// ============================================================================

#include "test_config.h"
#include "ace/Recursive_Thread_Mutex.h"
#include "ace/Condition_Recursive_Thread_Mutex.h"
#include "ace/Thread.h"

class ThreadTest
{
 public:
    ThreadTest();
    ~ThreadTest();
    int run(bool doubleLock);

 private:
    static void * workerThreadWrapper(void *);
    void workerThread();
    ACE_Recursive_Thread_Mutex m_mutex;
    ACE_Condition_Recursive_Thread_Mutex m_startedCondition;
    ACE_Condition_Recursive_Thread_Mutex m_stopCondition;
    bool m_workerRunning;
    bool m_doubleLock;
};

ThreadTest::ThreadTest() :
   m_startedCondition(m_mutex),
   m_stopCondition(m_mutex),
   m_workerRunning(false),
   m_doubleLock(false)
{
}

ThreadTest::~ThreadTest()
{
}

int
ThreadTest::run(bool doubleLock)
{
  ACE_hthread_t m_workerThreadHandle;
  ACE_thread_t m_workerThreadId;
  m_workerRunning = false;
  m_doubleLock = doubleLock;

  m_mutex.acquire();

  // Start worker thread
  int rval = ACE_Thread::spawn((ACE_THR_FUNC) workerThreadWrapper, this,
     THR_JOINABLE | THR_NEW_LWP, &m_workerThreadId, &m_workerThreadHandle,
     ACE_DEFAULT_THREAD_PRIORITY);

  if (rval == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         ACE_TEXT ("%t Could not start worker thread!\n")),
                         1);
    }

  if (!m_workerRunning)
    {
      ACE_DEBUG ((LM_DEBUG,
                 ACE_TEXT ("%t Waiting for worker thread to start running...\n")));
      m_startedCondition.wait();
    }
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%t Worker thread is running...\n")));

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%t Broadcasting STOP Condition...\n")));

  m_stopCondition.broadcast();

  m_mutex.release();

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%t Joining worker thread...\n")));

  ACE_Thread::join(m_workerThreadHandle);

  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%t Test finished...\n")));

  return 0;
}

void* ThreadTest::workerThreadWrapper(void *data)
{
   ThreadTest *thisPtr = reinterpret_cast<ThreadTest *>(data);
   thisPtr->workerThread();
   return 0;
}

void ThreadTest::workerThread()
{
   m_mutex.acquire();
   m_workerRunning = true;
   m_startedCondition.broadcast();

   ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%t Thread running, waiting for stop condition.\n")));

   if (m_doubleLock)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%t Thread starting double acquire.\n")));
      m_mutex.acquire();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%t Thread finished double acquire.\n")));
    }


   // Wait for the STOP condition to occur
   m_stopCondition.wait();
   ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("%t Thread received stop condition, exiting.\n")));

   if (m_doubleLock)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%t Thread starting double release.\n")));
      m_mutex.release();
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("%t Thread finished double acquire.\n")));
    }
   m_mutex.release();
}

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_2772_Regression_Test"));

  int status = 0;

  ThreadTest test;

  // This test passes
  ACE_DEBUG ((LM_DEBUG, "TEST 1 - Single Lock\n"));
  ACE_DEBUG ((LM_DEBUG, "--------------------\n"));

  status += test.run(false);

  // This test hangs; m_stopCondition.wait() in the worker thread
  // doesn't unlock the mutex twice and thus a deadlock occurs
  ACE_DEBUG ((LM_DEBUG, "TEST 2 - Double Lock\n"));
  ACE_DEBUG ((LM_DEBUG, "--------------------\n"));
  status += test.run(true);

  ACE_END_TEST;
  return status;
}
