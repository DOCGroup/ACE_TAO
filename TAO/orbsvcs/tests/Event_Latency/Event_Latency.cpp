// $Id$

#include <limits.h>
#if defined (quantify)
  #include <quantify.h>
#endif /* quantify */

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"
#include "ace/Profile_Timer.h"

#include "tao/Timeprobe.h"
#include "orbsvcs/Event_Utilities.h"
#include "orbsvcs/Event_Service_Constants.h"
#include "orbsvcs/Scheduler_Factory.h"
#include "orbsvcs/RtecEventChannelAdminC.h"
#include "Event_Latency.h"

static const char usage [] = "[-? |\n"
"            [-c <consumers> [4]]\n"
"            [-d directly connect all consumers/suppliers\n"
"            [-j to collect jitter statistics]\n"
"            [-m <count> of messages to send [10]]\n"
"            [-s <suppliers>, [1]]\n"
"            [-t <timeout interval>, msec [250]]]";

// Configuration parameters.
static u_int consumers = 1;
static u_int suppliers = 1;
static u_int total_messages = 1000;
static int measure_jitter = 0;
static u_int timeout_interval = 250; // msec

static int short_circuit_EC = 0;
static int shutting_down = 0;

// This is global to allow the Supplier to short ciruit the EC
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
      server->set (rt_info_,
		   1, 1, 1, 0,
		   RtecScheduler::VERY_LOW,
		   RtecScheduler::NO_QUANTUM, 1,
		   TAO_TRY_ENV);

      // Create the event that we're registering for.
      ACE_ConsumerQOS_Factory dependencies;
      dependencies.start_disjunction_group ();
      dependencies.insert_type (ACE_ES_EVENT_NOTIFICATION, rt_info_);
      dependencies.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info_);

      this->channel_admin_ = ec;

      // = Connect as a consumer.
      consumer_admin_ = 
	RtecEventChannelAdmin::ConsumerAdmin::_duplicate(channel_admin_->for_consumers (TAO_TRY_ENV));
      TAO_CHECK_ENV;
      suppliers_ =
	RtecEventChannelAdmin::ProxyPushSupplier::_duplicate(consumer_admin_->obtain_push_supplier (TAO_TRY_ENV));
      TAO_CHECK_ENV;

      suppliers_->connect_push_consumer (this,
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
  ACE_TIMEPROBE ("push event to consumer");
  
  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }
  // ACE_DEBUG ((LM_DEBUG, "%d event(s)\n", events.length ()));

#if defined (quantify)
  // If measuring jitter, just Quantify the supplier-consumer path.
  if (measure_jitter)
    {
      quantify_stop_recording_data ();
    }
#endif /* quantify */

  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      if (events[i].type_ == ACE_ES_EVENT_SHUTDOWN)
	{
	  ACE_DEBUG ((LM_DEBUG, "Latency Consumer: received shutdown event\n"));
	  this->shutdown ();
	}
      else
	{
	  if (measure_jitter_)
	    {
	      // @@ TOTAL HACK
	      ACE_hrtime_t creation;
	      ACE_OS::memcpy (&creation, &events[i].creation_time_,
			      sizeof (creation));

	      ACE_hrtime_t ec_recv;
	      ACE_OS::memcpy (&ec_recv, &events[i].ec_recv_time_,
			      sizeof (ec_recv));

	      ACE_hrtime_t ec_send;
	      ACE_OS::memcpy (&ec_send, &events[i].ec_send_time_,
			      sizeof (ec_send));

	      const ACE_hrtime_t now = ACE_OS::gethrtime ();
	      const ACE_hrtime_t elapsed = now - creation;
	      // Note: the division by 1 provides transparent support of
	      // ACE_U_LongLong.
	      ACE_Time_Value latency (elapsed / ACE_ONE_SECOND_IN_NSECS,
				      (elapsed / 1 % ACE_ONE_SECOND_IN_NSECS) / 1000);

	      const ACE_hrtime_t to_ec_nsecs = ec_recv - creation;
	      ACE_Time_Value to_ec (to_ec_nsecs / ACE_ONE_SECOND_IN_NSECS,
				    (to_ec_nsecs / 1 % ACE_ONE_SECOND_IN_NSECS) / 1000);
	      
	      const ACE_hrtime_t in_ec_nsecs = ec_send - ec_recv;
	      ACE_Time_Value in_ec (in_ec_nsecs / ACE_ONE_SECOND_IN_NSECS,
				    (in_ec_nsecs / 1 % ACE_ONE_SECOND_IN_NSECS) / 1000);

	      const ACE_hrtime_t from_ec_nsecs = now - ec_send;
	      ACE_Time_Value from_ec (from_ec_nsecs / ACE_ONE_SECOND_IN_NSECS,
				      (from_ec_nsecs / 1 % ACE_ONE_SECOND_IN_NSECS) / 1000);

	      if (! shutting_down)
		{
		  ++total_pushes_;
		  if (min_latency_ > latency) min_latency_ = latency;
		  if (max_latency_ < latency) max_latency_ = latency;
		  total_latency_ += latency;
		  if (min_to_ec_ > to_ec) min_to_ec_ = to_ec;
		  if (max_to_ec_ < to_ec) max_to_ec_ = to_ec;
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

void
Latency_Consumer::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s shutting down.\n", entry_point ()));

  TAO_TRY
    {
      // Disconnect from the push supplier.
      suppliers_->disconnect_push_supplier (TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::release (suppliers_);

      ACE_DEBUG ((LM_DEBUG, "@@ we should shutdown here!!!\n"));
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
	(total_latency_.sec () * 1000000.0 +total_latency_.usec ()) / total_pushes_ / 1000.0;
      ACE_DEBUG ((LM_TRACE,
                  "%s: Latency in msec (min/max/avg): "
                    "%5.3f/%5.3f/%5.3f\n",
                  entry_point (), lat_min, lat_max, lat_avg));

      double to_ec_min =
	(min_to_ec_.sec () * 1000000.0 + min_to_ec_.usec ()) / 1000.0;
      double to_ec_max =
	(max_to_ec_.sec () * 1000000.0 + max_to_ec_.usec ()) / 1000.0;
      double to_ec_avg = 
        (sum_to_ec_.sec () * 1000000.0 + sum_to_ec_.usec ()) / total_pushes_ / 1000.0;
      ACE_DEBUG ((LM_TRACE,
                  "%s: From test to EC (min/max/avg): "
		  "%5.3f/%5.3f/%5.3f\n",
                  entry_point (), to_ec_min, to_ec_max, to_ec_avg));

      double in_ec_min =
	(min_in_ec_.sec () * 1000000.0 + min_in_ec_.usec ()) / 1000.0;
      double in_ec_max =
	(max_in_ec_.sec () * 1000000.0 + max_in_ec_.usec ()) / 1000.0;
      double in_ec_avg = 
        (sum_in_ec_.sec () * 1000000.0 + sum_in_ec_.usec ()) / total_pushes_ / 1000.0;
      ACE_DEBUG ((LM_TRACE,
                  "%s: In the EC (min/max/avg): "
		  "%5.3f/%5.3f/%5.3f\n",
                  entry_point (), in_ec_min, in_ec_max, in_ec_avg));

      double from_ec_min =
	(min_from_ec_.sec () * 1000000.0 + min_from_ec_.usec ()) / 1000.0;
      double from_ec_max =
	(max_from_ec_.sec () * 1000000.0 + max_from_ec_.usec ()) / 1000.0;
      double from_ec_avg = 
        (sum_from_ec_.sec () * 1000000.0 + sum_from_ec_.usec ()) / total_pushes_ / 1000.0;
      ACE_DEBUG ((LM_TRACE,
                  "%s: From EC to test (min/max/avg): "
		  "%5.3f/%5.3f/%5.3f\n",
                  entry_point (), from_ec_min, from_ec_max, from_ec_avg));

    }
}


// ************************************************************

Latency_Supplier::Supplier::Supplier (Latency_Supplier* impl)
  :  impl_ (impl)
{
}

void Latency_Supplier::Supplier::disconnect_push_supplier
  (CORBA::Environment &_env)
{
  this->impl_->disconnect_push_supplier (_env);
}

Latency_Supplier::Consumer::Consumer (Latency_Supplier* impl)
  :  impl_ (impl)
{
}

void Latency_Supplier::Consumer::disconnect_push_consumer
  (CORBA::Environment &_env)
{
  this->impl_->disconnect_push_consumer (_env);
}

void Latency_Supplier::Consumer::push
  (const RtecEventComm::EventSet &events,
   CORBA::Environment &_env)
{
  this->impl_->push (events, _env);
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
    supplier_ (new Supplier (this)),
    consumer_ (new Consumer (this))
{
}

Latency_Supplier::~Latency_Supplier (void)
{
  delete this->consumer_;
  delete this->supplier_;
}

int
Latency_Supplier::open_supplier (RtecEventChannelAdmin::EventChannel_ptr ec,
				 const char *name, int master)
{
  this->entry_point (name); 
  master_ = master;
  TAO_TRY
    {
      RtecScheduler::Scheduler_ptr server =
	ACE_Scheduler_Factory::server ();

      rt_info_ = 
	server->create (name, TAO_TRY_ENV);
      
      server->set (rt_info_, 1, 1, 1, timeout_interval * 10000,
		   RtecScheduler::VERY_LOW,
		   RtecScheduler::NO_QUANTUM, 1,
		   TAO_TRY_ENV);

#if 0
      SUPPLIER_NS::RegisterService
        (entry_point (),
         //      ACE_Naming::NOBJECT, 
         NOBJECT, this);
#endif /* 0 */

      ACE_SupplierQOS_Factory publications;
      publications.insert (supplier_id_,
			   ACE_ES_EVENT_NOTIFICATION,
			   rt_info_, 1);
      publications.insert (supplier_id_,
			   ACE_ES_EVENT_SHUTDOWN,
			   rt_info_, 1);

      this->channel_admin_ = ec;

      // = Connect as a supplier.
      supplier_admin_ =
	RtecEventChannelAdmin::SupplierAdmin::_duplicate(channel_admin_->for_suppliers (TAO_TRY_ENV));
      TAO_CHECK_ENV;
      consumers_ =
	RtecEventChannelAdmin::ProxyPushConsumer::_duplicate(supplier_admin_->obtain_push_consumer (TAO_TRY_ENV));
      TAO_CHECK_ENV;

      consumers_->connect_push_supplier (this->supplier_,
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
  ACE_DEBUG ((LM_DEBUG, "Supplier-consumer received disconnect from channel.\n"));
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
  test_start_time_.set (now / 1000000000, (now / 1 % 1000000000) / 1000);

  TAO_TRY
    {
      ACE_ConsumerQOS_Factory dependencies;
      dependencies.start_disjunction_group ();
      dependencies.insert_time (ACE_ES_EVENT_INTERVAL_TIMEOUT,
				timeout_interval * 10000, 
				rt_info_);
      if (!master_)
	dependencies.insert_type (ACE_ES_EVENT_SHUTDOWN, rt_info_);

      // = Connect as a consumer.
      consumer_admin_ = 
	RtecEventChannelAdmin::ConsumerAdmin::_duplicate(channel_admin_->for_consumers (TAO_TRY_ENV));
      TAO_CHECK_ENV;
      suppliers_ =
	RtecEventChannelAdmin::ProxyPushSupplier::_duplicate(consumer_admin_->obtain_push_supplier (TAO_TRY_ENV));
      TAO_CHECK_ENV;

      suppliers_->connect_push_consumer (this->consumer_,
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
			CORBA::Environment & _env)
{
  // ACE_DEBUG ((LM_DEBUG, "Latency_Supplier::push - "));
  
  if (events.length () == 0)
    {
      // ACE_DEBUG ((LM_DEBUG, "no events\n"));
      return;
    }

  // ACE_DEBUG ((LM_DEBUG, "%d event(s)\n", events.length ()));

  for (CORBA::ULong i = 0; i < events.length (); ++i)
    {
      if (!master_ && events[i].type_ == ACE_ES_EVENT_SHUTDOWN)
	{
	  ACE_DEBUG ((LM_DEBUG, "Latency Supplier: received shutdown event\n"));
	  this->shutdown ();
	}
      else if (events[i].type_ == ACE_ES_EVENT_INTERVAL_TIMEOUT)
	{
	  // Create the event to send.
	  RtecEventComm::Event event;
	  event.source_ = supplier_id_;
	  event.type_ = ACE_ES_EVENT_NOTIFICATION;
	  ++total_sent_;

	  if (timestamp_)
	    {
	      // @@ David, event.time_ is now a long.  I'm not sure if
	      // this calculation is correct now. For the moment beign
	      // I use a global variable instead.
	      // const ACE_hrtime_t now = ACE_OS::gethrtime ();
	      // event.time_.set (now / ACE_ONE_SECOND_IN_NSECS,
	      // (now % ACE_ONE_SECOND_IN_NSECS) / 1000);

	      // @@ TOTAL HACK
	      // event_push_time = ACE_OS::gethrtime ();
	      ACE_hrtime_t t = ACE_OS::gethrtime ();
	      ACE_OS::memcpy (&event.creation_time_, &t,
			      sizeof (event.creation_time_));
	    }

	  // @@ ACE_TIMEPROBE_RESET;
	  // @@ ACE_TIMEPROBE ("start with new event in Supplier");

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
#if defined (quantify)
		  // If measuring jitter, just Quantify the supplier-consumer path.
		  if (measure_jitter)
		    {
		      quantify_start_recording_data ();
		    }
#endif /* quantify */

		  ACE_TIMEPROBE ("  supplier starts pushing event");

		  RtecEventComm::EventSet events (1);
		  events.length (1);
		  events[0] = event;
		  consumers_->push (events, TAO_TRY_ENV);

		  ACE_TIMEPROBE ("  supplier ends pushing event");
		}
	      TAO_CHECK_ENV;
	    } 
	  TAO_CATCH (RtecEventComm::Disconnected, d)
	    {
	      ACE_ERROR ((LM_ERROR, "(%t) Latency_Supplier::push: disconnected.\n"));
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
	  ACE_ERROR ((LM_ERROR, "(%t) %s received unexpected events: ", 
		      entry_point ()));
	  // ::dump_sequence (events);
	  return;
	}
    }
}

void
Latency_Supplier::shutdown (void)
{
  shutting_down = 1;

  #if defined (quantify)
    // Need to stop recording here even if testing for jitter, because
    // recording is still probably enabled.
    quantify_stop_recording_data ();
    if (! measure_jitter)
      {
        ACE_DEBUG ((LM_DEBUG, "(%t) stopped Quantify recording\n"));
      }
  #endif /* quantify */

  const ACE_hrtime_t now = ACE_OS::gethrtime ();
  test_stop_time_.set (now / ACE_ONE_SECOND_IN_NSECS,
		       (now / 1 % ACE_ONE_SECOND_IN_NSECS) / 1000);

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
	  event.source_ = supplier_id_;
	  event.type_ = ACE_ES_EVENT_SHUTDOWN;

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
	  // @@ TODO: Do this portably (keeping the ORB_ptr returned from
	  // ORB_init)
	  channel_admin_->destroy (TAO_TRY_ENV);
	  TAO_CHECK_ENV;

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
              (double) elapsed / total_sent_ / 1000.0));
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
          consumers = (u_int) temp;
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
          total_messages = (u_int) temp;
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
          suppliers = (u_int) temp;
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
			 argv[0], char(opt),
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
          ACE_OS::exit (-1);
        }
    }

  u_int i;

  TAO_TRY
    {
      // Initialize ORB.
      CORBA::ORB_ptr orb = 
	CORBA::ORB_init (argc, argv, "internet", TAO_TRY_ENV);
      TAO_CHECK_ENV;

      CORBA::POA_ptr poa =
	orb->POA_init(argc, argv, "POA");

      if (get_options (argc, argv))
	ACE_OS::exit (-1);



      CORBA::Object_ptr objref =
	orb->resolve_initial_references ("NameService");
      TAO_CHECK_ENV;
      CosNaming::NamingContext_var naming_context = 
        CosNaming::NamingContext::_narrow (objref, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      ACE_Scheduler_Factory::use_config (naming_context.ptr ());

      // Allocate the timeprobe instance now, so we don't measure
      // the cost of doing it later.
      ACE_TIMEPROBE_RESET;
      
      CosNaming::Name channel_name (1);
      channel_name[0].id = CORBA::string_dup ("EventService");
      channel_name.length (1);
      
      CORBA::Object_ptr ec_ptr = 
	naming_context->resolve (channel_name, TAO_TRY_ENV);
      TAO_CHECK_ENV;
      RtecEventChannelAdmin::EventChannel_var ec =
	RtecEventChannelAdmin::EventChannel::_narrow (ec_ptr, TAO_TRY_ENV);
      TAO_CHECK_ENV;

      // Create supplier(s).
      Latency_Supplier **supplier;
      ACE_NEW_RETURN (supplier, Latency_Supplier *[suppliers], -1);
      for (i = 0; i < suppliers; ++i)
	{
	  int supplier_timestamps = (i==0);
	  ACE_NEW_RETURN (supplier [i],
			  Latency_Supplier (total_messages,
					    measure_jitter,
					    supplier_timestamps),
			  -1);
	  char supplier_name [BUFSIZ];
	  sprintf (supplier_name, "supplier-%d", i+1);
	  // Only the first supplier timestamps its messages.
	  int master = (i==0);
	  if (supplier [i]->open_supplier (ec.ptr (),
					   supplier_name,
					   master) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, "Supplier open failed.\n"), -1);
	}

      // Create consumers.
      ACE_NEW_RETURN (consumer, Latency_Consumer *[consumers], -1);
      for (i = 0; i < consumers; ++i)
	{
	  ACE_NEW_RETURN (consumer [i], Latency_Consumer (measure_jitter), -1);
	  RtecEventComm::PushConsumer::_duplicate (consumer[i]);
	  char buf [BUFSIZ];
	  sprintf (buf, "consumer-%d", i+1);

	  if (consumer [i]->open_consumer (ec.ptr (), buf) == -1)
	    ACE_ERROR_RETURN ((LM_ERROR, "Someone was feeling introverted.\n"),
			      -1);
	}

  #if defined (quantify)
      if (! measure_jitter)
	{
	  ACE_DEBUG ((LM_DEBUG, "(%t) start Quantify recording\n"));
	  quantify_start_recording_data ();
	}
  #endif /* quantify */

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

      ACE_TIMEPROBE_PRINT;
      ACE_TIMEPROBE_FINI;
    }
  TAO_CATCH (CORBA::SystemException, sys_ex)
    {
      TAO_TRY_ENV.print_exception ("SYS_EX");
    }
  TAO_ENDTRY;


  return 0;
}
