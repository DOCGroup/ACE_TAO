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
#include "Notify_MT_Worker_Task.h"

#include "tao/debug.h"

ACE_RCSID(Notify, Notify_Default_EMO_Factory, "$Id$")

#define ASYNCH_UPDATES_THREADS 1
  // The number of threads to send subscription/publication updates.
  // As this is a relatively adminstrative function. let's just use 1
  // thread to send the updates asynchronously.


  TAO_EMO_Options::TAO_EMO_Options (void)
    : mt_dispatching_ (0),
      mt_source_eval_ (0),
      mt_lookup_ (0),
      mt_listener_eval_ (0),
      asynch_updates_ (0),
      alloc_task_per_proxy_ (0),
      dispatching_threads_ (1),
      source_threads_ (1),
      lookup_threads_ (1),
      listener_threads_ (1)
{
}

int
TAO_Notify_Default_EMO_Factory::init_svc (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_Notify_Default_EMO_Factory);
}

TAO_Notify_Default_EMO_Factory::TAO_Notify_Default_EMO_Factory (void)
  :prealloc_source_eval_task_ (0),
   prealloc_listener_eval_task_ (0),
   prealloc_dispatching_task_ (0)
{
}

TAO_Notify_Default_EMO_Factory::~TAO_Notify_Default_EMO_Factory ()
{
  delete prealloc_source_eval_task_;
  delete prealloc_listener_eval_task_;
  delete prealloc_dispatching_task_;
}

int
TAO_Notify_Default_EMO_Factory::init (int argc, char* argv[])
{
  // ACE_DEBUG ((LM_DEBUG, "TAO_Notify_Default_EMO_Factory::init\n"));

  ACE_Arg_Shifter arg_shifter (argc, argv);

  const char *current_arg = 0;
  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, "-MTDispatching") == 0)
        {
          EMO_OPTIONS::instance ()->mt_dispatching_ = 1;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-DispatchingThreads")))
        {
          EMO_OPTIONS::instance ()->dispatching_threads_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcasecmp (arg, "-MTSourceEval") == 0)
        {
          EMO_OPTIONS::instance ()->mt_source_eval_ = 1;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-SourceThreads")))
        {
          EMO_OPTIONS::instance ()->source_threads_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcasecmp (arg, "-MTLookup") == 0)
        {
          EMO_OPTIONS::instance ()->mt_lookup_ = 1;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-LookupThreads")))
        {
          EMO_OPTIONS::instance ()->lookup_threads_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcasecmp (arg, "-MTListenerEval") == 0)
        {
          EMO_OPTIONS::instance ()->mt_listener_eval_ = 1;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-ListenerThreads")))
        {
          EMO_OPTIONS::instance ()->listener_threads_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcasecmp (arg, "-AsynchUpdates") == 0)
        {
          EMO_OPTIONS::instance ()->asynch_updates_ = 1;
          arg_shifter.consume_arg ();
        }
      else if (ACE_OS::strcasecmp (arg, "-AllocateTaskperProxy") == 0)
        {
          EMO_OPTIONS::instance ()->alloc_task_per_proxy_ = 1;
          arg_shifter.consume_arg ();
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG,"EMO Factory did not understand %s",arg));
          arg_shifter.ignore_arg ();
        }
    }

  return 0;
}

TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_task (int mt, int tp_size ACE_ENV_ARG_DECL)
{
  TAO_Notify_Worker_Task* task;

  int threads_flags =
    THR_SCHED_DEFAULT|THR_BOUND|THR_NEW_LWP;

  //  int dispatching_threads_priority

  // Later: give the user options to specify threads flags and thread priority for each task.

  if (mt == 1)
    ACE_NEW_THROW_EX (task, TAO_Notify_MT_Worker_Task (tp_size,
                                                       threads_flags,
                                                       0),
                      CORBA::NO_MEMORY ());
  else
    ACE_NEW_THROW_EX (task,
                      TAO_Notify_Worker_Task (),
                      CORBA::NO_MEMORY ());
  return task;
}


int
TAO_Notify_Default_EMO_Factory::init_instance (void)
{
  if (EMO_OPTIONS::instance ()->alloc_task_per_proxy_ == 0) // preallocate all tasks.
    {
      return this->preallocate_tasks ();
    }
  return 0;
}

int
TAO_Notify_Default_EMO_Factory::preallocate_tasks (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;

  this->prealloc_source_eval_task_ =
    create_task (EMO_OPTIONS::instance ()->mt_source_eval_, EMO_OPTIONS::instance ()->source_threads_
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->prealloc_listener_eval_task_ =
    create_task (EMO_OPTIONS::instance ()->mt_listener_eval_, EMO_OPTIONS::instance ()->listener_threads_
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->prealloc_dispatching_task_ =
    create_task (EMO_OPTIONS::instance ()->mt_dispatching_, EMO_OPTIONS::instance ()->dispatching_threads_
                 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
TAO_Notify_Default_EMO_Factory::fini (void)
{
  //  ACE_DEBUG ((LM_DEBUG, "TAO_Notify_Default_EMO_Factory::fini\n"));
  return 0;
}

TAO_Notify_Event_Manager*
TAO_Notify_Default_EMO_Factory::create_event_manager (TAO_Notify_EventChannel_i* channel ACE_ENV_ARG_DECL)
{
  TAO_Notify_Event_Manager* event_manager;
  ACE_NEW_THROW_EX (event_manager,
                    TAO_Notify_Event_Manager (channel, this),
                    CORBA::NO_MEMORY ());
  return event_manager;
}

TAO_Notify_Event_Map*
TAO_Notify_Default_EMO_Factory::create_event_map (ACE_ENV_SINGLE_ARG_DECL)
{
  TAO_Notify_Event_Map* event_map;
  ACE_NEW_THROW_EX (event_map,
                    TAO_Notify_Event_Map (this),
                    CORBA::NO_MEMORY ());
  return event_map;
}

TAO_Notify_Event_Processor*
TAO_Notify_Default_EMO_Factory::create_event_processor (TAO_Notify_Event_Manager* event_manager ACE_ENV_ARG_DECL)
{
  TAO_Notify_Event_Processor* event_processor;
  ACE_NEW_THROW_EX (event_processor,
                    TAO_Notify_Event_Processor (event_manager),
                    CORBA::NO_MEMORY ());
  return event_processor;
}


TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_source_eval_task (ACE_ENV_SINGLE_ARG_DECL)
{
  if (EMO_OPTIONS::instance ()->alloc_task_per_proxy_ == 1)
    return create_task (EMO_OPTIONS::instance ()->mt_source_eval_,
                        EMO_OPTIONS::instance ()->source_threads_
                        ACE_ENV_ARG_PARAMETER);
  else
    return prealloc_source_eval_task_;
}

TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_lookup_task (ACE_ENV_SINGLE_ARG_DECL)
{
  return create_task (EMO_OPTIONS::instance ()->mt_lookup_,
                      EMO_OPTIONS::instance ()->lookup_threads_ ACE_ENV_ARG_PARAMETER);
}

TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_listener_eval_task (ACE_ENV_SINGLE_ARG_DECL)
{
  if (EMO_OPTIONS::instance ()->alloc_task_per_proxy_ == 1)
    return create_task (EMO_OPTIONS::instance ()->mt_listener_eval_,
                        EMO_OPTIONS::instance ()->listener_threads_
                        ACE_ENV_ARG_PARAMETER);
  else
    return prealloc_listener_eval_task_;
}

TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_dispatching_task (ACE_ENV_SINGLE_ARG_DECL)
{
  if (EMO_OPTIONS::instance ()->alloc_task_per_proxy_ == 1)
    return create_task (EMO_OPTIONS::instance ()->mt_dispatching_,
                        EMO_OPTIONS::instance ()->dispatching_threads_
                        ACE_ENV_ARG_PARAMETER);
  else
    return prealloc_dispatching_task_;
}

TAO_Notify_Worker_Task*
TAO_Notify_Default_EMO_Factory::create_updates_task (ACE_ENV_SINGLE_ARG_DECL)
{
  return create_task (EMO_OPTIONS::instance ()->asynch_updates_, ASYNCH_UPDATES_THREADS ACE_ENV_ARG_PARAMETER);
}

void
TAO_Notify_Default_EMO_Factory::destroy_source_eval_task (TAO_Notify_Worker_Task* task)
{
  if (EMO_OPTIONS::instance ()->alloc_task_per_proxy_ == 1)
    delete task;
}

void
TAO_Notify_Default_EMO_Factory::destroy_listener_eval_task (TAO_Notify_Worker_Task* task)
{
  if (EMO_OPTIONS::instance ()->alloc_task_per_proxy_ == 1)
    delete task;
}

void
TAO_Notify_Default_EMO_Factory::destroy_dispatching_task (TAO_Notify_Worker_Task* task)
{
  if (EMO_OPTIONS::instance ()->alloc_task_per_proxy_ == 1)
    delete task;
}

void
TAO_Notify_Default_EMO_Factory::destroy_lookup_task (TAO_Notify_Worker_Task* task)
{
  delete task;
}

void
TAO_Notify_Default_EMO_Factory::destroy_updates_task (TAO_Notify_Worker_Task* task)
{
  delete task;
}


void
TAO_Notify_Default_EMO_Factory::print_values (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG,
                "EMO Factory = "
                "mt_dispatching %d "
                "mt_source_eval %d "
                " mt_lookup %d "
                " mt_listener_eval %d"
                " dispatching_threads %d "
                " source_threads %d "
                " lookup_threads %d "
                " listener_threads_ %d "
                " AsynchUpdates %d"
                " AllocateTaskperProxy %d",
                EMO_OPTIONS::instance ()->mt_dispatching_,
                EMO_OPTIONS::instance ()->mt_source_eval_,
                EMO_OPTIONS::instance ()->mt_lookup_,
                EMO_OPTIONS::instance ()->mt_listener_eval_,
                EMO_OPTIONS::instance ()->dispatching_threads_,
                EMO_OPTIONS::instance ()->source_threads_,
                EMO_OPTIONS::instance ()->lookup_threads_,
                EMO_OPTIONS::instance ()->listener_threads_,
                EMO_OPTIONS::instance ()->asynch_updates_,
                EMO_OPTIONS::instance ()->alloc_task_per_proxy_
              ));
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

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
template class ACE_Singleton<TAO_EMO_Options, ACE_Null_Mutex>;
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#pragma instantiate ACE_Singleton<TAO_EMO_Options, ACE_Null_Mutex>
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
