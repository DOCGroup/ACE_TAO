// $Id$

#include "Notify_Event_Processor.h"
#include "Notify_Worker_Task.h"
#include "Notify_Source_Filter_Eval_Command.h"
#include "Notify_Factory.h"
#include "Notify_Event_Manager_Objects_Factory.h"

ACE_RCSID(Notify, Notify_Event_Processor, "$Id$")

#define NOTIFY_EVENT_PROCESSING_STAGES 4

#define NOTIFY_SOURCE_FILTER_EVAL_STAGE   0
#define NOTIFY_LOOKUP_STAGE               1
#define NOTIFY_LISTENER_FILTER_EVAL_STAGE 2
#define NOTIFY_DISPATCHING_STAGE          3

TAO_Notify_Event_Processor::TAO_Notify_Event_Processor (TAO_Notify_Event_Manager* event_manager)
  :event_manager_ (event_manager),
   first_task_ (0),
   listener_filter_eval_task_ (0),
   dispatching_task_ (0),
   emo_factory_ (0)
{
}

TAO_Notify_Event_Processor::~TAO_Notify_Event_Processor ()
{
}

void
TAO_Notify_Event_Processor::init (CORBA::Environment& ACE_TRY_ENV)
{
  this->emo_factory_ =
    TAO_Notify_Factory::get_event_manager_objects_factory ();

  // = Create the tasks.
  TAO_Notify_Worker_Task* tasks[NOTIFY_EVENT_PROCESSING_STAGES];

  tasks[NOTIFY_SOURCE_FILTER_EVAL_STAGE] =
    this->emo_factory_->create_source_eval_task (this->event_manager_,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  tasks[NOTIFY_LOOKUP_STAGE] =
    this->emo_factory_->create_lookup_task (this->event_manager_,
                                                 ACE_TRY_ENV);
  ACE_CHECK;

  tasks[NOTIFY_LISTENER_FILTER_EVAL_STAGE] =
    this->emo_factory_->create_listener_eval_task (this->event_manager_,
                                                        ACE_TRY_ENV);
  ACE_CHECK;

  tasks[NOTIFY_DISPATCHING_STAGE] =
    this->emo_factory_->create_dispatching_task (this->event_manager_,
                                                      ACE_TRY_ENV);
  ACE_CHECK;

  // = Create the Modules.
  TAO_Notify_Module* modules[NOTIFY_EVENT_PROCESSING_STAGES];

  ACE_NEW_THROW_EX (modules[NOTIFY_SOURCE_FILTER_EVAL_STAGE],
                    TAO_Notify_Module ("1",
                                       tasks[NOTIFY_SOURCE_FILTER_EVAL_STAGE]),
                    CORBA::NO_MEMORY ());

  ACE_NEW_THROW_EX (modules[NOTIFY_LOOKUP_STAGE],
                    TAO_Notify_Module ("2",
                                       tasks[NOTIFY_LOOKUP_STAGE]),
                    CORBA::NO_MEMORY ());

  ACE_NEW_THROW_EX (modules[NOTIFY_LISTENER_FILTER_EVAL_STAGE],
                    TAO_Notify_Module ("3",
                                       tasks[NOTIFY_LISTENER_FILTER_EVAL_STAGE]),
                    CORBA::NO_MEMORY ());

  ACE_NEW_THROW_EX (modules[NOTIFY_DISPATCHING_STAGE],
                    TAO_Notify_Module ("4",
                                       tasks[NOTIFY_DISPATCHING_STAGE]),
                    CORBA::NO_MEMORY ());

  for (int index = NOTIFY_EVENT_PROCESSING_STAGES -1;
       index > -1; --index)
    // push modules backworks
    {
      if (this->processing_stream_.push (modules[index]) == -1)
        ACE_THROW (CORBA::INTERNAL ());
    }

  // set the first stream
  this->first_task_ = tasks[NOTIFY_SOURCE_FILTER_EVAL_STAGE];
  this->listener_filter_eval_task_ = tasks [NOTIFY_LISTENER_FILTER_EVAL_STAGE];
  this->dispatching_task_ = tasks [NOTIFY_DISPATCHING_STAGE];
}

TAO_Notify_Worker_Task*
TAO_Notify_Event_Processor::get_listener_filter_eval_task (void)
{
  return this->listener_filter_eval_task_;
}

TAO_Notify_Worker_Task*
TAO_Notify_Event_Processor::get_dispatching_task (void)
{
  return this->dispatching_task_;
}

void
TAO_Notify_Event_Processor::shutdown (CORBA::Environment &/*ACE_TRY_ENV*/)
{
  this->processing_stream_.close ();
  // this->first_task_->shutdown (ACE_TRY_ENV);
  // This will post a "shutdown" message to all linked tasks.
}

void
TAO_Notify_Event_Processor::process_event (TAO_Notify_Event* event, TAO_Notify_EventSource* event_source, CORBA::Environment& ACE_TRY_ENV)
{
  // TODO: use cache allocator here.
  TAO_Notify_Source_Filter_Eval_Command* mb =
    new TAO_Notify_Source_Filter_Eval_Command (event, event_source);

  this->first_task_->process_event (mb, ACE_TRY_ENV);
}
