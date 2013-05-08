// $Id$

#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Log_Macros.h"
#include "orbsvcs/Event/EC_Kokyu_Factory.h"
#include "orbsvcs/Event/EC_Kokyu_Dispatching.h"
#include "orbsvcs/Event/EC_Kokyu_Scheduling.h"
#include "orbsvcs/Event/EC_Kokyu_Filter_Builder.h"
#include "orbsvcs/Event/EC_Event_Channel_Base.h"
#include "orbsvcs/RtecSchedulerC.h"

#include "ace/Arg_Shifter.h"
#include "ace/Sched_Params.h"
#include "ace/OS_NS_strings.h"

#if ! defined (__ACE_INLINE__)
#include "orbsvcs/Event/EC_Kokyu_Factory.inl"
#endif /* __ACE_INLINE__ */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_EC_Kokyu_Factory::~TAO_EC_Kokyu_Factory (void)
{
}

int
TAO_EC_Kokyu_Factory::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_TAO_EC_Kokyu_Factory);
}

int
TAO_EC_Kokyu_Factory::init (int argc, ACE_TCHAR* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR* arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECDispatching")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                {
                  this->dispatching_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("mt")) == 0)
                {
                  this->dispatching_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("kokyu")) == 0)
                {
                  this->dispatching_ = 2;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              ACE_TEXT("EC_Kokyu_Factory - ")
                              ACE_TEXT("unsupported dispatching <%s>\n"),
                              opt));
                }
              arg_shifter.consume_arg ();
            }
          //if Kokyu dispatching - look for sched policy
          if (this->dispatching_ == 2)
            {
              if (arg_shifter.is_parameter_next ())
                {
                  const ACE_TCHAR* opt = arg_shifter.get_current ();
                  if (ACE_OS::strcasecmp (opt, ACE_TEXT("SCHED_FIFO")) == 0)
                    {
                      this->disp_sched_policy_ = ACE_SCHED_FIFO;
                    }
                  else if (ACE_OS::strcasecmp (opt, ACE_TEXT("SCHED_RR")) == 0)
                    {
                      this->disp_sched_policy_ = ACE_SCHED_RR;
                    }
                  else
                    {
                      this->disp_sched_policy_ = ACE_SCHED_OTHER;
                    }
                  arg_shifter.consume_arg ();

                  if (arg_shifter.is_parameter_next ())
                    {
                      const ACE_TCHAR* opt = arg_shifter.get_current ();
                      if (ACE_OS::strcasecmp (opt, ACE_TEXT("SYSTEM")) == 0)
                        {
                          this->disp_sched_scope_ = ACE_SCOPE_THREAD;
                        }
                      else if (ACE_OS::strcasecmp (opt, ACE_TEXT("PROCESS")) == 0)
                        {
                          this->disp_sched_scope_ = ACE_SCOPE_PROCESS;
                        }
                      arg_shifter.consume_arg ();
                    }
                }
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECFiltering")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                {
                  this->filtering_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("basic")) == 0)
                {
                  this->filtering_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("prefix")) == 0)
                {
                  this->filtering_ = 2;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("kokyu")) == 0)
                {
                  this->filtering_ = 3;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              ACE_TEXT("EC_Kokyu_Factory - ")
                              ACE_TEXT("unsupported filtering <%s>\n"),
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECTimeout")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("reactive")) == 0)
                {
                  this->timeout_ = 0;
                }
#if 0
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("kokyu")) == 0)
                {
                  this->timeout_ = 1;
                }
#endif /* 0 */
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              ACE_TEXT("EC_Kokyu_Factory - ")
                              ACE_TEXT("unsupported timeout <%s>\n"),
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }

      else if (ACE_OS::strcasecmp (arg, ACE_TEXT("-ECScheduling")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_TEXT("null")) == 0)
                {
                  this->scheduling_ = 0;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("group")) == 0)
                {
                  this->scheduling_ = 1;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_TEXT("kokyu")) == 0)
                {
                  this->scheduling_ = 2;
                }
              else
                {
                  ORBSVCS_ERROR ((LM_ERROR,
                              ACE_TEXT("EC_Kokyu_Factory - ")
                              ACE_TEXT("unsupported scheduling <%s>\n"),
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
TAO_EC_Kokyu_Factory::fini (void)
{
  return 0;
}

// ****************************************************************

TAO_EC_Dispatching*
TAO_EC_Kokyu_Factory::create_dispatching (TAO_EC_Event_Channel_Base *ec)
{
  if (this->dispatching_ == 2)
    return new TAO_EC_Kokyu_Dispatching (ec,
                                         this->disp_sched_policy_,
                                         this->disp_sched_scope_);
  return this->TAO_EC_Default_Factory::create_dispatching (ec);
}

TAO_EC_Filter_Builder*
TAO_EC_Kokyu_Factory::create_filter_builder (TAO_EC_Event_Channel_Base *ec)
{
  if (this->filtering_ == 3)
    return new TAO_EC_Kokyu_Filter_Builder (ec);

  return this->TAO_EC_Default_Factory::create_filter_builder (ec);
}


TAO_EC_Timeout_Generator*
TAO_EC_Kokyu_Factory::create_timeout_generator (TAO_EC_Event_Channel_Base *ec)
{
#if 0
  if (this->timeout_ == 1)
    {
      //Kokyu timeout generator?
    }
#endif
  return this->TAO_EC_Default_Factory::create_timeout_generator (ec);
}

TAO_EC_Scheduling_Strategy*
TAO_EC_Kokyu_Factory::create_scheduling_strategy (TAO_EC_Event_Channel_Base* ec)
{
  if (this->scheduling_ == 2)
    {
      CORBA::Object_var tmp = ec->scheduler ();
      RtecScheduler::Scheduler_var scheduler =
        RtecScheduler::Scheduler::_narrow (tmp.in ());
      return new TAO_EC_Kokyu_Scheduling (scheduler.in ());
    }
  return this->TAO_EC_Default_Factory::create_scheduling_strategy (ec);
}

TAO_END_VERSIONED_NAMESPACE_DECL

// ****************************************************************

ACE_STATIC_SVC_DEFINE (TAO_EC_Kokyu_Factory,
                       ACE_TEXT ("EC_Factory"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_EC_Kokyu_Factory),
                       ACE_Service_Type::DELETE_THIS | ACE_Service_Type::DELETE_OBJ,
                       0)
ACE_FACTORY_DEFINE (TAO_RTKokyuEvent, TAO_EC_Kokyu_Factory)
