// $Id$

#include "Periodic_Supplier.h"

#include "ace/Arg_Shifter.h"
#include "ace/High_Res_Timer.h"
#include "tao/debug.h"
#include "tao/ORB_Core.h"
#include "orbsvcs/Time_Utilities.h"
#include "StructuredEvent.h"
#include "Task_Stats.h"
#include "Task_Callback.h"
#include "LookupManager.h"
#include "Priority_Mapping.h"

ACE_RCSID(RT_Notify, TAO_NS_Periodic_Supplier, "$id$")

TAO_NS_Periodic_Supplier::TAO_NS_Periodic_Supplier (void)
  :barrier_ (0),
   priority_ (0),
   period_ (0),
   exec_time_ (0),
   phase_ (0),
   iter_ (0),
   load_ (0),
   client_ (0)
{
}

TAO_NS_Periodic_Supplier::~TAO_NS_Periodic_Supplier ()
{
}


void
TAO_NS_Periodic_Supplier::task_callback(TAO_NS_Task_Callback* client)
{
  this->client_ = client;
}

const char*
TAO_NS_Periodic_Supplier::proxy_name (void)
{
  return this->proxy_name_.c_str ();
}

void
TAO_NS_Periodic_Supplier::connect (CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr proxy, CosNotifyChannelAdmin::ProxyID proxy_id ACE_ENV_ARG_DECL)
{
  TAO_Notify_StructuredPushSupplier::connect (proxy, proxy_id ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

void
TAO_NS_Periodic_Supplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin ACE_ENV_ARG_DECL)
{
  TAO_Notify_StructuredPushSupplier::connect (supplier_admin ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
TAO_NS_Periodic_Supplier::init_state (ACE_Arg_Shifter& arg_shifter)
{
  const ACE_TCHAR *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-Proxy")))
        {
          proxy_name_ = current_arg;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-EventType")))
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
TAO_NS_Periodic_Supplier::activate (ACE_Barrier* barrier)
{
  barrier_ = barrier;

  long flags = THR_NEW_LWP | THR_JOINABLE;

  // Resolve the ORB
  CORBA::ORB_var orb;
  LOOKUP_MANAGER->resolve (orb);

  flags |=
    orb->orb_core ()->orb_params ()->sched_policy () |
    orb->orb_core ()->orb_params ()->scope_policy ();

  TAO_NS_Priority_Mapping* priority_mapping;
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

int
TAO_NS_Periodic_Supplier::svc (void)
{
  if (TAO_debug_level > 0)
    ACE_DEBUG ((LM_DEBUG, "Thread_Task (%t) - wait\n"));

  // First, wait for other threads.
  this->barrier_->wait ();

  // first thread here inits the Base_Time.
  stats_.base_time (BASE_TIME::instance ()->base_time_);

  // now wait till the phase_ period expires.
  ACE_OS::sleep (ACE_Time_Value (0, phase_));

  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();

  ACE_hrtime_t before, after;

  // This event is special. its not counted to make the performance stats.
  //TAO_NS_StructuredEvent zeroth_event;

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

  ACE_DECLARE_NEW_CORBA_ENV;

  ACE_TRY
    {
      if (TAO_debug_level > 0)
        ACE_DEBUG ((LM_DEBUG, "(%P, %t) Supplier (%s) sending event 0th event\n"));

      this->send_event (zeroth_event.event () ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "Periodic supplier: error sending event. ");
      break;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "Periodic supplier: error sending event. ");
      break;
    }
  ACE_ENDTRY;

  for (int i = 0; i < iter_ ; ++i)
    {
      before = ACE_OS::gethrtime ();

      ORBSVCS_Time::hrtime_to_TimeT (base_time,
                                     before);

      CORBA::Any buffer;
      buffer <<= base_time;

      this->event_.payload (buffer);

      ACE_TRY
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "(%P, %t) Supplier (%s) sending event #%d\n",
                        this->proxy_name_.c_str (), i));

          this->send_event (this->event_.event () ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::UserException, ue)
        {
          ACE_PRINT_EXCEPTION (ue,
                               "Periodic supplier: error sending event. ");
          break;
        }
      ACE_CATCH (CORBA::SystemException, se)
        {
          ACE_PRINT_EXCEPTION (se,
                               "Periodic supplier: error sending event. ");
          break;
        }
      ACE_ENDTRY;

      /*ACE_CATCHANY
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "Periodic supplier: error sending event %s\n"));
          break;
        }
      ACE_ENDTRY;*/

      after = ACE_OS::gethrtime ();

      stats_.sample (before, after);

      if (period_ != 0) // blast mode, no sleep.
        {
          // convert to microseconds
#if !defined ACE_LACKS_LONGLONG_T

          ACE_UINT32 elapsed_microseconds = ACE_UINT32((after - before) / gsf);

#else  /* ! ACE_LACKS_LONGLONG_T */

          ACE_UINT32 elapsed_microseconds = (after - before) / gsf;

#endif /* ! ACE_LACKS_LONGLONG_T */

#if defined (ACE_WIN32)
          elapsed_microseconds*=1000; // convert to uSec on Win32
#endif /* ACE_WIN32 */

      // did we miss any deadlines?

          int missed =
            (int)elapsed_microseconds > period_ ? elapsed_microseconds/period_ : 0;

          long sleep_time = (missed + 1)*period_ ;
          sleep_time -= elapsed_microseconds;

          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "(%t) sleep time = %d\n", sleep_time));

          ACE_Time_Value t_sleep (0, sleep_time);
          ACE_OS::sleep (t_sleep);
        } /* period != 0 */
    } /* for */

  stats_.end_time (ACE_OS::gethrtime ());

  if (this->client_)
    this->client_->done (this);

  return 0;
}

void
TAO_NS_Periodic_Supplier::dump_stats (ACE_TCHAR* msg)
{
  char buf[BUFSIZ];
  ACE_OS::sprintf (buf, "%s.dat", this->name_.c_str ());

  ACE_CString fname (buf);

  ACE_OS::sprintf (buf,"%s# : Supplier Name = %s, Proxy ID = %d, Event Type = %s, priority %d, period %ld, exec_time %ld, phase %ld, iter_ %d , load_ %d\n",
                   msg, this->name_.c_str (), this->proxy_consumer_id_, this->event_.type (), priority_, period_, exec_time_, phase_, iter_, load_);

  stats_.dump_samples (fname.c_str (), buf,
                             ACE_High_Res_Timer::global_scale_factor ());
}

void
TAO_NS_Periodic_Supplier::offer_change (CosNotification::EventTypeSeq &added, CosNotification::EventTypeSeq& removed ACE_ENV_ARG_DECL)
{
  // Let the user see what we're subscribed for,
  ACE_DEBUG ((LM_DEBUG, "(%P, %t) Calling Supplier offer change: \n"));

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
  CosNotifyChannelAdmin::StructuredProxyPushConsumer_ptr proxy_consumer = this->get_proxy_consumer ();

  proxy_consumer->offer_change (added, removed ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}
