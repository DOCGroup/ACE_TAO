// $Id$

#include "Notify_MT_Worker_Task.h"
#include "Notify_Command.h"
#include "Notify_AdminProperties.h"
#include "Notify_Buffering_Strategy.h"
#include "Notify_QoSAdmin_i.h"
#include "Notify_Extensions.h"

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
TAO_Notify_MT_Worker_Task::init_task (
                   TAO_Notify_AdminProperties* const admin_properties,
                   TAO_Notify_QoSAdmin_i* const qos_properties)
{
  // Store the admin properties...
  this->queue_length_ = admin_properties->queue_length ();

  // Make us an Active Object.
  if (this->activate (this->flags_, this->n_threads_, this->force_active_, this->priority_) == -1)
    ACE_ERROR_RETURN ((LM_ERROR,
                       ACE_TEXT ("%p\n"),
                       ACE_TEXT ("activate failed")), -1);

  // Create the dispatching strategy.
  ACE_NEW_RETURN (this->buffering_strategy_,
                  TAO_Notify_Buffering_Strategy (
                            admin_properties->queue_length ()),
                  -1);

  // Set the admin properties
  this->update_admin (*admin_properties);

  // Set the qos policies
  this->update_qos (*qos_properties);

  return 0;
}

void
TAO_Notify_MT_Worker_Task::shutdown (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
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

  ACE_DECLARE_NEW_CORBA_ENV;
  this->process_event (mb ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // We can not wait for ourselves to quit
  if (this->thr_mgr ())
    {
      // call this->thr_mgr ()->task () in the main thread will assert ()
      // fail in ACE_Thread_Manager::thread_desc_self (void) so I get
      // task this way.
      ACE_Thread_Descriptor *mydesc = this->thr_mgr ()->thread_descriptor (ACE_OS::thr_self ());

      if (mydesc && mydesc->task () == this)
        return -1;
    }
  return this->wait ();
}

int
TAO_Notify_MT_Worker_Task::process_event (TAO_Notify_Command *mb ACE_ENV_ARG_DECL, ACE_Time_Value *tv)
{
  // Execute the buffering strategy.
  this->buffering_strategy_->execute (this->msg_queue (), mb ACE_ENV_ARG_PARAMETER, tv);
  ACE_CHECK_RETURN (-1);

  return 0;
}

void
TAO_Notify_MT_Worker_Task::update_admin (TAO_Notify_AdminProperties& admin)
{
  this->buffering_strategy_->max_queue_length (admin.max_queue_length ());
}

void
TAO_Notify_MT_Worker_Task::update_qos (TAO_Notify_QoSAdmin_i& qos_admin)
{
  // Only set values on the buffering_strategy_ that have actually been
  // set on the qos_admin that is passed in.  This way, values on the
  // buffering_strategy_ are preserved when the qos parameters are not
  // set on say the event channel or the supplier proxy.
  ACE_TRY_NEW_ENV
    {
      CosNotification::QoSProperties_var qos = qos_admin.get_qos (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      for (CORBA::ULong index = 0; index < qos->length (); ++index)
        {
          if (ACE_OS::strcmp (qos[index].name,
                              CosNotification::OrderPolicy) == 0)
            {
              CORBA::Short value;
              qos[index].value >>= value;
              this->buffering_strategy_->order_policy (value);
            }
          else if (ACE_OS::strcmp (qos[index].name,
                                   CosNotification::DiscardPolicy) == 0)
            {
              CORBA::Short value;
              qos[index].value >>= value;
              this->buffering_strategy_->discard_policy (value);
            }
          else if (ACE_OS::strcmp (qos[index].name,
                                   CosNotification::MaxEventsPerConsumer) == 0)
            {
              CORBA::Long value;
              qos[index].value >>= value;
              this->buffering_strategy_->max_events_per_consumer (value);
            }
          else if (ACE_OS::strcmp (qos[index].name,
                                   TAO_Notify_Extensions::BlockingPolicy) == 0)
            {
              TimeBase::TimeT value;
              qos[index].value >>= value;
              this->buffering_strategy_->blocking_timeout (value);
            }
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "EC (%P|%t) exception in update_qos");
    }
  ACE_ENDTRY;
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
            ACE_dynamic_cast (TAO_Notify_Command*, mb);

          int result = 0;

          if (command != 0)
            {
              result = command->execute (ACE_ENV_SINGLE_ARG_PARAMETER);
              ACE_TRY_CHECK;
            }

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
TAO_Notify_Shutdown_Command::execute (ACE_ENV_SINGLE_ARG_DECL_NOT_USED /*ACE_ENV_SINGLE_ARG_PARAMETER*/)
{
  return -1;
}
/**************************************************************************/
