// $Id$

#include "orbsvcs/ESF/ESF_Proxy_Collection.h"
#include "Notify_Lookup_Command.h"
#include "Notify_Event_Manager.h"
#include "Notify_Listener_Filter_Eval_Command.h"
#include "Notify_Event.h"
#include "Notify_Event_Map.h"
#include "Notify_Event_Processor.h"

ACE_RCSID(Notify, Notify_Lookup_Command, "$Id$")

TAO_Notify_Lookup_Command::TAO_Notify_Lookup_Command (TAO_Notify_Event_Processor* event_processor, TAO_Notify_Event* event, TAO_Notify_Event_Map* event_map)
  :TAO_Notify_Command (event_processor, event),
   event_map_ (event_map)
{
  this->event_->_incr_refcnt ();
}

TAO_Notify_Lookup_Command::~TAO_Notify_Lookup_Command ()
{
  this->event_->_decr_refcnt ();
}

int
TAO_Notify_Lookup_Command::execute (TAO_ENV_SINGLE_ARG_DECL)
{
  // If the event is *not* the special event
  // send it to the list that matches it.
  // In any case send it to the default list.
#if 0
  ACE_DEBUG ((LM_DEBUG, "finding a match for event: %s, %s\n",
              event->event_type ().event_type_.domain_name.in (),
              event->event_type ().event_type_.type_name.in ()));
#endif

  if (!this->event_->is_special_event_type ())
    {
      TAO_Notify_EventListener_List* listener_list;
      // find the subscription list for <event_type>

      if (event_map_->find (this->event_->event_type (),
                           listener_list) == 0)
        {
          listener_list->for_each (this TAO_ENV_ARG_PARAMETER);
          ACE_CHECK_RETURN (-1);
        }
    }
  // Those subscribed for the default events get everything.
  // if (this->default_subscription_list_->is_empty () == 0)
  // @@ can't do this test - is_empty is not impl.
  {
    event_map_->default_subscription_list ()->for_each (this TAO_ENV_ARG_PARAMETER);
    ACE_CHECK_RETURN (-1);
  }

  return 0;
}

void
TAO_Notify_Lookup_Command::work (TAO_Notify_EventListener* event_listener
                                  TAO_ENV_ARG_DECL)
{
  this->event_processor_->evaluate_listener_filter (this->event_, event_listener, 1 TAO_ENV_ARG_PARAMETER);
  // Note the last parameter, we want the parent filter to be evaluated.
}
