// This test program verifies the functionality of the ACE_OS
// $Id$

// implementation of readers/writer locks on Win32 and Posix pthreads.

#include "ace/Log_Msg.h"
#include "ace/Synch.h"
#include "ace/Thread.h"
#include "ace/Thread_Manager.h"
#include "ace/Get_Opt.h"

#if defined (ACE_HAS_THREADS)

// Default number of iterations.
static size_t n_iterations = 1000;

// Default number of loops.
static size_t n_loops = 100;

// Default number of readers.
static size_t n_readers = 6;

// Default number of writers.
static size_t n_writers = 2;

// Thread id of last writer.
volatile static int shared_data;  

// Lock for shared_data.
static ACE_RW_Mutex rw_mutex;     

// Count of the number of readers and writers.
ACE_Atomic_Op<ACE_Thread_Mutex, int> current_readers, current_writers;

// Thread manager
static ACE_Thread_Manager thr_mgr;

// Explain usage and exit.
static void 
print_usage_and_die (void)
{
  ACE_DEBUG ((LM_DEBUG, 
	      "usage: %n [-r n_readers] [-w n_writers] [-n iteration_count]\n"));
  ACE_OS::exit (1);
}

// Parse the command-line arguments and set options.
static void
parse_args (int argc, char *argv[])
{
  ACE_Get_Opt get_opt (argc, argv, "r:w:n:l:");

  int c; 

  while ((c = get_opt ()) != -1)
    switch (c)
    {
    case 'r':
      n_readers = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'w':
      n_writers = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'n':
      n_iterations = ACE_OS::atoi (get_opt.optarg);
      break;
    case 'l':
      n_loops = ACE_OS::atoi (get_opt.optarg);
      break;
    default:
      print_usage_and_die ();
      break;
  }
}

// Iterate <n_iterations> each time checking that nobody modifies the data
// while we have a read lock.

static void *
reader (void *)
{
  ACE_Thread_Control tc (&thr_mgr);
  ACE_DEBUG ((LM_DEBUG, "(%t) reader starting\n"));
  
  for (int iterations = 1; iterations <= n_iterations; iterations++)
    {
      ACE_Read_Guard<ACE_RW_Mutex> g(rw_mutex);
      int n = ++current_readers;
      //ACE_DEBUG ((LM_DEBUG, "(%t) I'm reader number %d\n", n));

      if (current_writers > 0)
        ACE_DEBUG ((LM_DEBUG, "(%t) writers found!!!\n"));
	
      int data = shared_data;

      for (int loop = 1; loop <= n_loops; loop++)
        {
	  ACE_Thread::yield();
	  if (shared_data != data)
            ACE_DEBUG ((LM_DEBUG, 
			"(%t) somebody changed %d to %d\n", 
			data, shared_data));
        }

      --current_readers;
      //ACE_DEBUG ((LM_DEBUG, "(%t) done with reading guarded data\n"));

      ACE_Thread::yield ();
    }
  return 0;
}

// Iterate <n_iterations> each time modifying the global data
// and checking that nobody steps on it while we can write it.

static void *
writer (void *)
{
  ACE_Thread_Control tc (&thr_mgr);
  ACE_DEBUG ((LM_DEBUG, "(%t) writer starting\n"));
  
  for (int iterations = 1; iterations <= n_iterations; iterations++)
    {
      ACE_Write_Guard<ACE_RW_Mutex> g(rw_mutex);

      ++current_writers;
      //ACE_DEBUG ((LM_DEBUG, "(%t) writing to guarded data\n"));

      if (current_writers > 1)
        ACE_DEBUG ((LM_DEBUG, "(%t) other writers found!!!\n"));

      if (current_readers > 0)
        ACE_DEBUG ((LM_DEBUG, "(%t) readers found!!!\n"));
	
      int self = (int) ACE_Thread::self ();
      shared_data = self;

      for (int loop = 1; loop <= n_loops; loop++)
        {
	  ACE_Thread::yield();
	  if (shared_data != self)
            ACE_DEBUG ((LM_DEBUG, "(%t) somebody wrote on my data %d\n", shared_data));
        }

      --current_writers;

      //ACE_DEBUG ((LM_DEBUG, "(%t) done with guarded data\n"));
      ACE_Thread::yield ();
    }
  return 0;
}

// Spawn off threads.

int main (int argc, char *argv[])
{
  ACE_LOG_MSG->open (argv[0]);
  parse_args (argc, argv);

  current_readers = 0; // Possibly already done
  current_writers = 0; // Possibly already done

  ACE_DEBUG ((LM_DEBUG, "(%t) main thread starting\n"));

  if (thr_mgr.spawn_n (n_readers, reader, 0, THR_NEW_LWP) == -1 ||
      thr_mgr.spawn_n (n_writers, writer, 0, THR_NEW_LWP) == -1)
     ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "spawn_n"), 1);

  thr_mgr.wait ();

  ACE_DEBUG ((LM_DEBUG, "(%t) exiting main thread\n"));
  return 0;
}

#if defined (ACE_TEMPLATES_REQUIRE_SPECIALIZATION)
template class ACE_Atomic_Op<ACE_Thread_Mutex, int>;
#endif /* ACE_TEMPLATES_REQUIRE_SPECIALIZATION */

#else
int 
main (int, char *[])
{
  ACE_ERROR ((LM_ERROR, "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */

