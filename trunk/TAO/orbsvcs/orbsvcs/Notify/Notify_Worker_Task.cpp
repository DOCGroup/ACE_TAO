// $Id$

#include "Notify_Worker_Task.h"
#include "Notify_Command.h"

ACE_RCSID(Notify, Notify_Worker_Task, "$Id$")

TAO_Notify_Worker_Task::TAO_Notify_Worker_Task (TAO_Notify_Event_Manager* event_manager, CORBA::Boolean activate_object)
  :event_manager_ (event_manager),
   activate_object_ (activate_object)
{
}

TAO_Notify_Event_Manager*
TAO_Notify_Worker_Task::event_manager (void)
{
  return this->event_manager_;
}

int
TAO_Notify_Worker_Task::open (void* /*args*/)
{
  if (this->activate_object_ == 0)
    return 1;

  // Make us an Active Object.
  if (this->activate (THR_NEW_LWP) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("activate failed")), -1);
  else
    return 0;
}

void
TAO_Notify_Worker_Task::shutdown (CORBA::Environment& /*ACE_TRY_ENV*/)
{
  // Put a shutdown message in the task queue and wait here till all
  // threads exit.
  this->close (0);
}

int
TAO_Notify_Worker_Task::close (u_long)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) close of worker\n")));

  TAO_Notify_Shutdown_Command * mb = new TAO_Notify_Shutdown_Command ();

  ACE_DECLARE_NEW_CORBA_ENV;
  this->process_event (mb, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  return this->wait ();
}

int
TAO_Notify_Worker_Task::process_event (TAO_Notify_Command *mb, CORBA::Environment& ACE_TRY_ENV, ACE_Time_Value *tv)
{
  // If this task is passive, execute the command here itself.
  // @@ Create Reactive_Worker and MT_Worker
  if (this->activate_object_ == 0)
    {
      int result = mb->execute (this, ACE_TRY_ENV);
      ACE_Message_Block::release (mb);
      return result;
    }
  else
    // Simply enqueue the Command into the end of the queue.
    return this->msg_queue ()->enqueue_prio (mb, tv);
}

int
TAO_Notify_Worker_Task::svc (void)
{
  int done = 0;
  while (!done)
    {
      ACE_TRY_NEW_ENV
        {
          ACE_Message_Block *mb;
          if (this->getq (mb) == -1)
            if (ACE_OS::last_error () == ESHUTDOWN)
              return 0;
          else
            ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) getq error in Dispatching Queue\n"));

          TAO_Notify_Command *command =
            ACE_dynamic_cast(TAO_Notify_Command*, mb);

          if (command == 0)
            {
              ACE_Message_Block::release (mb);
              continue;
            }

          int result = command->execute (this, ACE_TRY_ENV);
          ACE_TRY_CHECK;

          ACE_Message_Block::release (mb);

          if (result == -1)
            done = 1;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                               "EC (%P|%t) exception in dispatching queue");
        }
      ACE_ENDTRY;
    }
  return 0;
}

/**************************************************************************/

int
TAO_Notify_Shutdown_Command::execute (TAO_Notify_Worker_Task* parent_task,
                                      CORBA::Environment& ACE_TRY_ENV)
{
  if (parent_task->next()) // if there are other tasks, tell them!.
    {
      TAO_Notify_Worker_Task* next_task =
        ACE_static_cast (TAO_Notify_Worker_Task*, parent_task->next());

      TAO_Notify_Shutdown_Command * mb = new TAO_Notify_Shutdown_Command ();
      // ACE_Message_Block::duplicate (this); // increment our ref count.
      // @@ investigate crash due to duplicate.

      next_task->process_event (mb, ACE_TRY_ENV);
    }

  return -1;
}
/**************************************************************************/
