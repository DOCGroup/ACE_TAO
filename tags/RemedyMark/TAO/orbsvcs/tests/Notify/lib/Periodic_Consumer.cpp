// $Id$

#include "Periodic_Consumer.h"

#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"
#include "ace/Task.h"
#include "tao/debug.h"
#include "orbsvcs/Time_Utilities.h"
#include "StructuredEvent.h"
#include "Task_Stats.h"
#include "Task_Callback.h"
#include "LookupManager.h"
#include "Priority_Mapping.h"

int WARMUP_COUNT = 10;

TAO_Notify_Tests_Periodic_Consumer::TAO_Notify_Tests_Periodic_Consumer (void)
  : count_ (-2)
  , warmup_countdown_ (WARMUP_COUNT)
  , max_count_ (-1)
  , load_ (0)
  , client_ (0)
  , check_priority_ (0)
  , stop_received_ (0)
{
}

TAO_Notify_Tests_Periodic_Consumer::~TAO_Notify_Tests_Periodic_Consumer ()
{
}

void
TAO_Notify_Tests_Periodic_Consumer::task_callback (TAO_Notify_Tests_Task_Callback* client)
{
  this->client_ = client;
}

int
TAO_Notify_Tests_Periodic_Consumer::init_state (ACE_Arg_Shifter& arg_shifter)
{
  // First, let the base class look for options.
  if (TAO_Notify_Tests_StructuredPushConsumer::init_state (arg_shifter) == -1)
    return -1;

  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-MaxCount"))))
        {
          this->max_count_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();

          if (max_count_ == 0)
          {
            if (this->client_)
              this->client_->done (this);
          }
        }
       else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-Check_Priority")) == 0)
         {
          this->check_priority_ = 1;

          arg_shifter.consume_arg ();
         }
       else
         {
           break;
         }
    } /* while */

  return 0;
}

void
TAO_Notify_Tests_Periodic_Consumer::handle_start_event (const CosNotification::PropertySeq& prop_seq)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "(%P, %t)Consumer %s received inital (-1)th event\n", this->name_.c_str ()));

  for (CORBA::ULong i = 0; i < prop_seq.length (); ++i)
    {
      if (ACE_OS::strcmp (prop_seq[i].name.in (), "BaseTime") == 0)
        {
          TimeBase::TimeT base_time;
          ACE_hrtime_t base_time_hrtime;

          prop_seq[i].value >>= base_time;

          ORBSVCS_Time::TimeT_to_hrtime (base_time_hrtime, base_time);
          stats_.base_time (base_time_hrtime);
        }
      // if max_count has not been already specified, get it from the supplier.
      else if (this->max_count_ == -1 &&
               ACE_OS::strcmp (prop_seq[i].name.in (), "MaxCount") == 0)
        {
          prop_seq[i].value >>= this->max_count_;
        }
      else if (ACE_OS::strcmp (prop_seq[i].name.in (), "Load") == 0)
        {
          prop_seq[i].value >>= this->load_;
        }
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P, %t) Maxcount = %d, Load = %d\n",
                  this->max_count_, this->load_));
    }
}

void
TAO_Notify_Tests_Periodic_Consumer::check_priority (const CosNotification::PropertySeq& prop_seq)
{
  // Check if the event carries a Priority.
  int event_has_priority_set = 0;
  CORBA::Short event_priority = 0;

  for (CORBA::ULong i = 0; i < prop_seq.length (); ++i)
    {
      if (ACE_OS::strcmp (prop_seq[i].name.in (), CosNotification::Priority) == 0)
        {
          prop_seq[i].value >>= event_priority;

          event_has_priority_set = 1;
              break;
        }
    }

  if (event_has_priority_set == 1)
    {
      // Confirm that the current thread is at the priority set in the event
      ACE_hthread_t current;
      ACE_Thread::self (current);

      int priority;
      if (ACE_Thread::getprio (current, priority) == -1)
        {
          ACE_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("TAO (%P|%t) - ")
                      ACE_TEXT (" ACE_Thread::get_prio\n")));

          return ;
        }

      CORBA::Short native_priority = CORBA::Short (priority);

      TAO_Notify_Tests_Priority_Mapping* priority_mapping;
      LOOKUP_MANAGER->resolve (priority_mapping);

      CORBA::Short corba_priority;

      priority_mapping->to_CORBA (native_priority, corba_priority);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG,
                    "Periodic Consumer expected priority = %d, received priority = %d/%d (native/corba)\n",
                    event_priority, native_priority, corba_priority));

      if (corba_priority != event_priority)
        ACE_DEBUG ((LM_DEBUG,
                    "Error: Periodic Consumer expected priority = %d, received priority = %d\n",
                    event_priority, corba_priority));
    }
}

void
TAO_Notify_Tests_Periodic_Consumer::push_structured_event (const CosNotification::StructuredEvent & notification)
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());

  const CosNotification::PropertySeq& prop_seq =
    notification.header.variable_header;

  if (this->count_ == -2)
    {
      if (--warmup_countdown_ == 0)
        this->count_ = -1;

      return;
    }
  else if (this->count_ == -1)
    {
      this->handle_start_event (prop_seq);

      if (this->max_count_ > 0)
        this->stats_.init (this->max_count_);

      this->count_ = 0;
      return;
    }

  if (this->check_priority_)
    {
      this->check_priority (prop_seq);
    }

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P, %t)Consumer %s received %d event type (%s,%s)\n", this->name_.c_str (), this->count_,
                  notification.header.fixed_header.event_type.domain_name.in(),
                  notification.header.fixed_header.event_type.type_name.in()));
    }

  for (CORBA::ULong i = 0; i < prop_seq.length (); ++i)
    {
      if (ACE_OS::strcmp (prop_seq[i].name.in (), "Stop") == 0)
        {
          this->stop_received_ = 1;
        }
    }

  TimeBase::TimeT send_time, now;
  ACE_hrtime_t send_time_hrtime;

  notification.remainder_of_body >>= send_time;

  ORBSVCS_Time::TimeT_to_hrtime (send_time_hrtime, send_time);

  now = ACE_OS::gethrtime ();

  stats_.sample (send_time_hrtime, now);

  // Eat CPU
  static CORBA::ULong prime_number = 9619;

  (void)ACE::gcd (prime_number, prime_number/2 -1);

  for (CORBA::ULong load = this->load_; load != 0; --load)
    ACE::is_prime (prime_number,
                   2,
                   prime_number / 2);

  // ---

  if (++this->count_ >= this->max_count_ || this->stop_received_ == 1)
    {
      stats_.end_time (ACE_OS::gethrtime ());

      if (this->client_)
        this->client_->done (this);

      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "(%P, %t)Consumer %s done\n", this->name_.c_str ()));
    }
}

void
TAO_Notify_Tests_Periodic_Consumer::dump_stats (ACE_TCHAR* msg, int dump_samples)
{
  ACE_TCHAR buf[BUFSIZ];
  ACE_OS::sprintf (buf, ACE_TEXT("%s.dat"), this->name_.c_str ());

  ACE_TString fname (buf);

  ACE_OS::sprintf (buf,
                   ACE_TEXT("%s# Consumer Name = %s, Proxy ID = %d Load = %u\n"),
                   msg,
                   this->name_.c_str (), this->proxy_id_, this->load_);

  stats_.dump_samples (fname.c_str (), buf, dump_samples);
}
