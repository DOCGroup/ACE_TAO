
// $Id$

#include "mld.h"
#include "task.h"
#include "work.h"
#include "block.h"

int run_test (int iterations, int threads, int subtasks)
{
        // Create a task with some subtasks.  Each Task is a thread
        // pool of 'threads' size.  If a task has a subtask, it will
        // forward the unit of work to the subtask when finished.  See 
        // task.{h|cpp} for more details.
    Task *task = new Task (subtasks);

    if (task->start (threads) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "start"), -1);
    }

        // Give the threads a chance to get ready.
    ACE_OS::sleep (ACE_Time_Value (1));

    for (int i = 0; i < iterations; ++i)
    {
            // Create a custom message block that can contain our Work object
        Message_Block *message = new Message_Block (new Work (i));

            // Put the "unit of work" into the message queue
        if (task->putq (message) == -1)
        {
            ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "putq"), -1);
        }
    }

        // The default constructor of our custom message block will
        // insert a message telling our task to shutdown.
    Message_Block *message = new Message_Block ();

        // Put the shutdown request into the thread pool
    if (task->putq (message) == -1)
    {
        ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "putq"), -1);
    }

        // Wait for the task to shut down.  Any subtasks will also be
        // waited for.
    task->wait ();

        // Delete our Task to prevent a memory leak
    delete task;

        // Ask our memory leak detector if things are OK
    if (MLD_COUNTER != 0)
    {
        ACE_DEBUG ((LM_DEBUG, "(%P|%t) Memory Leak! (counter = %d)\n",MLD_COUNTER));
    }

    return (0);
}

int main (int argc, char *argv[])
{
        // Number of Work objects to put into the Task pool
    int iterations = argc > 1 ? atoi (argv[1]) : 4;
        // Number of threads for each Task
    int threads = argc > 2 ? atoi (argv[2]) : 2;
        // Number of tasks to chain after the primary task
    int subtasks = argc > 3 ? atoi (argv[3]) : 1;

    (void) run_test (iterations, threads, subtasks);

    ACE_DEBUG ((LM_DEBUG, "(%P|%t) Application exiting\n"));

    return (0);
}
#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Guard < ACE_Mutex >;
template class ACE_Lock_Adapter < ACE_Mutex >;
template class ACE_Atomic_Op < ACE_Mutex, int >;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Guard<ACE_Mutex>;
#pragma instantiate ACE_Lock_Adapter<ACE_Mutex>;
#pragma instantiate ACE_Atomic_Op<ACE_Mutex, int>;
#endif /*
         ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION 
       */
