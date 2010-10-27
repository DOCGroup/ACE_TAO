/**
 * @file Send_Task.cpp
 *
 * $Id$
 *
 * @author Carlos O'Ryan <coryan@uci.edu>
 */

#include "Send_Task.h"

#include "orbsvcs/Time_Utilities.h"

#include "ace/Barrier.h"
#include "ace/OS_NS_unistd.h"

Send_Task::Send_Task (void)
  : iterations_ (0)
  , period_in_usecs_ (0)
  , startup_sleep_ (0)
  , event_type_ (0)
  , event_source_ (0)
  , barrier_ (0)
  , stop_ (0)
{
}

void
Send_Task::init (int iterations,
                 int period_in_usecs,
                 int startup_sleep,
                 int event_type,
                 int event_source,
                 Supplier *supplier,
                 ACE_Barrier *barrier)
{
  this->iterations_ = iterations;
  this->period_in_usecs_ = period_in_usecs;
  this->startup_sleep_ = startup_sleep;
  this->event_type_ = event_type;
  this->event_source_ = event_source;
  this->supplier_ =
    TAO::Utils::Servant_Var<Supplier>::_duplicate (supplier);
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
  if (this->barrier_ == 0)
    return -1;

  this->barrier_->wait ();

  ACE_Time_Value startup (0, this->startup_sleep_);
  ACE_OS::sleep (startup);

  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) - Thread started, "
              "iterations = %d, period = %d, event_type = %d\n",
              this->iterations_, this->period_in_usecs_,
              this->event_type_));


  int start_i = 0;
  if (this->iterations_ == 0)
    {
      // Starting from 1 results in an infinite loop (well, so long
      // that I call it infinite), which is exactly what we want, kind
      // of hackish, oh well.
      start_i = 1;
    }

  RtecEventComm::EventSet event (1);
  event.length (1);
  event[0].header.type   = this->event_type_;
  event[0].header.source = this->event_source_;
  event[0].header.ttl    = 1;

  for (int i = start_i; i != this->iterations_; ++i)
    {
      if ((i + 1) % 1000 == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "(%P|%t) - Thread has sent %d messages @ %T\n",
                      i + 1));
        }

      ACE_Time_Value period (0, this->period_in_usecs_);
      ACE_OS::sleep (period);
      {
        ACE_GUARD_RETURN (TAO_SYNCH_MUTEX, ace_mon, this->mutex_, -1);
        if (this->stop_ != 0)
          {
            ACE_DEBUG ((LM_DEBUG,
                        "(%P|%t) - Thread has been stopped\n"));
            return 0;
          }
      }
      ACE_hrtime_t creation = ACE_OS::gethrtime ();
      ORBSVCS_Time::hrtime_to_TimeT (event[0].header.creation_time,
                                     creation);
      try
        {
          // push one event...
          this->supplier_->push (event);
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception ("Caught exception:");
        }
    }
  ACE_DEBUG ((LM_DEBUG,
              "(%P|%t) - Thread finished\n"));
  return 0;
}
