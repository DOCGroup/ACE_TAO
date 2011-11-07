// $Id$

// The following test was written by Hamutal Yanay & Ari Erev's
// (Ari_Erev@comverse.com).
//
// This test program test enhancements to the thread_manager and task
// classes.  The purpose of these enhancements was to allow the
// thread_manager to recognize the concept of an ACE_Task and to be
// able to group ACE_Tasks in groups.
//
// There are two main ACE_Tasks in this sample:
//
// Invoker_Task - is run from main (). It's purpose is to run a number of
//                ACE_Tasks of type Worker_Task. The number can be specified
//                on the command line.
//                After starting the tasks, the Invoker_Task groups all the tasks
//                in one group and then uses the
//                num_tasks_in_group () to find out if the real number of tasks
//                that are now running (should be the same as the number of tasks
//                started).
//                It also, suspends and resumes all the threads in the group to
//                test the suspend_grp () and resume_grp () methods.
//                Then it waits for all the tasks to end.
//
// Worker_Task  - ACE_Tasks that are started by the Invoker_Task.
//                Each Worker_Task can start a number of threads.
//                The Worker_Task threads perform some work (iteration). The number
//                of the iterations can be specified on the command line.
//
// The command line syntax is:
//
// test_task   [num_tasks] [num_threads] [num_iterations]

#include "ace/OS_NS_unistd.h"
#include "ace/OS_main.h"
#include "ace/Task.h"
#include "ace/Service_Config.h"

#if defined (ACE_HAS_THREADS)

#include "ace/Task.h"

class Invoker_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Invoker_Task (ACE_Thread_Manager *thr_mgr,
                size_t n_tasks,
                size_t n_threads,
                size_t n_iterations);
  virtual int svc (void);
  // creats <n_tasks> and wait for them to finish

private:
  size_t n_tasks_;
  // Number of tasks to start.
  size_t n_threads_;
  // Number of threads per task.
  size_t n_iterations_;
  // Number of iterations per thread.
};

class Worker_Task : public ACE_Task<ACE_MT_SYNCH>
{
public:
  Worker_Task (ACE_Thread_Manager *thr_mgr,
               size_t n_threads,
               size_t n_iterations);
  virtual int svc (void);

  //FUZZ: disable check_for_lack_ACE_OS
  // Does a small work...
  virtual int open (void * = NULL);
  //FUZZ: enable check_for_lack_ACE_OS

private:
  static size_t workers_count_;
  size_t index_;
  size_t n_threads_;
  size_t n_iterations_;

  //FUZZ: disable check_for_lack_ACE_OS
  // = Not needed for this test.
  virtual int close (u_long);
  //FUZZ: enable check_for_lack_ACE_OS

  virtual int put (ACE_Message_Block *, ACE_Time_Value *) { return 0; }
};

size_t Worker_Task::workers_count_ = 1;

int
Worker_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG,
              "(%t) closing task %d\n",
              this->index_));
  delete this;
  return 0;
}

Worker_Task::Worker_Task (ACE_Thread_Manager *thr_mgr,
                          size_t n_threads,
                          size_t n_iterations)
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr),
    index_ (Worker_Task::workers_count_++),
    n_threads_ (n_threads),
    n_iterations_ (n_iterations)
{
}

int
Worker_Task::open (void *)
{
  // Create the pool of worker threads.
  return this->activate (THR_NEW_LWP,
                         ACE_Utils::truncate_cast<int> (this->n_threads_),
                         0,
                         -1,
                         -1,
                         this);
}

int
Worker_Task::svc (void)
{
  ACE_DEBUG ((LM_DEBUG,
              " (%t) in worker %d\n",
              index_));

  for (size_t iterations = 1;
       iterations <= this->n_iterations_;
       iterations++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  " (%t) in iteration %d\n",
                  iterations));
      ACE_OS::sleep (0);
    }

  ACE_DEBUG ((LM_DEBUG,
              " (%t) worker %d ends\n",
              index_));

  return 0;
}

Invoker_Task::Invoker_Task (ACE_Thread_Manager *thr_mgr,
                            size_t n_tasks,
                            size_t n_threads,
                            size_t n_iterations)
  : ACE_Task<ACE_MT_SYNCH> (thr_mgr),
    n_tasks_ (n_tasks),
    n_threads_ (n_threads),
    n_iterations_ (n_iterations)
{
  // Create a single worker thread.
  if (this->activate (THR_NEW_LWP,
                      1,
                      0,
                      -1,
                      -1,
                      this) == -1)
    ACE_ERROR ((LM_ERROR,
                "%p\n",
                "activate failed"));
}

// Iterate <n_iterations> time printing off a message and "waiting"
// for all other threads to complete this iteration.

int
Invoker_Task::svc (void)
{
  // Note that the ACE_Task::svc_run () method automatically adds us
  // to the Thread_Manager when the thread begins.

  ACE_Thread_Manager *thr_mgr =
    ACE_Thread_Manager::instance ();
  Worker_Task **worker_task = 0;

  ACE_NEW_RETURN (worker_task,
                  Worker_Task *[n_tasks_],
                  -1);
  size_t task = 0;

  for (task = 0;
       task < this->n_tasks_;
       task++)
    {
      ACE_DEBUG ((LM_DEBUG,
                  " (%t) in task %d\n",
                  task + 1));

      ACE_NEW_RETURN (worker_task[task],
                      Worker_Task (thr_mgr,
                                   n_threads_,
                                   n_iterations_),
                                   -1);

      if (worker_task[task]->open () == -1)
        ACE_ERROR_RETURN ((LM_ERROR,
                           "%p\n",
                           "open failed"),
                          -1);
    }

  // Set all tasks to be one group
  ACE_DEBUG ((LM_DEBUG,
              " (%t) setting tasks group id\n"));

  for (task = 0;
       task < this->n_tasks_;
       task++)
    if (thr_mgr->set_grp (worker_task[task],
                          1) == -1)
      ACE_ERROR ((LM_DEBUG,
                  " (%t) %p\n",
                  "set_grp"));

  size_t n_tasks =
    thr_mgr->num_tasks_in_group (1);
  ACE_DEBUG ((LM_DEBUG,
              "Number of tasks in group 1: %d\n",
              n_tasks)) ;

  // Wait for 1 second and then suspend every thread in the group.
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG,
              " (%t) suspending group\n"));

  if (thr_mgr->suspend_grp (1) == -1)
    ACE_ERROR ((LM_DEBUG,
                " (%t) %p\n",
                "suspend_grp"));

  // Wait for 3 more second and then resume every thread in the group.
  ACE_OS::sleep (ACE_Time_Value (2));

  ACE_DEBUG ((LM_DEBUG,
              " (%t) resuming group\n"));

  if (thr_mgr->resume_grp (1) == -1)
    ACE_ERROR ((LM_DEBUG,
                " (%t) %p\n",
                "resume_grp"));

  // Wait for all the tasks to reach their exit point.
  thr_mgr->wait ();

  // Note that the ACE_Task::svc_run () method automatically removes
  // us from the Thread_Manager when the thread exits.
  return 0;
}

// Default number of tasks and iterations.
static const size_t DEFAULT_TASKS = 4;
static const size_t DEFAULT_ITERATIONS = 5;

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  size_t n_tasks = argc > 1 ? ACE_OS::atoi (argv[1]) : DEFAULT_TASKS;
  size_t n_threads = argc > 2 ? ACE_OS::atoi (argv[2]) : ACE_DEFAULT_THREADS;
  size_t n_iterations = argc > 3 ? ACE_OS::atoi (argv[3]) : DEFAULT_ITERATIONS;

  // Since ACE_Thread_Manager can only wait for all threads, we'll
  // have special manager for the Invoker_Task.
  ACE_Thread_Manager invoker_manager;

  Invoker_Task invoker (&invoker_manager,
                        n_tasks,
                        n_threads,
                        n_iterations);

  // Wait for 1 second and then suspend the invoker task
  ACE_OS::sleep (1);
  ACE_DEBUG ((LM_DEBUG,
              " (%t) suspending invoker task\n"));

  if (invoker_manager.suspend_task (&invoker) == -1)
    ACE_ERROR ((LM_DEBUG,
                " (%t) %p\n",
                "suspend_task"));

  // Wait for 3 more second and then resume the invoker task.
  ACE_OS::sleep (ACE_Time_Value (3));

  ACE_DEBUG ((LM_DEBUG,
              " (%t) resuming invoker task\n"));

  if (invoker_manager.resume_task (&invoker) == -1)
    ACE_ERROR ((LM_DEBUG,
                " (%t) %p\n",
                "resume_task"));

  // Wait for all the threads to reach their exit point.
  invoker_manager.wait ();

  ACE_DEBUG ((LM_DEBUG,
              " (%t) done\n"));
  return 0;
}
#else
int
ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_ERROR ((LM_ERROR,
              "threads not supported on this platform\n"));
  return 0;
}
#endif /* ACE_HAS_THREADS */
