// $Id$

#include "Notify_MT_Worker_Task.h"
#include "Notify_Command.h"
#include "Notify_AdminProperties.h"
#include "Notify_Buffering_Strategy.h"

#include "tao/debug.h"

ACE_RCSID(Notify, Notify_MT_Worker_Task, "$Id$")

TAO_Notify_MT_Worker_Task::TAO_Notify_MT_Worker_Task (int n_threads, long flags, int force_active, long priority)
  :buffering_strategy_ (0),
   queue_length_ (0),
   n_threads_ (n_threads),
   flags_ (flags),
   force_active_ (force_active),
   priority_ (priority)
{
}

TAO_Notify_MT_Worker_Task::~TAO_Notify_MT_Worker_Task ()
{
  delete this->buffering_strategy_;
}

int
TAO_Notify_MT_Worker_Task::init_task (TAO_Notify_AdminProperties* const admin_properties)
{
  // Store the admin properties...

  this->queue_length_ = admin_properties->queue_length ();

  // Make us an Active Object.
  if (this->activate (flags_, n_threads_, force_active_, priority_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("activate failed")), -1);

  // Create the dispatching strategy.
  this->buffering_strategy_ =
    new TAO_Notify_Buffering_Strategy (admin_properties->queue_length ());

  this->buffering_strategy_->max_queue_length (admin_properties->max_queue_length ());

  // temporary
  this->buffering_strategy_->order_policy (CosNotification::PriorityOrder);
  return 0;
}

void
TAO_Notify_MT_Worker_Task::shutdown (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
{
  // Put a shutdown message in the task queue and wait here till all
  // threads exit.
  this->close (0);
}

int
TAO_Notify_MT_Worker_Task::close (u_long)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("(%t) close of worker\n")));

  TAO_Notify_Shutdown_Command * mb = new TAO_Notify_Shutdown_Command ();

  TAO_ENV_DECLARE_NEW_ENV;
  this->process_event (mb TAO_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We can not wait for ourselves to quit
  if (this->thr_mgr ())
    {
      // call this->thr_mgr()->task() in the main thread will assert()
      // fail in ACE_Thread_Manager::thread_desc_self (void) so I get
      // task this way.
      ACE_Thread_Descriptor *mydesc = this->thr_mgr ()->thread_descriptor (ACE_OS::thr_self ());

      if (mydesc && mydesc->task () == this)
        return -1;
    }
  return this->wait ();
}

int
TAO_Notify_MT_Worker_Task::process_event (TAO_Notify_Command *mb TAO_ENV_ARG_DECL, ACE_Time_Value *tv)
{
  // Execute the buffering strategy.
  this->buffering_strategy_->execute (this->msg_queue (), mb TAO_ENV_ARG_PARAMETER, tv);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_Notify_MT_Worker_Task::svc (void)
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

          // Decrement the global event count.
          (*this->queue_length_)--;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "removing from queue\n"));
          TAO_Notify_Command *command =
            ACE_dynamic_cast(TAO_Notify_Command*, mb);

          if (command == 0)
            {
              ACE_Message_Block::release (mb);
              continue;
            }

          int result = command->execute (TAO_ENV_SINGLE_ARG_PARAMETER);
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

TAO_Notify_Shutdown_Command::TAO_Notify_Shutdown_Command (void)
  :TAO_Notify_Command (0,0)
{
}

int
TAO_Notify_Shutdown_Command::execute (TAO_ENV_SINGLE_ARG_DECL_NOT_USED /*TAO_ENV_SINGLE_ARG_PARAMETER*/)
{
  return -1;
}
/**************************************************************************/
