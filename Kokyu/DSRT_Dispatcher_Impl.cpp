// $Id$

#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"

#include "DSRT_Dispatcher_Impl.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Dispatcher_Impl.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, Dispatcher_Impl, "$Id$")

  //using namespace Kokyu;

namespace Kokyu
{

static Priority_t schedule_MIF (Importance_t imp)
{
  Priority_t min =
    ACE_Sched_Params::priority_min (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);

  Priority_t max =
    ACE_Sched_Params::priority_max (ACE_SCHED_FIFO, ACE_SCOPE_THREAD);

  switch (imp)
    {
    case VERY_LOW_IMPORTANCE:
      return min;
      break;

    case LOW_IMPORTANCE:
      return ACE_Sched_Params::next_priority
        (ACE_SCHED_FIFO, min, ACE_SCOPE_THREAD);
      break;

    case MEDIUM_IMPORTANCE:
      return ACE_Sched_Params::next_priority
        (ACE_SCHED_FIFO,
         ACE_Sched_Params::next_priority
         (ACE_SCHED_FIFO, min, ACE_SCOPE_THREAD),
         ACE_SCOPE_THREAD);
      break;

    case HIGH_IMPORTANCE:
      return ACE_Sched_Params::previous_priority
        (ACE_SCHED_FIFO, max, ACE_SCOPE_THREAD);
      break;

    case VERY_HIGH_IMPORTANCE:
      return max;
      break;

    default:
      break;
    }

  return 0;
}

int
DSRT_Dispatcher_Impl::init_svcs (void)
{
  return ACE_Service_Config::static_svcs ()->
    insert (&ace_svc_desc_DSRT_Dispatcher_Impl);
}

int
DSRT_Dispatcher_Impl::init (int argc, ACE_TCHAR* argv[])
{
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
                  this->sched_type_ = SCHED_MIF;
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

      else
        {
          arg_shifter.ignore_arg ();
        }
    }
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
  switch (this->sched_type_)
    {
    case SCHED_MIF:
      ACE_DEBUG ((LM_DEBUG, "(%t) request for MIF schedule\n"));
      Priority_t prio = schedule_MIF (qos_info.importance_);
      ACE_OS::thr_setprio (ACE_OS::thr_self (), prio);
      break;
    }
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
