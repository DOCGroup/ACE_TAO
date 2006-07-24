// $Id$

#include "Periodic_Supplier.h"

#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"
#include "ace/Barrier.h"
#include "ace/OS_NS_unistd.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/Time_Utilities.h"
#include "StructuredEvent.h"
#include "Task_Stats.h"
#include "Task_Callback.h"
#include "LookupManager.h"
#include "Priority_Mapping.h"

ACE_RCSID (RT_Notify,
           TAO_Notify_Tests_Periodic_Supplier,
           "$Id$")

TAO_Notify_Tests_Periodic_Supplier::TAO_Notify_Tests_Periodic_Supplier (void)
  : barrier_ (0),
    priority_ (0),
    period_ (0),
    total_deadlines_missed_ (0),
    run_time_ (0),
    exec_time_ (0),
    phase_ (0),
    iter_ (0),
    load_ (0),
    client_ (0)
{
}

TAO_Notify_Tests_Periodic_Supplier::~TAO_Notify_Tests_Periodic_Supplier ()
{
}


void
TAO_Notify_Tests_Periodic_Supplier::task_callback(TAO_Notify_Tests_Task_Callback* client)
{
  this->client_ = client;
}

int
TAO_Notify_Tests_Periodic_Supplier::init_state (ACE_Arg_Shifter& arg_shifter)
{
  // First, let the base class look for options.
  if (TAO_Notify_Tests_StructuredPushSupplier::init_state (arg_shifter) == -1)
    return -1;

  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-EventType")))
        {
          this->event_.type ("*", current_arg) ;
          zeroth_event.type ("*", current_arg) ;
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-FilterLongData") == 0) // -FilterLongData name value
        {
          arg_shifter.consume_arg ();

          ACE_CString name = arg_shifter.get_current ();

          arg_shifter.consume_arg ();

          CORBA::Long value = (CORBA::Long)ACE_OS::atoi (arg_shifter.get_current ());

          arg_shifter.consume_arg ();

          CORBA::Any buffer;
          buffer <<= (CORBA::Long) value;

          this->event_.filter (name.c_str (), buffer);
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-Priority")))
        {
          priority_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();

          CORBA::Any buffer;
          buffer <<= (CORBA::Short) this->priority_;
          this->event_.qos (CosNotification::Priority, buffer);
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-Period")))
        {
          period_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-ExecTime")))
        {
          exec_time_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-Phase")))
        {
          phase_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-Iter")))
        {
          iter_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();

          if (stats_.init (iter_) == -1)
            return -1;
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-Load")))
        {
          load_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-RunTime"))) // in seconds
        {
          run_time_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else
        {
          ACE_DEBUG ((LM_DEBUG, "parse Task unknown option %s\n",
                      arg_shifter.get_current ()));
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "event type %s, priority %d, period %duS, exec_time %duS, phase %duS, iter %d, load %d\n",
                        event_.type(), priority_, period_, exec_time_, phase_, iter_, load_));
          break;
        }
    }
  return 0;
}

int
TAO_Notify_Tests_Periodic_Supplier::activate_task (ACE_Barrier* barrier)
{
  barrier_ = barrier;

  long flags = THR_NEW_LWP | THR_JOINABLE;

  // Resolve the ORB
  CORBA::ORB_var orb;
  LOOKUP_MANAGER->resolve (orb);

  flags |=
    orb->orb_core ()->orb_params ()->thread_creation_flags ();

  TAO_Notify_Tests_Priority_Mapping* priority_mapping;
  LOOKUP_MANAGER->resolve (priority_mapping);

  CORBA::Short native_prio;

  priority_mapping->to_native (this->priority_, native_prio);

  // Become an active object.
  if (this->ACE_Task <ACE_SYNCH>::activate (flags,
                                            1,
                                            0,
                                            native_prio) == -1)
       {
         if (ACE_OS::last_error () == EPERM)
           ACE_ERROR_RETURN ((LM_ERROR,
                              ACE_TEXT ("Insufficient privilege to activate ACE_Task.\n")),
                             -1);
         else
           ACE_DEBUG ((LM_ERROR,
                       ACE_TEXT ("(%t) Task activation at priority %d failed, ")
                        ACE_TEXT ("exiting!\n%a"),
                       this->priority_,
                       -1));
       }

  ACE_DEBUG ((LM_ERROR, "Activated Periodic Supplier Thread at priority %d\n", this->priority_));

  return 0;
}

void
TAO_Notify_Tests_Periodic_Supplier::send_warmup_events (ACE_ENV_SINGLE_ARG_DECL)
{
  int WARMUP_COUNT = 10;

  for (int i = 0; i < WARMUP_COUNT ; ++i)
    {
      this->send_event (this->event_.event () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
TAO_Notify_Tests_Periodic_Supplier::send_prologue (ACE_ENV_SINGLE_ARG_DECL)
{
  // populate event.
  // send the base time and max count.
  TimeBase::TimeT base_time;
  ORBSVCS_Time::hrtime_to_TimeT (base_time,
                                 BASE_TIME::instance ()->base_time_);

  CORBA::Any buffer;
  buffer <<= base_time;
  zeroth_event.opt_header ("BaseTime", buffer);

  buffer <<= this->iter_;
  zeroth_event.opt_header ("MaxCount", buffer);

  buffer <<= this->load_;
  zeroth_event.opt_header ("Load", buffer);

  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "(%P, %t) Supplier (%s) sending event 0th event\n"));

  this->send_event (zeroth_event.event () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_Notify_Tests_Periodic_Supplier::handle_svc (ACE_ENV_SINGLE_ARG_DECL)
{
  this->send_prologue (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_hrtime_t before, after;
  TimeBase::TimeT time_t;

  CORBA::Any buffer;

  ACE_hrtime_t base_time = BASE_TIME::instance ()->base_time_;

  for (int i = 0; i < iter_ ; ++i)
    {
      before = ACE_OS::gethrtime ();

      ORBSVCS_Time::hrtime_to_TimeT (time_t,
                                     before);

      buffer <<= time_t;

      this->event_.payload (buffer);

      if (this->run_time_ != 0 &&
          Task_Stats::diff_sec (base_time, before) > this->run_time_)
        {
          // Time up, send a "Stop" event.
          buffer <<= (CORBA::Long) 1;
          this->event_.opt_header ("Stop", buffer);

          i = iter_;  // Load the iter so that the loop exits.
        }

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "(%P, %t) Supplier (%s) sending event #%d\n",
                    this->name_.c_str (), i));

      this->send_event (this->event_.event () ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;

      after = ACE_OS::gethrtime ();

      stats_.sample (before, after);

      if (period_ != 0) // blast mode, no sleep.
        {
          ACE_UINT32 elapsed_microseconds =
            Task_Stats::diff_usec (before, after);

          // did we miss any deadlines?
          int missed =
            (int)elapsed_microseconds > period_ ? elapsed_microseconds/period_ : 0;
          this->total_deadlines_missed_ += missed;

          /* Start -- "Immediate run if last call missed deadline" */
          if (missed > 0) // if we missed
            continue;

          long sleep_time = period_ - elapsed_microseconds;
          /* End -- "Immediate run if last call missed deadline" */

          /*
           * This logic sleeps till the next period.
           * So, if we missed a deadline we wait.
           *
             long sleep_time = (missed + 1)*period_ ;
             sleep_time -= elapsed_microseconds;
          */

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "(%t) sleep time = %d uSec, missed %d deadlines\n", sleep_time, missed));

          ACE_Time_Value t_sleep (0, sleep_time);
          ACE_OS::sleep (t_sleep);
        } /* period != 0 */

    } /* for */

  stats_.end_time (ACE_OS::gethrtime ());

  if (this->client_)
    this->client_->done (this);
}

int
TAO_Notify_Tests_Periodic_Supplier::svc (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Thread_Task (%t) - wait\n"));

  ACE_TRY_NEW_ENV
    {
      // First, send warmup events.
      this->send_warmup_events (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      // Next, wait for other threads.
      this->barrier_->wait ();

      // first thread here inits the Base_Time.
      stats_.base_time (BASE_TIME::instance ()->base_time_);

      // now wait till the phase_ period expires.
      ACE_OS::sleep (ACE_Time_Value (0, phase_));

      this->handle_svc (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
        "Error: Periodic supplier: error sending event. ");
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
        "Error: Periodic supplier: error sending event. ");
    }
  ACE_ENDTRY;

  return 0;
}

void
TAO_Notify_Tests_Periodic_Supplier::dump_stats (ACE_TCHAR* msg, int dump_samples)
{
  char buf[BUFSIZ];
  ACE_OS::sprintf (buf, "%s.dat", this->name_.c_str ());

  ACE_CString fname (buf);

  ACE_OS::sprintf (buf,"%s# : Supplier Name = %s, Proxy ID = %d, Event Type = %s, priority %d, period %ld, exec_time %ld, phase %ld, iter_ %d , load_ %d, deadlines missed = %d\n",
                   msg, this->name_.c_str (), this->proxy_id_, this->event_.type (), priority_, period_, exec_time_, phase_, iter_, load_, this->total_deadlines_missed_);

  stats_.dump_samples (fname.c_str (), buf, dump_samples);
}

#if defined (ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION)
template ACE_Singleton<Base_Time, ACE_Thread_Mutex> *ACE_Singleton<Base_Time, ACE_Thread_Mutex>::singleton_;
#endif /* ACE_HAS_EXPLICIT_STATIC_TEMPLATE_MEMBER_INSTANTIATION */
