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
#if defined (ACE_WIN32)
  // Count up the number of distinct native priorities on current
  // platform.
  int n;
  Priority_t current_priority = this->min_prio_;
  for (n = 1; current_priority != this->max_; ++n)
    {
      current_priority =
        ACE_Sched_Params::next_priority (this->sched_policy_,
                                         current_priority);
    }

  this->prio_range_ = n-1;

#else
  this->prio_range_ = this->max_prio_ - this->min_prio_ + 1;
#endif /* ACE_WIN32 */

}

int
DSRT_Scheduler_Impl::schedule (guid_t id, const DSRT_QoSDescriptor& qos)
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
      else if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-min_importance")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->min_importance_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }
      else if (ACE_OS::strcasecmp (arg, ACE_LIB_TEXT("-max_importance")) == 0)
        {
          arg_shifter.consume_arg ();

          if (arg_shifter.is_parameter_next ())
            {
              const ACE_TCHAR* opt = arg_shifter.get_current ();
              this->max_importance_ = ACE_OS::atoi (opt);
              arg_shifter.consume_arg ();
            }
        }
    }

  this->importance_range_ = this->max_importance_ - this->min_importance_;
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT("MIF_Scheduler: importance_range = %d\n"),
              this->importance_range_));
  return 0;
}

int
MIF_Scheduler_Impl::fini (void)
{
  return 0;
}

Priority_t
MIF_Scheduler_Impl::schedule_i (guid_t id, const DSRT_QoSDescriptor& qos)
{
  ACE_UNUSED_ARG ((id));
  ACE_DEBUG ((LM_DEBUG, "(%t) request for MIF schedule\n"));

  Priority_t prio = qos.importance_ * this->prio_range_ / this->importance_range_;

  if (prio > max_prio_)
    {
      prio = max_prio_;
    }

  return prio_;
}


}

// ****************************************************************

//could not use ACE_FACTORY_DEFINE macro here because of Kokyu namespace
// see new Kokyu::MIF_Scheduler_Impl in _make_xx factory function
//need to revisit later

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
