// $Id$

// This test shows the use of an ACE_Auto_Event as a signaling
// mechanism. Two threads are created (one a reader, the other a
// writer). The reader waits till the writer has completed
// calculations. Upon waking up the reader prints the data calculated
// by the writer. The writer thread calculates the value and signals
// the reader when the calculation completes.

#include "ace/Service_Config.h"
#include "ace/Synch.h"
#include "ace/Singleton.h"
#include "ace/Thread_Manager.h"

#if defined (ACE_HAS_THREADS)
// Shared event between reader and writer.  The ACE_Thread_Mutex is
// necessary to make sure that only one ACE_Auto_Event is created.
// The default constructor for ACE_Auto_Event sets it initially into
// the non-signaled state.

typedef ACE_Singleton <ACE_Auto_Event, ACE_Thread_Mutex> EVENT;

// work time for writer
static int work_time;

// Reader thread.
static void *
reader (void *arg)
{
  // Shared data via a reference.
  int& data = *(int *) arg;

  ACE_Thread_Control tc (ACE_Thread_Manager::instance ());

  // Wait for writer to complete.

  ACE_DEBUG ((LM_DEBUG, "(%t) reader: waiting...... \n"));

  if (EVENT::instance ()->wait () == -1)
    {
      ACE_ERROR ((LM_ERROR, "thread wait failed"));
      ACE_OS::exit (0);
    }

  // Read shared data.
  ACE_DEBUG ((LM_DEBUG, "(%t) reader: value of data is: %d \n", data));

  return 0;
}

// Writer thread.
static void *
writer (void *arg)
{
  int& data = *(int *) arg;

  ACE_Thread_Control tc (ACE_Thread_Manager::instance ());

  // Calculate (work).
  ACE_DEBUG ((LM_DEBUG, "(%t) writer: working for %d secs\n", work_time));
  ACE_OS::sleep (work_time);

  // Write shared data.
  data = 42;

  // Wake up reader.
  ACE_DEBUG ((LM_DEBUG, "(%t) writer: calculation complete, waking reader\n"));

  if (EVENT::instance ()->signal () == -1)
    {
      ACE_ERROR ((LM_ERROR, "thread wait failed"));
      ACE_OS::exit (0);
    }

  return 0;
}

int
main (int argc, char **argv)
{
  // Shared data: set by writer, read by reader.
  int data;

  // Work time for writer.
  work_time = argc == 2 ? atoi (argv[1]) : 5;

  // threads manager
  ACE_Thread_Manager& tm = *ACE_Thread_Manager::instance ();

  // Create reader thread.
  if (tm.spawn (reader, (void *) &data) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "thread create for reader failed"), -1);

  // Create writer thread.
  if (tm.spawn (writer, (void *) &data) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "thread create for writer failed"), -1);

  // Wait for both.
  if (tm.wait () == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "thread wait failed"), -1);
  else
    ACE_DEBUG ((LM_ERROR, "graceful exit\n"));

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<ACE_Auto_Event, ACE_Thread_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<ACE_Auto_Event, ACE_Thread_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */


#else
int
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */

