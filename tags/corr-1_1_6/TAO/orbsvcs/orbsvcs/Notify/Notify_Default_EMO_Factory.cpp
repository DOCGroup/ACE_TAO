// $Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "Notify_Default_EMO_Factory.h"
#include "Notify_Event_Manager.h"
#include "Notify_Event_Map.h"
#include "Notify_Event_Processor.h"
#include "Notify_Worker_Task.h"
#include "Notify_Factory.h"
#include "Notify_Event.h"

ACE_RCSID(Notify, Notify_Default_EMO_Factory, "$Id$")

int
TAO_Notify_Default_EMO_Factory::init_svc (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_Notify_Default_EMO_Factory);
}

TAO_Notify_Default_EMO_Factory::TAO_Notify_Default_EMO_Factory (void)
  : mt_dispatching_ (0),
    mt_source_eval_ (0),
    mt_lookup_ (0),
    mt_listener_eval_ (0)
{
}

TAO_Notify_Default_EMO_Factory::~TAO_Notify_Default_EMO_Factory ()
{
}

int
TAO_Notify_Default_EMO_Factory::init (int argc, char* argv[])
{
  ACE_DEBUG ((LM_DEBUG, "TAO_Notify_Default_EMO_Factory::init\n"));
  ACE_Arg_Shifter arg_shifter (argc, argv);


  while (arg_shifter.is_anything_left ())
    {
      char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, "-MTDispatching") == 0)
        {
          this->mt_dispatching_ = 1;
          arg_shifter.consume_arg ();
        }
      if (ACE_OS::strcasecmp (arg, "MTSourceEval") == 0)
        {
          this->mt_source_eval_ = 1;
          arg_shifter.consume_arg ();
        }
      if (ACE_OS::strcasecmp (arg, "-MTLookup") == 0)
        {
          this->mt_lookup_ = 1;
          arg_shifter.consume_arg ();
        }
      if (ACE_OS::strcasecmp (arg, "-MTListenerEval") == 0)
        {
          this->mt_listener_eval_ = 1;
          arg_shifter.consume_arg ();
        }
      else
        {
            arg_shifter.ignore_arg ();
        }
    }
  return 0;
}

int
TAO_Notify_Default_EMO_Factory::fini (void)
{
  ACE_DEBUG ((LM_DEBUG, "TAO_Notify_Default_EMO_Factory::fini\n"));
  return 0;
}

TAO_Notify_Event_Manager*
TAO_Notify_Default_EMO_Factory::create_event_manager (TAO_Notify_EventChannel_i* channel, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Event_Manager* event_manager;
  ACE_NEW_THROW_EX (event_manager,
                    TAO_Notify_Event_Manager (channel),
                    CORBA::NO_MEMORY ());
  return event_manager;
}

TAO_Notify_Event_Map*
TAO_Notify_Default_EMO_Factory::create_event_map (CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Event_Map* event_map;
  ACE_NEW_THROW_EX (event_map,
                    TAO_Notify_Event_Map (),
                    CORBA::NO_MEMORY ());
  return event_map;
}

TAO_Notify_Event_Processor*
TAO_Notify_Default_EMO_Factory::create_event_processor (TAO_Notify_Event_Manager* event_manager, CORBA::Environment &ACE_TRY_ENV)
{
  TAO_Notify_Event_Processor* event_processor;
  ACE_NEW_THROW_EX (event_processor,
                    TAO_Notify_Event_Processor (event_manager),
                    CORBA::NO_MEMORY ());
  return event_processor;
}


TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_source_eval_task (TAO_Notify_Event_Manager* event_manager, CORBA::Environment &ACE_TRY_ENV)
{
  // @@ pass the correct option to initialize this as passive/active object.
  TAO_Notify_Worker_Task* task;
  ACE_NEW_THROW_EX (task,
                    TAO_Notify_Worker_Task (event_manager,this->mt_source_eval_),
                    CORBA::NO_MEMORY ());
  return task;
}

TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_lookup_task (TAO_Notify_Event_Manager* event_manager, CORBA::Environment &ACE_TRY_ENV)
{
  // @@ pass the correct option to initialize this as passive/active object.
  TAO_Notify_Worker_Task* task;
  ACE_NEW_THROW_EX (task,
                    TAO_Notify_Worker_Task (event_manager,this->mt_lookup_),
                    CORBA::NO_MEMORY ());
  return task;
}

TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_listener_eval_task (TAO_Notify_Event_Manager* event_manager, CORBA::Environment &ACE_TRY_ENV)
{
  // @@ pass the correct option to initialize this as passive/active object.
  TAO_Notify_Worker_Task* task;
  ACE_NEW_THROW_EX (task,
                    TAO_Notify_Worker_Task (event_manager, this->mt_listener_eval_),
                    CORBA::NO_MEMORY ());
  return task;
}

TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_dispatching_task (TAO_Notify_Event_Manager* event_manager, CORBA::Environment &ACE_TRY_ENV)
{
  // @@ pass the correct option to initialize this as passive/active object.
  TAO_Notify_Worker_Task* task;
  ACE_NEW_THROW_EX (task,
                    TAO_Notify_Worker_Task (event_manager,
                                            this->mt_dispatching_),
                    CORBA::NO_MEMORY ());
  return task;
}


// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_Notify_Default_EMO_Factory,
                       ACE_TEXT (TAO_NOTIFY_DEF_EMO_FACTORY_NAME),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Notify_Default_EMO_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_Notify, TAO_Notify_Default_EMO_Factory)

// ****************************************************************
