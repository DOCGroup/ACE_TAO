// $Id$

#include "ace/Sched_Params.h"
#include "ace/Arg_Shifter.h"

#include "DSRT_Schedulers.h"

#if ! defined (__ACE_INLINE__)
#include "DSRT_Schedulers.i"
#endif /* __ACE_INLINE__ */

ACE_RCSID(Kokyu, DSRT_Schedulers, "$Id$")


namespace Kokyu
{
DSRT_Scheduler_Impl::DSRT_Scheduler_Impl ()
  :sched_policy_ (ACE_SCHED_FIFO),
   sched_scope_ (ACE_SCOPE_THREAD),
   min_prio_ (ACE_Sched_Params::priority_min (sched_policy_, sched_scope_)),
   max_prio_ (ACE_Sched_Params::priority_max (sched_policy_, sched_scope_))
{
}

int
DSRT_Scheduler_Impl::schedule (guid_t id, const QoSDescriptor& qos)
{
  return this->schedule_i (id, qos);
}

MIF_Scheduler_Impl::MIF_Scheduler_Impl ()
  : DSRT_Scheduler_Impl ()
{
}

int
MIF_Scheduler_Impl::init (int argc, ACE_TCHAR* argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  //ACE_DEBUG ((LM_DEBUG, "svc::init called\n"));

  while (arg_shifter.is_anything_left ())
    {
      const ACE_TCHAR* arg = arg_shifter.get_current ();

      if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-sched_policy")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("FIFO")) == 0)
                {
                  this->sched_policy_ = ACE_SCHED_FIFO;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("OTHER")) == 0)
                {
                  this->sched_policy_ = ACE_SCHED_OTHER;
                }
              else
                {
                  ACE_ERROR ((LM_ERROR,
                              ACE_LIB_TEXT("-sched_policy ")
                              ACE_LIB_TEXT("unsupported sched policy <%s>\n"),
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
                  this->sched_scope_ = ACE_SCOPE_THREAD;
                }
              else if (ACE_OS::strcasecmp (opt, ACE_LIB_TEXT("PROCESS")) == 0)
                {
                  this->sched_scope_ = ACE_SCOPE_PROCESS;
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

  return 0;
}

int
MIF_Scheduler_Impl::fini (void)
{
  return 0;
}

Priority_t
MIF_Scheduler_Impl::schedule_i (guid_t id, const QoSDescriptor& qos)
{
  ACE_UNUSED_ARG ((id));
  ACE_DEBUG ((LM_DEBUG, "(%t) request for MIF schedule\n"));

  switch (qos.importance_)
    {
    case VERY_LOW_IMPORTANCE:
      return min_prio_;
      break;

    case LOW_IMPORTANCE:
      return ACE_Sched_Params::next_priority
        (sched_policy_, min_prio_, sched_scope_);
      break;

    case MEDIUM_IMPORTANCE:
      return ACE_Sched_Params::next_priority
        (sched_policy_,
         ACE_Sched_Params::next_priority
         (sched_policy_, min_prio_, sched_scope_),
         sched_scope_);
      break;

    case HIGH_IMPORTANCE:
      return ACE_Sched_Params::previous_priority
        (sched_policy_, max_prio_, sched_scope_);
      break;

    case VERY_HIGH_IMPORTANCE:
      return max_prio_;
      break;

    default:
      break;
    }

  return min_prio_;
}


}

// ****************************************************************

void _gobble_MIF_Scheduler_Impl (void *p) { \
  ACE_Service_Object *_p = ACE_static_cast (ACE_Service_Object *, p);
  ACE_ASSERT (_p != 0);
  delete _p; }
extern "C" Kokyu_Export ACE_Service_Object *
_make_MIF_Scheduler_Impl (ACE_Service_Object_Exterminator *gobbler)
{
  if (gobbler != 0)
    *gobbler = (ACE_Service_Object_Exterminator) _gobble_MIF_Scheduler_Impl;
  ACE_DEBUG ((LM_DEBUG, "about to create MIF_Scheduler_Impl\n" ));
  return new Kokyu::MIF_Scheduler_Impl;
}

// ****************************************************************
