// $Id$

#include "Notify_Event_Processor.h"
#include "Notify_Worker_Task.h"
#include "Notify_Source_Filter_Eval_Command.h"
#include "Notify_Listener_Filter_Eval_Command.h"
#include "Notify_Event_Manager_Objects_Factory.h"
#include "Notify_Lookup_Command.h"
#include "Notify_Event_Dispatch_Command.h"
#include "Notify_Factory.h"
#include "Notify_Listeners.h"
#include "Notify_Event_Manager.h"

ACE_RCSID(Notify, Notify_Event_Processor, "$Id$")

TAO_Notify_Event_Processor::TAO_Notify_Event_Processor (TAO_Notify_Event_Manager* event_manager)
  :event_manager_ (event_manager),
   lookup_task_ (0),
   emo_factory_ (0)
{
}

TAO_Notify_Event_Processor::~TAO_Notify_Event_Processor ()
{
  delete this->lookup_task_;
}

void
TAO_Notify_Event_Processor::init (CORBA::Environment& ACE_TRY_ENV)
{
  this->emo_factory_ =
    TAO_Notify_Factory::get_event_manager_objects_factory ();

  this->lookup_task_ = this->emo_factory_->create_lookup_task (ACE_TRY_ENV);
  ACE_CHECK;

  // Get hold of the admin properties.
  TAO_Notify_AdminProperties* const admin_properties =
    this->event_manager_->admin_properties ();

  this->lookup_task_->init_task (admin_properties);
}

void
TAO_Notify_Event_Processor::shutdown (CORBA::Environment & ACE_TRY_ENV)
{
  this->lookup_task_->shutdown (ACE_TRY_ENV);
}

void
TAO_Notify_Event_Processor::evaluate_source_filter (TAO_Notify_Event* event, TAO_Notify_EventSource* event_source, CORBA::Environment& ACE_TRY_ENV)
{
  // TODO: use cache allocator here.
  TAO_Notify_Source_Filter_Eval_Command* mb =
    new TAO_Notify_Source_Filter_Eval_Command (this, event, event_source);

  event_source->filter_eval_task ()->process_event (mb, ACE_TRY_ENV);
}

void
TAO_Notify_Event_Processor::lookup_subscriptions (TAO_Notify_Event* event, TAO_Notify_EventSource* /*event_source*/, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Lookup_Command* lookup =
    new TAO_Notify_Lookup_Command (this, event, this->event_manager_->event_map ());

  this->lookup_task_->process_event (lookup, ACE_TRY_ENV);
}

void
TAO_Notify_Event_Processor::evaluate_listener_filter (TAO_Notify_Event* event, TAO_Notify_EventListener* event_listener, CORBA::Boolean eval_parent, CORBA::Environment &ACE_TRY_ENV)
{
  // @@ Pradeep: you should use ACE_NEW here....
  // @@ Pradeep: do you really need to allocate this guy from the
  // heap? Maybe you can just allocate it from the stack and only if
  // somebody really wants to keep it around you make a copy?  The
  // idea is to save allocations in the good case.

  TAO_Notify_Listener_Filter_Eval_Command* mb =
    new TAO_Notify_Listener_Filter_Eval_Command (this, event, event_listener, eval_parent);

  event_listener->filter_eval_task ()->process_event (mb, ACE_TRY_ENV);
}

void
TAO_Notify_Event_Processor::dispatch_event (TAO_Notify_Event* event, TAO_Notify_EventListener* event_listener, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Event_Dispatch_Command* dispatch =
    new TAO_Notify_Event_Dispatch_Command (this, event, event_listener);

  event_listener->event_dispatch_task ()->process_event (dispatch, ACE_TRY_ENV);
}
