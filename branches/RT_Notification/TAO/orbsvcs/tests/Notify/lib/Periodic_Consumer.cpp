// $Id$

#include "Periodic_Consumer.h"

#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"
#include "tao/debug.h"
#include "orbsvcs/Time_Utilities.h"
#include "StructuredEvent.h"
#include "Task_Stats.h"
#include "Task_Callback.h"
#include "LookupManager.h"
#include "Priority_Mapping.h"

ACE_RCSID(RT_Notify, TAO_NS_Periodic_Consumer, "$id$")

TAO_NS_Periodic_Consumer::TAO_NS_Periodic_Consumer (void)
  :count_ (0), max_count_ (-1), client_ (0), check_priority_ (0)
{
}

TAO_NS_Periodic_Consumer::~TAO_NS_Periodic_Consumer ()
{
}

void
TAO_NS_Periodic_Consumer::task_callback (TAO_NS_Task_Callback* client)
{
  this->client_ = client;
}

int
TAO_NS_Periodic_Consumer::init_state (ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
       if ((current_arg = arg_shifter.get_the_parameter ("-Proxy")))
        {
          proxy_name_ = current_arg;
          arg_shifter.consume_arg ();
        }
       else if ((current_arg = arg_shifter.get_the_parameter ("-MaxCount")))
        {
          this->max_count_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();

          if (max_count_ == 0)
          {
            if (this->client_)
              this->client_->done (this);
          }
        }
       else if (arg_shifter.cur_arg_strncasecmp ("-Check_Priority") == 0)
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

const char*
TAO_NS_Periodic_Consumer::proxy_name (void)
{
  return this->proxy_name_.c_str ();
}

void
TAO_NS_Periodic_Consumer::connect (CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr proxy, CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  TAO_Notify_StructuredPushConsumer::connect (proxy, proxy_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Consumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin ACE_ENV_ARG_DECL)
{
  TAO_Notify_StructuredPushConsumer::connect (consumer_admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Consumer::subscription_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "Calling Consumer subscription change: "));

  CORBA::ULong i = 0;

  for (; i < added.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "+(%s,%s), ", added[i].domain_name.in (), added[i].type_name.in ()));
    }
  for (i = 0; i < removed.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "-(%s,%s), ", removed[i].domain_name.in (), removed[i].type_name.in ()));
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));

  /// subscribe
  CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr proxy_supplier = this->get_proxy_supplier ();

  proxy_supplier->subscription_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Consumer::push_structured_event (const CosNotification::StructuredEvent & notification ACE_ENV_ARG_DECL)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX, ace_mon, this->lock_,
                      CORBA::INTERNAL ());
  ACE_CHECK;

  if (TAO_debug_level > 0)
    {
      ACE_DEBUG ((LM_DEBUG, "(%P, %t)Consumer %s received %d event type (%s,%s) \n", this->name_.c_str (), this->count_,
                  notification.header.fixed_header.event_type.domain_name.in(),
                  notification.header.fixed_header.event_type.type_name.in()));
    }

  if (this->count_ == 0)
  {
    const CosNotification::PropertySeq& prop_seq = notification.header.variable_header;

    for (CORBA::ULong i = 0; i < prop_seq.length (); ++i)
    {
      if (ACE_OS::strcmp (prop_seq[i].name.in (), "BaseTime") == 0)
        {
          TimeBase::TimeT base_time;
          ACE_hrtime_t base_time_hrtime;

          prop_seq[i].value >>= base_time;

          ORBSVCS_Time::TimeT_to_hrtime (base_time_hrtime, base_time);
          stats_.base_time (base_time_hrtime);
        } // if max_count has not been already specified, get it from the supplier.
      else if (this->max_count_ == -1 && ACE_OS::strcmp (prop_seq[i].name.in (), "MaxCount") == 0)
        {
          prop_seq[i].value >>= this->max_count_;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "(%P, %t) Setting Maxcount = %d\n", this->max_count_));
        }
    }

    this->stats_.init (this->max_count_);
  }

  if (this->check_priority_)
    {
      // Check if the event carries a Priority.
      const CosNotification::PropertySeq& prop_seq = notification.header.variable_header;

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

          TAO_NS_Priority_Mapping* priority_mapping;
          LOOKUP_MANAGER->resolve (priority_mapping);

          CORBA::Short corba_priority;

          priority_mapping->to_CORBA (native_priority, corba_priority);

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG,
                        "Periodic Consumer expected priority = %d, received priority = %d\n",
                        event_priority, corba_priority));

          if (corba_priority != event_priority)
            ACE_DEBUG ((LM_DEBUG,
                        "Error: Periodic Consumer expected priority = %d, received priority = %d\n",
                        event_priority, corba_priority));
        }
    }

  TimeBase::TimeT send_time, now;
  ACE_hrtime_t send_time_hrtime;

  notification.remainder_of_body >>= send_time;

  ORBSVCS_Time::TimeT_to_hrtime (send_time_hrtime, send_time);

  now = ACE_OS::gethrtime ();

  stats_.sample (send_time_hrtime, now);

  if (++this->count_ >= this->max_count_ )
    {
      stats_.end_time (ACE_OS::gethrtime ());

      if (this->client_)
        this->client_->done (this);
    }
}

void
TAO_NS_Periodic_Consumer::dump_stats (ACE_TCHAR* msg)
{
  char buf[BUFSIZ];
  ACE_OS::sprintf (buf, "%s.dat", this->name_.c_str ());

  ACE_CString fname (buf);

  ACE_OS::sprintf (buf,"%s# Consumer Name = %s, Proxy ID = %d\n",
                   msg, this->name_.c_str (), this->proxy_supplier_id_);

  stats_.dump_samples (fname.c_str (), buf,
                             ACE_High_Res_Timer::global_scale_factor ());
}
