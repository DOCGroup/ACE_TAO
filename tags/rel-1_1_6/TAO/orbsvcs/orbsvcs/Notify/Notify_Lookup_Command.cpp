// $Id$

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Notify_Lookup_Command.h"
#include "Notify_Event_Manager.h"
#include "Notify_Listener_Filter_Eval_Command.h"
#include "Notify_Worker_Task.h"
#include "Notify_Event.h"
#include "Notify_Event_Map.h"

ACE_RCSID(Notify, Notify_Lookup_Command, "$Id$")

TAO_Notify_Lookup_Command::TAO_Notify_Lookup_Command (TAO_Notify_Event* event)
  :event_ (event)
{
  this->event_->_incr_refcnt ();
}

TAO_Notify_Lookup_Command::~TAO_Notify_Lookup_Command ()
{
  this->event_->_decr_refcnt ();
}

int
TAO_Notify_Lookup_Command::execute (TAO_Notify_Worker_Task* parent_task, CORBA::Environment& ACE_TRY_ENV)
{
  this->parent_task_ = parent_task;
  // There better be a next task and of type TAO_Notify_Worker_Task !
  ACE_ASSERT (parent_task->next () != 0);
  this->next_task_ = ACE_static_cast (TAO_Notify_Worker_Task*,
                                      parent_task->next());

  // If the event is *not* the special event
  // send it to the list that matches it.
  // In any case send it to the default list.
#if 0
  ACE_DEBUG ((LM_DEBUG, "finding a match for event: %s, %s\n",
              event->event_type ().event_type_.domain_name.in (),
              event->event_type ().event_type_.type_name.in ()));
#endif

  TAO_Notify_Event_Map* event_map =
    this->parent_task_->event_manager ()->event_map ();

  if (!this->event_->is_special_event_type ())
    {
      TAO_Notify_EventListener_List* listener_list;
      // find the subscription list for <event_type>

      if (event_map->find (this->event_->event_type (),
                           listener_list) == 0)
        {
          listener_list->for_each (this, ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);
        }
    }
  // Those subscribed for the default events get everything.
  // if (this->default_subscription_list_->is_empty () == 0)
  // @@ can't do this test - is_empty is not impl.
  {
    event_map->default_subscription_list ()->for_each (this, ACE_TRY_ENV);
    ACE_CHECK_RETURN (-1);
  }

  return 0;
}

void
TAO_Notify_Lookup_Command::work (TAO_Notify_EventListener* event_listener,
                                  CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Listener_Filter_Eval_Command* mb =
    new TAO_Notify_Listener_Filter_Eval_Command (this->event_, event_listener, 1);
  // Notr the last parameter, we want the parent filter to be evaluated.

  this->next_task_->process_event (mb, ACE_TRY_ENV);
}
