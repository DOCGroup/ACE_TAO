// $Id$

#include "ace/Sched_Params.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "RT_Task.h"
#include "Debug_Macros.h"
#include "Event_Channel.h"
#include "Memory_Pools.h"

#if !defined (__ACE_INLINE__)
#include "RT_Task.i"
#endif /* __ACE_INLINE__ */

#include "tao/Timeprobe.h"

ACE_RCSID(Event, RT_Task, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *TAO_RT_Task_Timeprobe_Description[] =
{
  "RT_Task - start execute",
  "RT_Task - end execute",
  "synch_threads - priority requested",
  "synch_threads - priority obtained"
};

enum
{
  // Timeprobe description table start key
  TAO_RT_TASK_START_EXECUTE = 5200,
  TAO_RT_TASK_END_EXECUTE,
  TAO_RT_TASK_SYNCH_THREADS_PRIORITY_REQUESTED,
  TAO_RT_TASK_SYNCH_THREADS_PRIORITY_OBTAINED
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (TAO_RT_Task_Timeprobe_Description,
                                  TAO_RT_TASK_START_EXECUTE);

#endif /* ACE_ENABLE_TIMEPROBES */

class ACE_RT_Task_Shutdown : public ACE_RT_Task_Command
// = TITLE
//    Flush Queue Command.
//
// = DESCRIPTION
//    This command object will call close on task_.  This is used by
//    single-threaded tasks to flush any queued messages.
{
public:
  ACE_RT_Task_Shutdown (ACE_ES_TASK *task)
    : task_ (task) {}

  virtual int execute (u_long &command_action);

  ACE_ES_TASK *task_;
};

int
ACE_RT_Task_Shutdown::execute (u_long &command_action)
{
  ACE_UNUSED_ARG (command_action);

  if (task_ == 0)
    return 1;
  else
    {
      task_->close (0);
      return 0;
    }
}

// ************************************************************

ACE_RT_Task::ACE_RT_Task (RtecScheduler::Scheduler_ptr scheduler)
  : closed_ (0)
{
  if (CORBA::is_nil (scheduler))
    {
      this->scheduler_ =
        RtecScheduler::Scheduler::_duplicate (ACE_Scheduler_Factory::server ());
    }
  else
    {
      this->scheduler_ =
        RtecScheduler::Scheduler::_duplicate (scheduler);
    }
}

ACE_RT_Task::~ACE_RT_Task (void)
{
  msg_queue_->deactivate ();
}

int
ACE_RT_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      // @@ TODO It may be necessary to pass the options to this class

#if 0
      static char* const argv[] = {
        "task"
      };
      int argc = sizeof (argv)/sizeof (argv[0]);
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "" ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif

      RtecScheduler::OS_Priority thread_priority;
      RtecScheduler::Preemption_Subpriority_t subpriority;
      RtecScheduler::Preemption_Priority_t preemption_priority;

#if 1
      this->scheduler_->priority
        (this->rt_info_,
         thread_priority,
         subpriority,
         preemption_priority ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#else
      ACE_Scheduler_Factory::server ()->priority
        (this->rt_info_,
         thread_priority,
         subpriority,
         preemption_priority ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif
      if (ACE_OS::thr_setprio (thread_priority) == -1)
        {
#if defined (ACE_HAS_STHREADS)
          if (thread_priority != 0  ||  ACE_OS::getuid () == 0)
            // Solaris 2.5.1 doesn't allow setting a thread priority to 0
            // in Time Shared scheduling class.  Maybe other priorities
            // as well?
#endif /* ACE_HAS_STHREADS */
            ACE_ERROR ((LM_ERROR, "(%P|%t) %p\n",
                        "RT_Task thr_setprio failed, "
                        "this is OK if you don't want RT threads\n"));

        }

      int done = 0;

      ACE_hthread_t self;
      ACE_OS::thr_self (self);

      int priority = 0;
      if (ACE_OS::thr_getprio (self, priority) == 0)
        ACE_DEBUG ((LM_DEBUG, "EC (%t) new thread priority = %d.\n", priority));

      // Initialize channel thread-specific data.
      ACE_ES_Memory_Pools::thr_init ();

      // Update the Scheduler's concept of this thread's rate group.
      ACE_Scheduler_Factory::set_preemption_priority (preemption_priority);

      done = this->svc_hook (priority);

      while (!done)
        {
          done = this->svc_one ();
        }

      ACE_DEBUG ((LM_DEBUG, "EC (%t) thread exiting.\n"));
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, "priority failed\n"), -1);
    }
  ACE_ENDTRY;

  return 0;
}

int
ACE_RT_Task::svc_hook (RtecScheduler::OS_Priority)
{
  return 0;
}

int
ACE_RT_Task::svc_one (void)
{
  // Dequeue the command.
  ACE_Message_Block *mb;

  if (this->getq (mb) == -1)
    {
      if (ACE_OS::last_error () == ESHUTDOWN)
        return 1;
      else
        // We'll continue in spite of this error.
        ACE_ERROR ((LM_ERROR, "%p (%t) getq error.\n", "ACE_RT_Task::svc_one"));
    }

  // Execute the command.
  ACE_RT_Task_Command *command = (ACE_RT_Task_Command *) mb;

  int result;
  u_long command_action = ACE_RT_Task_Command::RELEASE;

  {
    ACE_FUNCTION_TIMEPROBE (TAO_RT_TASK_START_EXECUTE);

    // @@ Put exception handling around this!
    result = command->execute (command_action);
  }

  switch (command_action)
    {
    case ACE_RT_Task_Command::RELEASE:
      // Free the message block.
      if (ACE_RT_Task_Command::release (command) != 0)
        ACE_ERROR ((LM_ERROR, "ACE_RT_Task::svc_one: "
                    "ACE_RT_Task_Command::release returned != 0!\n"));
      break;

    case ACE_RT_Task_Command::UNGETQ:
      this->ungetq (command);
      break;
    }

  return result;
}

// A thread has exited.
int
ACE_RT_Task::close (u_long)
{
  // If we're the last one out, call threads_closed.
  if (thr_count_ == 0)
    this->threads_closed ();

  return 0;
}

// All threads have exited.
void
ACE_RT_Task::threads_closed (void)
{
}

int
ACE_RT_Task::open_task (const char* name)
{
  const char *tempname = name;
  char tempbuffer[64];
  if (tempname == 0)
    {
      ACE_OS::sprintf (tempbuffer,
                       "unnamed task %lx", (long) this);
      tempname = tempbuffer;
    }

  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
#if 1
      rt_info_ =
        this->scheduler_->create (tempname ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#else
      rt_info_ =
        ACE_Scheduler_Factory::server()->create (tempname
                                                  ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
#endif
      // @@ TODO: We do no initialization of the new rt_info, the
      // caller does, this is (IMnsHO) very error prone.
    }
  ACE_CATCH (RtecScheduler::DUPLICATE_NAME, dn_ex)
    {
      // @@ TODO: Its already registered, IMHO this should at least
      // report a warning, but I'll stick to the previous code.
      // ACE_ERROR_RETURN ((LM_WARNING,
      // "RT_Info for %s was already createn",
      // tempname), 0);
      return 0;
    }
  ACE_ENDTRY;

  return 0;
}

int
ACE_RT_Task::try_put (ACE_Message_Block *mb)
{
  if (!closed_)
    {
      return this->msg_queue ()->enqueue_prio (mb);
    }
  else
    {
      errno = EPIPE;
      return -1;
    }
}

// The point of this method is to spawn or shutdown threads depending
// on any differences between the task's RT_Info::threads_ and how
// many threads are actually running.
int
ACE_RT_Task::synch_threads (size_t threads)
{
  if (threads > this->thr_count ())
    // Add threads.
    {
      RtecScheduler::OS_Priority thread_priority;
      RtecScheduler::Preemption_Subpriority_t subpriority;
      RtecScheduler::Preemption_Priority_t preemption_priority;

      ACE_DECLARE_NEW_CORBA_ENV;
      ACE_TRY
        {
          {
            // @@ TODO handle exceptions
            ACE_FUNCTION_TIMEPROBE (TAO_RT_TASK_SYNCH_THREADS_PRIORITY_REQUESTED);
#if 1
            this->scheduler_->priority
              (rt_info_,
               thread_priority,
               subpriority,
               preemption_priority ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
#else
            ACE_Scheduler_Factory::server ()->priority
              (rt_info_,
               thread_priority,
               subpriority,
               preemption_priority ACE_ENV_ARG_PARAMETER);
            ACE_TRY_CHECK;
#endif
          }

          ACE_DEBUG ((LM_DEBUG, "EC (%t) spawning %d threads at os thread"
                      " priority %d.\n",
                      threads - this->thr_count (),
                      thread_priority));

          // Add the difference.
          // First try real-time scheduling with specified priority.
          long flags = THR_BOUND | THR_SCHED_FIFO;
          if (this->activate (flags,
                              threads - this->thr_count (),
                              1, // Force it to spawn more threads
                              thread_priority) == -1)
            {
              // That didn't work.  Try default scheduling class with
              // the requested priority.
              flags = THR_BOUND;
              if (this->activate (flags,
                                  threads - this->thr_count (),
                                  1, // Force it to spawn more threads
                                  thread_priority) == -1)
                {
                  // That didn't work.  Finally, try default
                  // scheduling class with minimum priority.

                  // On Linux, for example, only the superuser can set
                  // the policy to other than ACE_SCHED_OTHER.  But
                  // with ACE_SCHED_OTHER, there is only one thread
                  // priority value, for example, 0.  So, let the
                  // superuser run an interesting test, but for other
                  // users use the minimum ACE_SCHED_OTHER thread
                  // priority.

                  RtecScheduler::OS_Priority fallback_priority =
                    ACE_Sched_Params::priority_min (ACE_SCHED_OTHER,
                                                    ACE_SCOPE_THREAD);

                  ACE_DEBUG ((LM_DEBUG,
                              "EC (%t) task activation at priority %d "
                              "with flags 0x%X failed; retry at priority %d "
                              "with flags 0x%X\n",
                              thread_priority,
                              flags,
                              fallback_priority,
                              THR_BOUND));

                  flags = THR_BOUND;

                  if (this->activate (flags,
                                      threads - this->thr_count (),
                                      1, // Force it to spawn more threads
                                      fallback_priority) == -1)
                    {
                      ACE_DEBUG ((LM_ERROR,
                                  "EC (%t) thread spawn at priority %d FAILED "
                                  "(errno is %d%p)!!!!\n",
                                  fallback_priority,
                                  errno,
                                  ""));
                    }
                }
            }
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "priority failed\n");
          return -1;
        }
      ACE_ENDTRY;

    }
  else
    // Remove threads.
    {
      // kill_threads has to be off the stack in case the last thread
      // deletes this RT_Task.
      int kill_threads = this->thr_count () - threads;

      for (int x = kill_threads ; x > 0; x--)
        {
          // Create a new shutdown command with a task pointer of 0.
          ACE_RT_Task_Shutdown *te = new ACE_RT_Task_Shutdown (0);

          if (te == 0)
            return -1;

          ACE_DEBUG ((LM_DEBUG, "EC (%t) enqueueing thread exit.\n"));
          if (this->putq (te) == -1)
            {
              ACE_ERROR ((LM_ERROR, "%p putq failed.\n",
                          "ACE_RT_Task::synch_threads"));
              if (ACE_RT_Task_Shutdown::release (te) != 0)
                ACE_ERROR ((LM_ERROR, "ACE_RT_Task::synch_threads: "
                            "ACE_RT_Task_Shutdown::release returned != 0!\n"));
              return -1;
            }
        }
    }

  return 0;
}


// If we are not active, we will flush the queue and then call
// this->close.  Otherwise, we will send shutdown messages to each
// thread.  ~ACE_Task_Exit will call this->close when each thread
// exits.
int
ACE_RT_Task::shutdown_task (void)
{
  // Be sure to only execute this once, and only if we're active.
  if (closed_)
    return 0;

  // This will keep any messages from entering the queue.
  closed_ = 1;

  if (thr_count_ > 0)
    return this->synch_threads (0);
  else
    {
      // Create a new flush queue command.  We're passive, so pass in
      // a reference to the task for which close will be called.
      ACE_RT_Task_Shutdown *fq = new ACE_RT_Task_Shutdown (this);

      if (fq == 0)
        {
          ACE_ERROR ((LM_ERROR, "%p.\n", "ACE_RT_Task::shutdown_threads"));
          return -1;
        }

      // Enqueue the command.
      ACE_DEBUG ((LM_DEBUG, "EC (%t) enqueueing task shutdown.\n"));
      if (this->putq (fq) == -1)
        {
          ACE_ERROR ((LM_ERROR, "%p putq failed.\n",
                      "ACE_RT_Task::shutdown_task"));
          if (ACE_RT_Task_Shutdown::release (fq) != 0)
            ACE_ERROR ((LM_ERROR, "ACE_RT_Task::shutdown_task: "
                        "ACE_RT_Task_Shutdown::release returned != 0!\n"));
          return -1;
        }
    }

  return 0;
}

// ************************************************************

ACE_RT_Thread_Manager::ACE_RT_Thread_Manager (void)
  : flags_ (0)
{
}

void
ACE_RT_Thread_Manager::suspend_spawns (void)
{
  flags_ |= THR_SUSPENDED;
}

void
ACE_RT_Thread_Manager::unsuspend_spawns (void)
{
  flags_ = 0;
  this->resume_all ();
}

int
ACE_RT_Thread_Manager::spawn_i (ACE_THR_FUNC func,
                                void *args,
                                long flags,
                                ACE_thread_t *t_id,
                                ACE_hthread_t *t_handle,
                                long priority,
                                int grp_id,
                                void *stack,
                                size_t stack_size,
                                ACE_Task_Base *task)
{
  flags |= flags_;
  return ACE_Thread_Manager::spawn_i (func, args, flags, t_id, t_handle,
                                      priority, grp_id, stack, stack_size, task);
}
