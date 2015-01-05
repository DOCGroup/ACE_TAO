/**
 * @file Bug_3541_Regression_Test.cpp
 *
 * $Id$
 *
 * Reproduces the problems reported in bug 3541:
 *   http://deuce.doc.wustl.edu/bugzilla/show_bug.cgi?id=3541
 *
 * @author Bill Rizzi <rizzi@softserv.com>
 */

#include "ace/Event.h"
#include "ace/Mutex.h"
#include "ace/Semaphore.h"
#include "ace/OS_NS_errno.h"
#include "ace/SString.h"
#include "test_config.h"

int
run_main (int, ACE_TCHAR *[])
{
  ACE_START_TEST (ACE_TEXT ("Bug_3541_Regression_Test"));

  int ret = 0;

#if defined(ACE_WIN32) && !defined (ACE_USES_WINCE_SEMA_SIMULATION)
  int lastError;

  // ACE_OS::event_init()

  const ACE_TCHAR *eventName = ACE_TEXT ("Bug3541_Event");

  ACE_Event event0(0,             // int manual_reset = 0
                   0,             // int initial_state = 0
                   USYNC_PROCESS, // int type = USYNC_THREAD
                   eventName);    // const ACE_TCHAR *name = 0

  lastError = ACE_OS::last_error();

  ACE_event_t eventHandle = event0.handle();

  if ((eventHandle == ACE_INVALID_HANDLE) ||
      (lastError != 0))
  {
    ret = -1;

    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Event(%s) failed - handle %d lastError %d\n"),
                eventName, eventHandle, lastError));
  }
  else
  {
    ACE_Event event1(0,              // int manual_reset = 0
                     0,              // int initial_state = 0
                     USYNC_PROCESS,  // int type = USYNC_THREAD
                     eventName);     // const ACE_TCHAR *name = 0

    lastError = ACE_OS::last_error();

    eventHandle = event1.handle();

    if ((eventHandle == ACE_INVALID_HANDLE) ||
        (lastError != ERROR_ALREADY_EXISTS))
    {
      ret = -1;

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ACE_Event(%s) failed - handle %d lastError %d\n"),
                  eventName, eventHandle, lastError));
    }
  }

  // ACE_OS::sema_init

  const ACE_TCHAR *semaphoreName = ACE_TEXT ("Bug3541_Semaphore");

  ACE_Semaphore semaphore0(1,               // int count = 1
                           USYNC_PROCESS,   // int type = USYNC_THREAD
                           semaphoreName);  // const ACE_TCHAR *name = 0

  lastError = ACE_OS::last_error();

  const ACE_sema_t &semaphoreLock = semaphore0.lock();
  if ((semaphoreLock == ACE_INVALID_HANDLE) ||
      (lastError != 0))
  {
    ret = -1;

    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Semaphore(%s) failed - lock %d lastError %d\n"),
                semaphoreName, semaphoreLock, lastError));
  }
  else
  {
    ACE_Semaphore semaphore1(1,               // int count = 1
                             USYNC_PROCESS,   // int type = USYNC_THREAD
                             semaphoreName);  // const ACE_TCHAR *name = 0

    lastError = ACE_OS::last_error();

    const ACE_sema_t &semaphoreLock = semaphore1.lock();

    if ((semaphoreLock == ACE_INVALID_HANDLE) ||
        (lastError != ERROR_ALREADY_EXISTS))
    {
      ret = -1;

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ACE_Semaphore(%s) failed - lock %d lastError %d\n"),
                  semaphoreName, semaphoreLock, lastError));
    }
  }

  // ACE_OS::mutex_init()

  const ACE_TCHAR *mutexName = ACE_TEXT ("Bug3541_Mutex");

  ACE_Mutex mutex0(USYNC_PROCESS,  // int type = USYNC_THREAD
                   mutexName);     // const ACE_TCHAR *name = 0

  lastError = ACE_OS::last_error();

  const ACE_mutex_t &mutexLock = mutex0.lock();

  if ((mutexLock.proc_mutex_ == ACE_INVALID_HANDLE) ||
      (lastError != 0))
  {
    ret = -1;

    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("ACE_Mutex(%s) failed - lock %d lastError %d\n"),
                mutexName, mutexLock, lastError));
  }
  else
  {
    ACE_Mutex mutex1(USYNC_PROCESS,  // int type = USYNC_THREAD
                     mutexName);     // const ACE_TCHAR *name = 0

    lastError = ACE_OS::last_error();

    const ACE_mutex_t &mutexLock = mutex1.lock();

    if ((mutexLock.proc_mutex_ == ACE_INVALID_HANDLE) ||
        (lastError != ERROR_ALREADY_EXISTS))
    {
      ret = -1;

      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("ACE_Mutex(%s) failed - lock %d lastError %d\n"),
                  mutexName, mutexLock, lastError));
    }
  }

 #endif

  ACE_END_TEST;

  return ret;
}
