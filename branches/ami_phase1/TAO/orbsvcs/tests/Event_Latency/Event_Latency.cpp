// $Id$

#if defined (ACE_HAS_QUANTIFY)
# include <quantify.h>
#endif /* ACE_HAS_QUANTIFY */

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Profile_Timer.h"

#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "orbsvcs/Event/EC_Event_Channel.h"
#include "orbsvcs/Event/EC_Basic_Factory.h"
#include "Event_Latency.h"

#include "tao/Timeprobe.h"

ACE_RCSID(Event_Latency, Event_Latency, "$Id$")

#if defined (ACE_ENABLE_TIMEPROBES)

static const char *Event_Latency_Timeprobe_Description[] =
{
  "push event to consumer",
  "start with new event in Supplier",
  "supplier starts pushing event",
  "supplier ends pushing event"
};

enum
{
  // Timeprobe description table start key
  EVENT_LATENCY_PUSH_EVENT_TO_CONSUMER = 20000,
  EVENT_LATENCY_START_WITH_NEW_EVENT_IN_SUPPLIER,
  EVENT_LATENCY_SUPPLIER_STARTS_PUSHING_EVENT,
  EVENT_LATENCY_SUPPLIER_ENDS_PUSHING_EVENT
};

// Setup Timeprobes
ACE_TIMEPROBE_EVENT_DESCRIPTIONS (Event_Latency_Timeprobe_Description,
                                  EVENT_LATENCY_PUSH_EVENT_TO_CONSUMER);

#endif /* ACE_ENABLE_TIMEPROBES */

static const char usage [] = "[-? |\n"
"            [-c <consumers> [1]]\n"
"            [-d directly connect all consumers/suppliers\n"
"            [-j to collect jitter statistics]\n"
"            [-m <count> of messages to send [10]]\n"
"            [-s <suppliers>, [1]]\n"
"            [-t <timeout interval>, msec [250]]]";

// Configuration parameters.
static u_int consumers = 1;
static u_int suppliers = 1;
static u_int total_messages = 10;
static int measure_jitter = 0;
static u_int timeout_interval = 250; // msec

static int short_circuit_EC = 0;
static int shutting_down = 0;

// This is global to allow the Supplier to short circuit the EC
// and talk directly to consumers.  For testing only :-)
static Latency_Consumer **consumer;

// ************************************************************

Latency_Consumer::Latency_Consumer (const int measure_jitter)
  : measure_jitter_ (measure_jitter),
    min_latency_ (INT_MAX),
    max_latency_ (INT_MIN),
    total_latency_ (0),
    total_pushes_ (0),
    min_to_ec_ (INT_MAX),
    max_to_ec_ (INT_MIN),
    sum_to_ec_ (0),
    min_in_ec_ (INT_MAX),
    max_in_ec_ (INT_MIN),
    sum_in_ec_ (0),
    min_from_ec_ (INT_MAX),
    max_from_ec_ (INT_MIN),
    sum_from_ec_ (0)
{
}

int
Latency_Consumer::open_consumer (RtecEventChannelAdmin::EventChannel_ptr ec,
                                 const char *my_name)
{
  entry_point (my_name);
  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      rt_info_ =
        server->create (my_name, TAO_TRY_ENV);

      const ACE_hrtime_t wcet_ns = ACE_UINT64_LITERAL (10000000); // 1 ms
      RtecScheduler::Time wcet;
      ORBSVCS_Time::hrtime_to_TimeT (wcet, wcet_ns);

      server->set (rt_info_,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
                   wcet,
                   wcet,
                   ORBSVCS_Time::zero (),
                   0,
                   RtecScheduler::VERY_LOW_IMPORTANCE,
                   ORBSVCS_Time::zero (),
                   1,
                   RtecScheduler::OPERATION,
                   TAO_TRY_ENV);

      // Create the event that we're registering for.
      ACE_ConsumerQOS_Factory dependencies;
      dependencies.start_disjunction_group ();
      dependencies.insert_type (ACE_ES_EVENT_NOTIFICATION, rt_info_);
      dependencies.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info_);

      this->channel_admin_ =
        RtecEventChannelAdmin::EventChannel::_duplicate (ec);

      // = Connect as a consumer.
      this->consumer_admin_ =
        channel_admin_->for_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->suppliers_ =
        consumer_admin_->obtain_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushConsumer_var objref =
        this->_this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->suppliers_->connect_push_consumer (objref.in (),
                                               dependencies.get_ConsumerQOS (),
                                               TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCH (RtecEventChannelAdmin::EventChannel::SUBSCRIPTION_ERROR, se)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Latency_Consumer::open: "
                         "subscribe failed.\n"), -1);
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Latency_Consumer::open: "
                         "unexpected exception.\n"), -1);
    }
  TAO_ENDTRY;

  return 0;
}

void
Latency_Consumer::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, "Consumer received disconnect from channel.\n"));
}

void
Latency_Consumer::push (const RtecEventComm::EventSet &events,
                        CORBA::Environment &)
{
  // ACE_DEBUG ((LM_DEBUG, "Latency_Consumer:push - "));
  ACE_TIMEPROBE (EVENT_LATENCY_PUSH_EVENT_TO_CONSUMER);

  // ACE_DEBUG ((LM_DEBUG, "%d event(s)\n", events.length ()));
  if (events.length () == 0)
    {
      return;
    }

#if defined (ACE_HAS_QUANTIFY)
  // If measuring jitter, just Quantify the supplier-consumer path.
  if (measure_jitter_)
    {
      quantify_stop_recording_data ();
    }
#endif /* ACE_HAS_QUANTIFY */

  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      if (events[i].header.type == ACE_ES_EVENT_SHUTDOWN)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Latency Consumer: received shutdown event\n"));
          this->shutdown ();
        }
      else
        {
          if (measure_jitter_)
            {
              ACE_hrtime_t creation;
              ORBSVCS_Time::TimeT_to_hrtime (creation,
                                             events[i].header.creation_time);

              ACE_hrtime_t ec_recv;
              ORBSVCS_Time::TimeT_to_hrtime (ec_recv,
                                             events[i].header.ec_recv_time);

              ACE_hrtime_t ec_send;
              ORBSVCS_Time::TimeT_to_hrtime (ec_send,
                                             events[i].header.ec_send_time);

              const ACE_hrtime_t now = ACE_OS::gethrtime ();
              const ACE_hrtime_t elapsed = now - creation;
              ACE_Time_Value latency (ACE_static_cast (long,
                elapsed / ACE_ONE_SECOND_IN_NSECS),
                                      ACE_static_cast (long,
                ACE_CU64_TO_CU32 (elapsed) % ACE_ONE_SECOND_IN_NSECS / 1000));

              const long to_ec_nsecs = ACE_CU64_TO_CU32 (ec_recv - creation);
              ACE_Time_Value to_ec (to_ec_nsecs / ACE_ONE_SECOND_IN_NSECS,
                                    to_ec_nsecs % ACE_ONE_SECOND_IN_NSECS /
                                      1000);

              ACE_Time_Value in_ec, from_ec;
              if (! short_circuit_EC)
                {
                  const ACE_hrtime_t in_ec_nsecs = ec_send - ec_recv;
                  in_ec =
                    ACE_Time_Value (ACE_static_cast (long,
                      in_ec_nsecs / ACE_ONE_SECOND_IN_NSECS),
                                    ACE_static_cast (long,
                      ACE_CU64_TO_CU32 (in_ec_nsecs) %
                        ACE_ONE_SECOND_IN_NSECS / 1000));

                  const ACE_hrtime_t from_ec_nsecs = now - ec_send;
                  from_ec =
                    ACE_Time_Value (ACE_static_cast (long,
                      from_ec_nsecs / ACE_ONE_SECOND_IN_NSECS),
                                    ACE_static_cast (long,
                      ACE_CU64_TO_CU32 (from_ec_nsecs) %
                        ACE_ONE_SECOND_IN_NSECS / 1000));
                }

              if (! shutting_down)
                {
                  ++total_pushes_;
                  if (min_latency_ > latency) min_latency_ = latency;
                  if (max_latency_ < latency) max_latency_ = latency;
                  total_latency_ += latency;
                  if (min_to_ec_ > to_ec) min_to_ec_ = to_ec;
                  if (max_to_ec_ < to_ec) max_to_ec_ = to_ec;
                  if (! short_circuit_EC)
                    {
                      sum_to_ec_ += to_ec;
                      if (min_in_ec_ > in_ec) min_in_ec_ = in_ec;
                      if (max_in_ec_ < in_ec) max_in_ec_ = in_ec;
                      sum_in_ec_ += in_ec;
                      if (min_from_ec_ > from_ec) min_from_ec_ = from_ec;
                      if (max_from_ec_ < from_ec) max_from_ec_ = from_ec;
                      sum_from_ec_ += from_ec;
                    }
                }
            }
        }

    }
}

void
Latency_Consumer::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s shutting down.\n", entry_point ()));

  TAO_TRY
    {
      // Disconnect from the push supplier.
      this->suppliers_->disconnect_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                 "(%t) %s Latency_Consumer::shutdown: unexpected exception.\n",
                  entry_point ()));
    }
  TAO_ENDTRY;
}


void
Latency_Consumer::print_stats () /* const */
{
  if (measure_jitter_)
    {
      double lat_min =
        (min_latency_.sec () * 1000000.0 + min_latency_.usec ()) / 1000.0;
      double lat_max =
        (max_latency_.sec () * 1000000.0 + max_latency_.usec ()) / 1000.0;
      double lat_avg =
        (total_latency_.sec () * 1000000.0 +total_latency_.usec ()) /
        total_pushes_ / 1000.0;
      ACE_DEBUG ((LM_TRACE,
                  "%s: Latency in msec (min/max/avg): "
                    "%5.3f/%5.3f/%5.3f\n",
                  entry_point (), lat_min, lat_max, lat_avg));

      if (! short_circuit_EC)
        {
          double to_ec_min =
            (min_to_ec_.sec () * 1000000.0 + min_to_ec_.usec ()) / 1000.0;
          double to_ec_max =
            (max_to_ec_.sec () * 1000000.0 + max_to_ec_.usec ()) / 1000.0;
          double to_ec_avg =
            (sum_to_ec_.sec () * 1000000.0 + sum_to_ec_.usec ()) /
            total_pushes_ / 1000.0;
          ACE_DEBUG ((LM_TRACE,
                      "%s: From test to EC (min/max/avg): "
                      "%5.3f/%5.3f/%5.3f\n",
                      entry_point (), to_ec_min, to_ec_max, to_ec_avg));

          double in_ec_min =
            (min_in_ec_.sec () * 1000000.0 + min_in_ec_.usec ()) / 1000.0;
          double in_ec_max =
            (max_in_ec_.sec () * 1000000.0 + max_in_ec_.usec ()) / 1000.0;
          double in_ec_avg =
            (sum_in_ec_.sec () * 1000000.0 + sum_in_ec_.usec ()) /
            total_pushes_ / 1000.0;
          ACE_DEBUG ((LM_TRACE,
                      "%s: In the EC (min/max/avg): "
                      "%5.3f/%5.3f/%5.3f\n",
                      entry_point (), in_ec_min, in_ec_max, in_ec_avg));

          double from_ec_min =
            (min_from_ec_.sec () * 1000000.0 + min_from_ec_.usec ()) / 1000.0;
          double from_ec_max =
            (max_from_ec_.sec () * 1000000.0 + max_from_ec_.usec ()) / 1000.0;
          double from_ec_avg =
            (sum_from_ec_.sec () * 1000000.0 + sum_from_ec_.usec ()) /
            total_pushes_ / 1000.0;
          ACE_DEBUG ((LM_TRACE,
                      "%s: From EC to test (min/max/avg): "
                      "%5.3f/%5.3f/%5.3f\n",
                      entry_point (), from_ec_min, from_ec_max, from_ec_avg));
        }
    }
}


// ************************************************************

Latency_Supplier::Supplier::Supplier (Latency_Supplier* impl)
  :  impl_ (impl)
{
}

void Latency_Supplier::Supplier::disconnect_push_supplier
  (CORBA::Environment &TAO_IN_ENV)
{
  this->impl_->disconnect_push_supplier (TAO_IN_ENV);
}

Latency_Supplier::Consumer::Consumer (Latency_Supplier* impl)
  :  impl_ (impl)
{
}

void Latency_Supplier::Consumer::disconnect_push_consumer
  (CORBA::Environment &TAO_IN_ENV)
{
  this->impl_->disconnect_push_consumer (TAO_IN_ENV);
}

void Latency_Supplier::Consumer::push
  (const RtecEventComm::EventSet &events,
   CORBA::Environment &TAO_IN_ENV)
{
  this->impl_->push (events, TAO_IN_ENV);
}

// ************************************************************

Latency_Supplier::Latency_Supplier (const u_int total_messages,
                                    CORBA::Long supplier_id,
                                    const int timestamp)
  : total_messages_ (total_messages),
    supplier_id_ (supplier_id),
    timestamp_ (timestamp),
    total_sent_ (0),
    master_ (0),
    supplier_ (this),
    consumer_ (this)
{
}

int
Latency_Supplier::open_supplier (RtecEventChannelAdmin::EventChannel_ptr ec,
                                 const char *name, int master)
{
  this->entry_point (name);
  master_ = master;
  TAO_TRY
    {
      this->channel_admin_ =
        RtecEventChannelAdmin::EventChannel::_duplicate (ec);

      RtecScheduler::Scheduler_ptr server =
        ACE_Scheduler_Factory::server ();

      this->rt_info_ =
        server->create (name, TAO_TRY_ENV);

      RtecScheduler::Period_t period = timeout_interval * 20000;

      server->set (rt_info_,
                   RtecScheduler::VERY_HIGH_CRITICALITY,
                   ORBSVCS_Time::zero (),
                   ORBSVCS_Time::zero (),
                   ORBSVCS_Time::zero (),
                   period,
                   RtecScheduler::VERY_LOW_IMPORTANCE,
                   ORBSVCS_Time::zero (),
                   1,
                   RtecScheduler::OPERATION,
                   TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_SupplierQOS_Factory publications;
      publications.insert (supplier_id_,
                           ACE_ES_EVENT_NOTIFICATION,
                           rt_info_, 1);
      publications.insert (supplier_id_,
                           ACE_ES_EVENT_SHUTDOWN,
                           rt_info_, 1);

      // = Connect as a supplier.
      this->supplier_admin_ =
        channel_admin_->for_suppliers (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->consumers_ =
        supplier_admin_->obtain_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushSupplier_var objref =
        this->supplier_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      consumers_->connect_push_supplier (objref.in (),
                                         publications.get_SupplierQOS (),
                                         TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("Latency_Supplier::open");
      return -1;
    }
  TAO_ENDTRY;

  return 0;
}

void
Latency_Supplier::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG,
              "Supplier-consumer received disconnect from channel.\n"));
}

void
Latency_Supplier::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, "Supplier received disconnect from channel.\n"));
}

int
Latency_Supplier::start_generating_events (void)
{
  const ACE_hrtime_t now = ACE_OS::gethrtime ();
  test_start_time_.set (ACE_static_cast (long, now / 1000000000),
                        ACE_static_cast (long,
                                         ACE_CU64_TO_CU32 (now) % 1000000000 /
                                           1000));

  TAO_TRY
    {
      ACE_Time_Value tv_timeout (0, timeout_interval * 1000);
      TimeBase::TimeT timeout;
      ORBSVCS_Time::Time_Value_to_TimeT (timeout, tv_timeout);

      ACE_DEBUG ((LM_DEBUG,
                  "start generating events: "
                  "timeout = %d "
                  "interval = %d "
                  "tv.msec () = %d\n",
                  ACE_U64_TO_U32 (timeout),
                  timeout_interval,
                  tv_timeout.msec ()));

      ACE_ConsumerQOS_Factory dependencies;
      dependencies.start_disjunction_group ();
      dependencies.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
                                timeout,
                                rt_info_);
      if (!master_)
        dependencies.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info_);

      // = Connect as a consumer.
      consumer_admin_ =
        channel_admin_->for_consumers (TAO_TRY_ENV);
      TAO_CHECK_ENV;
      suppliers_ =
        consumer_admin_->obtain_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventComm::PushConsumer_var objref =
        this->consumer_._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      this->suppliers_->connect_push_consumer (objref.in (),
                                               dependencies.get_ConsumerQOS (),
                                               TAO_TRY_ENV);
      TAO_CHECK_ENV;
    }
  TAO_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Latency_Supplier::generate_events:"
                         " unexpected exception.\n"), -1);
    }
  TAO_ENDTRY;

  return 0;
}

void
Latency_Supplier::push (const RtecEventComm::EventSet &events,
                        CORBA::Environment & TAO_IN_ENV)
{
  // ACE_DEBUG ((LM_DEBUG, "Latency_Supplier::push - "));
  ACE_UNUSED_ARG (TAO_IN_ENV);

  // ACE_DEBUG ((LM_DEBUG, "%d event(s)\n", events.length ()));
  if (events.length () == 0)
    {
      return;
    }

  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      if (!master_ && events[i].header.type == ACE_ES_EVENT_SHUTDOWN)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "Latency Supplier: received shutdown event\n"));
          this->shutdown ();
        }
      else if (events[i].header.type == ACE_ES_EVENT_INTERVAL_TIMEOUT)
        {
          // Create the event to send.
          RtecEventComm::Event event;
          event.header.source = this->supplier_id_;
          event.header.type = ACE_ES_EVENT_NOTIFICATION;
          ++total_sent_;

          if (this->timestamp_)
            {
              ACE_hrtime_t now = ACE_OS::gethrtime ();
              ORBSVCS_Time::hrtime_to_TimeT (event.header.creation_time,
                                             now);
            }

          // @@ ACE_TIMEPROBE_RESET;
          // @@ ACE_TIMEPROBE (EVENT_LATENCY_START_WITH_NEW_EVENT_IN_SUPPLIER);

          TAO_TRY
            {
              if (short_circuit_EC)
                {
                  for (u_int cons = 0; cons < consumers; ++cons)
                    {
                      // This constructor is fast.
                      const RtecEventComm::EventSet es (1, 1, &event);
                      consumer [cons]->push (es, TAO_TRY_ENV);
                    }
                }
              else
                {
#if defined (ACE_HAS_QUANTIFY)
                  // If measuring jitter, just Quantify the
                  // supplier-consumer path.
                  if (measure_jitter)
                    {
                      quantify_start_recording_data ();
                    }
#endif /* ACE_HAS_QUANTIFY */

                  ACE_TIMEPROBE (EVENT_LATENCY_SUPPLIER_STARTS_PUSHING_EVENT);

                  RtecEventComm::EventSet events (1);
                  events.length (1);
                  events[0] = event;
                  consumers_->push (events, TAO_TRY_ENV);

                  ACE_TIMEPROBE (EVENT_LATENCY_SUPPLIER_ENDS_PUSHING_EVENT);
                }
              TAO_CHECK_ENV;
            }
          TAO_CATCHANY
            {
              ACE_ERROR ((LM_ERROR, "(%t) %s Latency_Supplier::push:"
                          " unexpected exception.\n",
                          entry_point ()));
            }
          TAO_ENDTRY;

          // Check if we're done.
          if (master_ && (total_sent_ >= total_messages_))
            this->shutdown ();
        }
      else
        {
          ACE_ERROR ((LM_ERROR, "(%t) %s received unexpected events: %d\n",
                      entry_point (), events[i].header.type));
          // ::dump_sequence (events);
          return;
        }
    }
}

void
Latency_Supplier::shutdown (void)
{
  shutting_down = 1;

#if defined (ACE_HAS_QUANTIFY)
    // Need to stop recording here even if testing for jitter, because
    // recording is still probably enabled.
    quantify_stop_recording_data ();
    if (! measure_jitter)
      {
        ACE_DEBUG ((LM_DEBUG, "(%t) stopped Quantify recording\n"));
      }
#endif /* ACE_HAS_QUANTIFY */

  const ACE_hrtime_t now = ACE_OS::gethrtime ();
  test_stop_time_.set (ACE_static_cast (long, now / ACE_ONE_SECOND_IN_NSECS),
                       ACE_static_cast (long, ACE_CU64_TO_CU32 (now) %
                                                ACE_ONE_SECOND_IN_NSECS /
                                                1000));

  static int total_iterations = 1;
  if (--total_iterations > 0)
    {
      total_sent_ = 0;
      return;
    }

  TAO_TRY
    {
      if (master_)
        {
          // Create the shutdown message.
          RtecEventComm::Event event;
          event.header.source = this->supplier_id_;
          event.header.type = ACE_ES_EVENT_SHUTDOWN;

          // Push the shutdown event.
          RtecEventComm::EventSet events (1);
          events.length (1);
          events[0] = event;
          consumers_->push (events, TAO_TRY_ENV);
          TAO_CHECK_ENV;
        }

      // Disconnect from the channel.
      consumers_->disconnect_push_consumer (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Disconnect from the push supplier.
      suppliers_->disconnect_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      if (master_)
        {
          TAO_ORB_Core_instance ()->orb ()->shutdown ();
        }
    }
  TAO_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "(%t) %s Latency_Supplier::shutdown:"
                  " unexpected exception.\n",
                  entry_point ()));
      TAO_TRY_ENV.print_exception ("Latency_Supplier::shutdown");
    }
  TAO_ENDTRY;
}


void
Latency_Supplier::print_stats () /* const */
{
  ACE_Time_Value test_elapsed_time (test_stop_time_ - test_start_time_);
  const u_int elapsed = test_elapsed_time.sec () * 1000000 +
                        test_elapsed_time.usec ();  // usec

  ACE_DEBUG ((LM_TRACE,
              "%s: delivered %u events to %u consumer(s) in %u msec;\n"
                "        per-event average was %8.3f msec.\n",
              entry_point (),
              total_sent_,
              consumers / suppliers + consumers % suppliers,
              elapsed / 1000,
              ACE_static_cast (ACE_CAST_CONST double, elapsed) / total_sent_ /
                1000.0));
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function get_options
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

static
unsigned int
get_options (int argc, char *argv [])
{
  ACE_Get_Opt get_opt (argc, argv, "Oc:djm:s:t:?");
  int opt;
  int temp;

  while ((opt = get_opt ()) != EOF)
  {
    switch (opt) {
    case 'c':
      if ((temp = ACE_OS::atoi (get_opt.optarg)) > 0)
        {
          consumers = ACE_static_cast (u_int, temp);
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%s: number of consumers must be > 0",
                             argv[0]), 1);
        }
      break;
    case 'd':
      short_circuit_EC = 1;
      break;
    case 'j':
      measure_jitter = 1;
      break;
    case 'm':
      if ((temp = ACE_OS::atoi (get_opt.optarg)) > 0)
        {
          total_messages = ACE_static_cast (u_int, temp);
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%s: count must be > 0",
                             argv[0]), 1);
        }
      break;
    case 's':
      if ((temp = ACE_OS::atoi (get_opt.optarg)) > 0)
        {
          suppliers = ACE_static_cast (u_int, temp);
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%s: number of suppliers must be > 0",
                             argv[0]), 1);
        }
      break;
    case 't':
      if (ACE_OS::atoi (get_opt.optarg) >= 0)
        {
          timeout_interval = ACE_OS::atoi (get_opt.optarg);
        }
      else
        {
          ACE_ERROR_RETURN ((LM_ERROR,
                             "%s: timeout must be >= 0",
                             argv[0]), 1);
        }
      break;
    case '?':
      ACE_DEBUG ((LM_DEBUG,
                  "Usage: %s %s\n",
                  argv[0], usage));
      ACE_OS::exit (0);
      break;
    default:
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: unknown arg, -%c\n"
                         "Usage: %s %s\n",
                         argv[0], ACE_static_cast (char, opt),
                         argv[0], usage), 1);
    }
  }

  if (argc != get_opt.optind)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "%s: too many arguments\n"
                         "Usage: %s %s\n",
                         argv[0], argv[0], usage), 1);
    }

  return 0;
}


///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function main
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////

int
main (int argc, char *argv [])
{
  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
          ACE_Sched_Params::priority_min (ACE_SCHED_FIFO),
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_MAX, "Latency: user is not superuser, "
                              "so remain in time-sharing class\n"));
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "%p\n", "Latency"));
        }
    }

  u_int i;

  TAO_TRY
    {
      // Initialize ORB.
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");
      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::Object_var naming_obj =
        orb->resolve_initial_references ("NameService");
      if (CORBA::is_nil (naming_obj.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           " (%P|%t) Unable to get the Naming Service.\n"),
                          1);

      CosNaming::NamingContext_var naming_context =
        CosNaming::NamingContext::_narrow (naming_obj.in (), TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Scheduler_Factory::use_config (naming_context.in ());

      if (get_options (argc, argv))
        ACE_OS::exit (-1);

      // Allocate the timeprobe instance now, so we don't measure
      // the cost of doing it later.
      ACE_TIMEPROBE_RESET;

#if 1
      CosNaming::Name channel_name (1);
      channel_name.length (1);
      channel_name[0].id = CORBA::string_dup ("EventService");

      CORBA::Object_var ec_obj =
        naming_context->resolve (channel_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::EventChannel_var ec =
        RtecEventChannelAdmin::EventChannel::_narrow (ec_obj.in (),
                                                      TAO_TRY_ENV);
      TAO_CHECK_ENV;
#else
      TAO_EC_Basic_Factory ec_factory;

      TAO_EC_Event_Channel ec_impl  (root_poa.in (),
                                     root_poa.in (),
                                     &ec_factory);
      ec_impl.activate (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      RtecEventChannelAdmin::EventChannel_var ec =
        ec_impl._this (TAO_TRY_ENV);
      TAO_CHECK_ENV;
#endif /* 0 */

      // Create supplier(s).
      Latency_Supplier **supplier;
      ACE_NEW_RETURN (supplier, Latency_Supplier *[suppliers], -1);
      for (i = 0; i < suppliers; ++i)
        {
          int supplier_timestamps = (i==0);
          ACE_NEW_RETURN (supplier [i],
                          Latency_Supplier (total_messages,
                                            ACE_static_cast (CORBA::Long, i),
                                            supplier_timestamps),
                          -1);
          char supplier_name [BUFSIZ];
          sprintf (supplier_name, "supplier-%d", i+1);
          // Only the first supplier timestamps its messages.
          int master = (i==0);
          if (supplier [i]->open_supplier (ec.in (),
                                           supplier_name,
                                           master) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "Supplier open failed.\n"), -1);
        }

      // Create consumers.
      ACE_NEW_RETURN (consumer, Latency_Consumer *[consumers], -1);
      for (i = 0; i < consumers; ++i)
        {
          ACE_NEW_RETURN (consumer [i], Latency_Consumer (measure_jitter), -1);
          char buf [BUFSIZ];
          sprintf (buf, "consumer-%d", i+1);

          if (consumer [i]->open_consumer (ec.in (), buf) == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "Someone was feeling introverted.\n"),
                              -1);
        }

#if defined (ACE_HAS_QUANTIFY)
      if (! measure_jitter)
        {
          ACE_DEBUG ((LM_DEBUG, "(%t) start Quantify recording\n"));
          quantify_start_recording_data ();
        }
#endif /* ACE_HAS_QUANTIFY */

      // Tell supplier(s) to generate events.
      for (i = 0; i < suppliers; ++i)
        {
          if (supplier [i]->start_generating_events () == -1)
            ACE_ERROR_RETURN ((LM_ERROR, "generate_events failed.\n"), -1);
        }

      orb->run ();

      for (i = 0; i < suppliers; ++i)
        {
          supplier [i]->print_stats ();
          delete supplier[i];
          TAO_CHECK_ENV;
        }
      delete [] supplier;

      for (i = 0; i < consumers; ++i)
        {
          consumer [i]->print_stats ();
          delete consumer [i];
          TAO_CHECK_ENV;
        }
      delete [] consumer;

      ec->destroy (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_TIMEPROBE_PRINT;
    }
  TAO_CATCHANY
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;


  return 0;
}
