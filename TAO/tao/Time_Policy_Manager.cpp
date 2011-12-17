// $Id$

#include "tao/Time_Policy_Manager.h"
#include "tao/ORB_Time_Policy.h"
#include "tao/debug.h"

#include "ace/Dynamic_Service.h"
#include "ace/OS_NS_string.h"
#include "ace/OS_NS_strings.h"

#if (TAO_HAS_TIME_POLICY == 1)

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

TAO_Time_Policy_Manager::TAO_Time_Policy_Manager ()
  : time_policy_strategy_ (0)
#if defined(TAO_USE_HR_TIME_POLICY_STRATEGY)
  , time_policy_setting_ (TAO_HR_TIME_POLICY)
#else
  , time_policy_setting_ (TAO_OS_TIME_POLICY)
#endif
{
}

TAO_Time_Policy_Manager::~TAO_Time_Policy_Manager ()
{
  TAO::ORB_Time_Policy::reset_time_policy ();
}

// = Service Configurator hooks.
/// Dynamic linking hook
int
TAO_Time_Policy_Manager::init (int argc, ACE_TCHAR* argv[])
{
  return this->parse_args (argc, argv);
}

/// Parse svc.conf arguments
int
TAO_Time_Policy_Manager::parse_args (int argc, ACE_TCHAR* argv[])
{
  ACE_TRACE ("TAO_Time_Policy_Manager::parse_args");

  int curarg;

  for (curarg = 0; curarg < argc && argv[curarg]; ++curarg)
    {
      if (ACE_OS::strcasecmp (argv[curarg],
                                   ACE_TEXT("-ORBTimePolicyStrategy")) == 0)
        {
          curarg++;
          if (curarg < argc)
            {
              ACE_TCHAR* name = argv[curarg];

              if (ACE_OS::strcasecmp (name,
                                      ACE_TEXT("OS")) == 0)
                this->time_policy_setting_ = TAO_OS_TIME_POLICY;
              else if (ACE_OS::strcasecmp (name,
                                           ACE_TEXT("HR")) == 0)
                this->time_policy_setting_ = TAO_HR_TIME_POLICY;
              else
                {
                  this->time_policy_setting_ = TAO_DYN_TIME_POLICY;
                  this->time_policy_name_ = ACE_TEXT_ALWAYS_CHAR (name);
                }
            }
        }
    }
  return 0;
}

ACE_Timer_Queue * TAO_Time_Policy_Manager::create_timer_queue (void)
{
  // locking scope
  {
    ACE_GUARD_RETURN (TAO_SYNCH_MUTEX,
                      monitor,
                      this->lock_,
                      0);

    // check if time policy strategy has already been initialized
    if (this->time_policy_strategy_ == 0)
      {
        // load strategy
        if (this->time_policy_setting_ == TAO_OS_TIME_POLICY)
          {
            this->time_policy_name_ = "TAO_SYSTEM_TIME_POLICY";
          }
        else if (this->time_policy_setting_ == TAO_HR_TIME_POLICY)
          {
            this->time_policy_name_ = "TAO_HR_TIME_POLICY";
          }
        this->time_policy_strategy_ =
            ACE_Dynamic_Service<TAO_Time_Policy_Strategy>::instance (
                this->time_policy_name_.c_str ());
        if (this->time_policy_strategy_ == 0)
          {
            ACE_ERROR ((LM_ERROR,
                        ACE_TEXT ("TAO (%P|%t) - TAO_Time_Policy_Manager: ")
                        ACE_TEXT ("FAILED to load time policy strategy '%C'\n"),
                        this->time_policy_name_.c_str ()));
            return 0;
          }

        if (TAO_debug_level > 1)
          {
            ACE_DEBUG ((LM_INFO,
                ACE_TEXT ("TAO (%P|%t) - TAO_Time_Policy_Manager: ")
                ACE_TEXT ("loaded time policy strategy '%C'\n"),
                this->time_policy_name_.c_str ()));
          }

        // handle one time initialization of ORB_Time_Policy
        TAO::ORB_Time_Policy::set_time_policy (
            this->time_policy_strategy_->get_time_policy ());
      }
  }

  return this->time_policy_strategy_->create_timer_queue ();
}

void
TAO_Time_Policy_Manager::destroy_timer_queue (ACE_Timer_Queue *tmq)
{
  // locking scope
  {
    ACE_GUARD (TAO_SYNCH_MUTEX,
                monitor,
                this->lock_);

    // check if time policy strategy has been initialized
    if (this->time_policy_strategy_ == 0)
      {
        return;
      }
  }

  this->time_policy_strategy_->destroy_timer_queue (tmq);
}


ACE_STATIC_SVC_DEFINE (TAO_Time_Policy_Manager,
                       ACE_TEXT ("Time_Policy_Manager"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (TAO_Time_Policy_Manager),
                       ACE_Service_Type::DELETE_THIS |
                                  ACE_Service_Type::DELETE_OBJ,
                       0)

ACE_FACTORY_DEFINE (TAO, TAO_Time_Policy_Manager)

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* TAO_HAS_TIME_POLICY */
