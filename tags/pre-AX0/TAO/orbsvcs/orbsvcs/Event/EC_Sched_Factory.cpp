// $Id$

#include "EC_Sched_Factory.h"
#include "EC_Priority_Dispatching.h"
#include "EC_Priority_Scheduling.h"
#include "EC_Sched_Filter_Builder.h"
#include "EC_Event_Channel.h"
#include "orbsvcs/RtecSchedulerC.h"

#include "ace/Arg_Shifter.h"
#include "ace/Sched_Params.h"

#if ! defined (__ACE_INLINE__)
#include "EC_Sched_Factory.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Event, EC_Sched_Factory, "$Id$")

TAO_EC_Sched_Factory::~TAO_EC_Sched_Factory (void)
{
}

int
TAO_EC_Sched_Factory::init (int argc, char* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const char *arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, "-ECDispatching") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "reactive") == 0)
                {
                  this->dispatching_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "mt") == 0)
                {
                  this->dispatching_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "priority") == 0)
                {
                  this->dispatching_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Sched_Factory - "
                              "unsupported dispatching <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECFiltering") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->filtering_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "basic") == 0)
                {
                  this->filtering_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "prefix") == 0)
                {
                  this->filtering_ = 2;
                }
              else if (ACE_OS::strcasecmp (opt, "priority") == 0)
                {
                  this->filtering_ = 3;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Sched_Factory - "
                              "unsupported filtering <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECTimeout") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "reactive") == 0)
                {
                  this->timeout_ = 0;
                }
#if 0
              else if (ACE_OS::strcasecmp (opt, "priority") == 0)
                {
                  this->timeout_ = 1;
                }
#endif /* 0 */
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Sched_Factory - "
                              "unsupported timeout <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, "-ECScheduling") == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const char* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, "null") == 0)
                {
                  this->scheduling_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, "group") == 0)
                {
                  this->scheduling_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, "priority") == 0)
                {
                  this->scheduling_ = 2;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              "EC_Sched_Factory - "
                              "unsupported scheduling <%s>\n",
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else
        {
          arg_shifter.ignore_arg ();
        }
    }
  return this->TAO_EC_Default_Factory::init (argc, argv);
}

int
TAO_EC_Sched_Factory::fini (void)
{
  return 0;
}

// ****************************************************************

TAO_EC_Dispatching*
TAO_EC_Sched_Factory::create_dispatching (TAO_EC_Event_Channel *ec)
{
  if (this->dispatching_ == 2)
    return new TAO_EC_Priority_Dispatching (ec);
  return this->TAO_EC_Default_Factory::create_dispatching (ec);
}

TAO_EC_Filter_Builder*
TAO_EC_Sched_Factory::create_filter_builder (TAO_EC_Event_Channel *ec)
{
  if (this->filtering_ == 3)
    return new TAO_EC_Sched_Filter_Builder (ec);

  return this->TAO_EC_Default_Factory::create_filter_builder (ec);
}


TAO_EC_Timeout_Generator*
TAO_EC_Sched_Factory::create_timeout_generator (TAO_EC_Event_Channel *ec)
{
#if 0
  if (this->timeout_ == 1)
    {
    }
#endif
  return this->TAO_EC_Default_Factory::create_timeout_generator (ec);
}

TAO_EC_Scheduling_Strategy*
TAO_EC_Sched_Factory::create_scheduling_strategy (TAO_EC_Event_Channel* ec)
{
  if (this->scheduling_ == 2)
    {
      CORBA::Object_var tmp = ec->scheduler ();
      RtecScheduler::Scheduler_var scheduler =
        RtecScheduler::Scheduler::_narrow (tmp.in ());
      return new TAO_EC_Priority_Scheduling (scheduler.in ());
    }
  return this->TAO_EC_Sched_Factory::create_scheduling_strategy (ec);
}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_EC_Sched_Factory,
                       ACE_TEXT ("EC_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_Sched_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTSchedEvent, TAO_EC_Sched_Factory)

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
