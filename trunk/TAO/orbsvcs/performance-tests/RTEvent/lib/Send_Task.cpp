/**
 * @file Send_Task.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Send_Task.h"
#include "orbsvcs/Time_Utilities.h"

ACE_RCSID(TAO_PERF_RTEC, Send_Task, "$Id$")

Send_Task::Send_Task (void)
  : iterations_ (0)
  , period_in_usecs_ (0)
  , event_type_ (0)
  , event_source_ (0)
  , barrier_ (0)
  , stop_ (0)
{
}

void
Send_Task::init (int iterations,
                 int period_in_usecs,
                 int event_type,
                 int event_source,
                 Supplier *supplier,
                 ACE_Barrier *barrier)
{
  this->iterations_ = iterations;
  this->period_in_usecs_ = period_in_usecs;
  this->event_type_ = event_type;
  this->event_source_ = event_source;
  this->supplier_ = Servant_var<Supplier>::duplicate (supplier);
  this->barrier_ = barrier;
}

void
Send_Task::stop (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->mutex_);
  this->stop_ = 1;
}

int
Send_Task::svc (void)
{
  ACE_DECLARE_NEW_CORBA_ENV;
  ACE_TRY
    {
      if (this->barrier_ == 0)
        return -1;

      this->barrier_->wait ();

      int start_i = 0;
      if (this->iterations_ == 0)
        {
          /// Starting from 1 results in an infinite loop, which is
          /// exactly what we want, kind of hackish IMHO...
          start_i = 1;
        }
      for (int i = start_i; i != this->iterations_; ++i)
        {
          RtecEventComm::EventSet event (1);
          event.length (1);
          event[0].header.type   = this->event_type_;
          event[0].header.source = this->event_source_;
          event[0].header.ttl    = 1;

          ACE_Time_Value period (0, this->period_in_usecs_);

          ACE_OS::sleep (period);
          {
            ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
            if (this->stop_)
              return 0;
          }
          ACE_hrtime_t creation = ACE_OS::gethrtime ();
          ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                         creation);
          // push one event...
          this->supplier_->push (event ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
#elif defined(ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
