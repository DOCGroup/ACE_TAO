// $Id$

#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"

#include "DSRT_Dispatcher_Impl.h"
#include "DSRT_Schedulers.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Dispatcher_Impl, "$Id$")

  //using namespace Kokyu;

namespace Kokyu
{

int
DSRT_Dispatcher_Impl::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_DSRT_Dispatcher_Impl);
}

DSRT_Scheduler_Impl*
DSRT_Dispatcher_Impl::create_scheduler (DSRT_Sched_t sched_type,
                                        ACE_Sched_Params::Policy sched_policy,
                                        int sched_scope)
{
  DSRT_Scheduler_Impl* impl;

  switch (sched_type)
    {
    case SCHED_MIF:
      ACE_NEW_RETURN (impl, MIF_Scheduler_Impl (sched_policy, sched_scope), 0);
      break;

    default:
      break;
    }

  return impl;
}

int
DSRT_Dispatcher_Impl::init (int argc, ACE_TCHAR* argv[])
{
  ACE_Sched_Params::Policy sched_policy = ACE_SCHED_FIFO;
  int sched_scope = ACE_SCOPE_THREAD;
  DSRT_Sched_t sched_type = SCHED_MIF;

  ACE_Arg_Shifter arg_shifter (argc, argv);

  ACE_DEBUG ((LM_DEBUG, "svc::init called\n"));

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR* arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-schedtype")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("MIF")) == 0)
                {
                  sched_type = SCHED_MIF;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("EDF")) == 0)
                {
                  sched_type = SCHED_EDF;
                  if (arg_shifter.is_parameter_next ())
                    {
                      //const ACE_TCHAR* opt = arg_shifter.get_current ();
                    }
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_LIB_TEXT("DSRT_Dispatcher_Impl - ")
                              ACE_LIB_TEXT("unsupported sched type <%s>\n"),
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-sched_policy")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("FIFO")) == 0)
                {
                  sched_policy = ACE_SCHED_FIFO;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("OTHER")) == 0)
                {
                  sched_policy = ACE_SCHED_OTHER;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_LIB_TEXT("-sched_type ")
                              ACE_LIB_TEXT("unsupported sched type <%s>\n"),
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-sched_scope")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("THREAD")) == 0)
                {
                  sched_scope = ACE_SCOPE_THREAD;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("PROCESS")) == 0)
                {
                  sched_scope = ACE_SCOPE_PROCESS;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_LIB_TEXT("-sched_scope ")
                              ACE_LIB_TEXT("unsupported sched scope <%s>\n"),
                              opt));
                }
              arg_shifter.consume_arg ();
            }
        }
    }

  this->scheduler_impl_ = create_scheduler (sched_type, sched_policy, sched_scope);

  return 0;
}

int
DSRT_Dispatcher_Impl::fini (void)
{
  return 0;
}

int
DSRT_Dispatcher_Impl::init_i (const DSRT_ConfigInfo& config_info)
{
  //create and init the dispatcher tasks here
  ACE_DEBUG ((LM_DEBUG, "entering init_i\n" ));

  //  ACE_NEW_RETURN (this->task_, DSRT_Dispatcher_Task (*config), -1);

  //this->activate ();

  curr_config_info_ = config_info;
  return 0;
}

  /************************
int
DSRT_Dispatcher_Impl::activate ()
{
  long flags = THR_BOUND | THR_SCHED_FIFO;

  Priority_t priority =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO,
                                    ACE_SCOPE_THREAD);

  if (this->task_->activate (flags, 1, 1, priority) == -1)
    {
      flags = THR_BOUND;
      priority = ACE_Sched_Params::priority_max (ACE_SCHED_OTHER,
                                                 ACE_SCOPE_THREAD);
      if (this->task_->activate (flags, 1, 1, priority) == -1)
        ACE_ERROR ((LM_ERROR,
                        "EC (%P|%t) cannot activate queue %d", i));
    }
  return 0;
}
*********************************/

int
DSRT_Dispatcher_Impl::schedule_i (guid_t guid,
                  const QoSDescriptor& qos_info)
{
  ACE_UNUSED_ARG (guid);

  Priority_t prio = this->scheduler_impl_->schedule (guid, qos_info);
  ACE_OS::thr_setprio (ACE_OS::thr_self (), prio);

  return 0;
}

int
DSRT_Dispatcher_Impl::update_schedule_i (guid_t guid,
                  const QoSDescriptor& qos_info)
{
  ACE_UNUSED_ARG (guid);
  ACE_UNUSED_ARG (qos_info);
  return 0;
}

int
DSRT_Dispatcher_Impl::cancel_schedule_i (guid_t guid,
                  const QoSDescriptor& qos_info)
{
  return schedule_i (guid, qos_info);
}

}

// ****************************************************************

ACE_STATIC_SVC_DEFINE (DSRT_Dispatcher_Impl,
                       ACE_TEXT ("DSRT_Dispatcher_Impl"),
                       ACE_SVC_OBJ_T,
                       &ACE_SVC_NAME (DSRT_Dispatcher_Impl),
                       ACE_Service_Type::DELETE_THIS |
                       ACE_Service_Type::DELETE_OBJ,
                       0)
//ACE_FACTORY_DEFINE (Kokyu, DSRT_Dispatcher_Impl)

void _gobble_DSRT_Dispatcher_Impl (void *p) { \
  ACE_Service_Object *_p = ACE_static_cast (ACE_Service_Object *, p);
  ACE_ASSERT (_p != 0);
  delete _p; }
extern "C" Kokyu_Export ACE_Service_Object *
_make_DSRT_Dispatcher_Impl (ACE_Service_Object_Exterminator *gobbler)
{
  if (gobbler != 0)
    *gobbler = (ACE_Service_Object_Exterminator) _gobble_DSRT_Dispatcher_Impl;
  ACE_DEBUG ((LM_DEBUG, "about to create DSRT_Dispatcher_Impl\n" ));
  return new Kokyu::DSRT_Dispatcher_Impl;
}

// ****************************************************************
