// $Id$

#if defined (quantify)
  #include <quantify.h>
#endif /* quantify */
#include <limits.h>

#include "ace/Get_Opt.h"
#include "ace/Sched_Params.h"

#include "Event_Service_Constants.h"
#include "Event_Utilities.h"
#include "Utilization.h"

static const char usage [] = "[-? |\n"
                         "            [-c <consumers> [1]]\n"
                         "            [-d directly connect all consumers/suppliers\n"
                         "            [-j to collect jitter statistics]\n"
                         "            [-m <count> of messages to send [10]]\n"
                         "            [-s <suppliers>, [1]]\n"
                         "            [-t <timeout interval>, msec [100]]]";

// Configuration parameters.
static u_int consumers = 1;
static u_int suppliers = 1;
static u_int total_messages = 10;
static int measure_jitter = 0;
static u_int timeout_interval = 100; // msec

static int short_circuit_EC = 0;
static int shutting_down = 0;

// This is global to allow the Supplier to short ciruit the EC
// and talk directly to consumers.  For testing only :-)
static Utilization_Consumer **consumer;

// Hold the Scavenger up until we're ready to run . . .
ACE_Barrier start_scavenger (2);
u_int started_scavenger = 0;

// ************************************************************

Utilization_Consumer::Utilization_Consumer (const int measure_jitter)
  : measure_jitter_ (measure_jitter),
    iterations_ (0),
    min_utilization_ (INT_MAX),
    max_utilization_ (INT_MIN),
    total_utilization_ (0),
    total_pushes_ (0)
{
}

int
Utilization_Consumer::open_consumer (const char *my_name, const char *)
{
  // We look up name for our qos requirements.
  ACE_Scheduler::status_t result =
    ACE_Scheduler::instance ().get_rt_info (my_name, rt_info_);

  switch (result)
    {
    case ACE_Scheduler::SUCCEEDED :
      // Found.
      break;

    case ACE_Scheduler::UNKNOWN_TASK :
      rt_info_->set ((timeout_interval - 1) * 10000 /* 100 ns/msec */,
                     (timeout_interval - 1) * 10000 /* 100 ns/msec */,
                     (timeout_interval - 1) * 10000 /* 100 ns/msec */,
                     0);

      // Not found, but created.  We'll leave it blank for now.
      break;

    case ACE_Scheduler::FAILED :
    default :
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR, "Utilization_Consumer: get_rt_info failed.\n"), -1);
    }

  ACE_TRY
    {
      // Look up the event service in the name server.
      channel_admin_ = EVENT_CHANNEL_NS::BindService ("Event Service");
      if (channel_admin_ == 0)
        {
          ACE_ERROR ((LM_ERROR, "%p bind failed.\n", "Utilization_Consumer::open"));
          return -1;
        }

      // Create the event that we're registering for.
      ACE_ConsumerQOS_Factory dependencies;
      dependencies.start_disjunction_group ();
      dependencies.insert (ACE_ES_EVENT_NOTIFICATION, rt_info_);
      dependencies.insert (ACE_ES_EVENT_SHUTDOWN, rt_info_);

      // = Connect as a consumer.
      consumer_admin_ = channel_admin_->for_consumers (ACE_TRY_ENV);
      ACE_CHECK_ENV;
      suppliers_ = consumer_admin_->obtain_push_supplier (ACE_TRY_ENV);
      ACE_CHECK_ENV;
      suppliers_->connect_push_consumer (this,
                                         dependencies.get_ConsumerQOS (),
                                         ACE_TRY_ENV);
      ACE_CHECK_ENV;
    }
  ACE_CATCH (const ACE_EventChannel::SUBSCRIPTION_ERROR, se)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Utilization_Consumer::open: subscribe failed.\n"), -1);
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Utilization_Consumer::open: unexpected exception.\n"), -1);
    }
  ACE_ENDTRY;

  return 0;
}

void
Utilization_Consumer::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, "Consumer received disconnect from channel.\n"));
}

void 
Utilization_Consumer::push (const CosEventComm::EventSet &events,
                        CORBA::Environment &)
{
  ACE_TIMEPROBE ("push event to consumer");

  #if defined (quantify)
    // If measuring jitter, just Quantify the supplier-consumer path.
    if (measure_jitter)
      {
        quantify_stop_recording_data ();
      }
  #endif /* quantify */

  if (events[0].type_ == ACE_ES_EVENT_SHUTDOWN)
    this->shutdown ();
  else
    {
      if (measure_jitter_)
        {
          const ACE_hrtime_t now = ACE_OS::gethrtime ();
          // Note: the division by 1 provides transparent support of
          // ACE_U_LongLong.
          ACE_Time_Value utilization (now / 1000000000,
                                  (now / 1 % 1000000000) / 1000);
          utilization -= ACE_Time_Value (events[0].time_ / 1000000000,
                                     (events[0].time_ / 1 % 1000000000) / 1000);

          if (! shutting_down)
            {
              if (min_utilization_ > utilization) min_utilization_ = utilization;
              if (max_utilization_ < utilization) max_utilization_ = utilization;
              total_utilization_ += utilization;
              ++total_pushes_;
            }
        }

      ACE_TIMEPROBE_PRINT;

      // Chew up almost the entire timeout interval on the CPU.
      u_long n = 1279ul;  /* takes about 40.2 usecs on a 167 MHz Ultra2 */

      for (u_int i = 0; i < (timeout_interval * 1000 / 40.2); ++i)
        {
          ACE::is_prime (n,
                         2ul /* min_factor */,
                         n/2 /* max_factor */);
          iterations_ += 1;      // ACE_U_LongLong doesn't have ++
        }
    }
}

void
Utilization_Consumer::shutdown (void)
{
  ACE_DEBUG ((LM_DEBUG, "(%t) %s shutting down.\n", entry_point ()));

  ACE_TRY
    {
      // Disconnect from the push supplier.
      suppliers_->disconnect_push_supplier (ACE_TRY_ENV);
      ACE_CHECK_ENV;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR,
                 "(%t) %s Utilization_Consumer::shutdown: unexpected exception.\n",
                 entry_point ()));
    }
  ACE_ENDTRY;
}


void
Utilization_Consumer::print_stats () /* const */
{
  if (measure_jitter_)
    {
      ACE_DEBUG ((LM_TRACE,
                  "%s: minimum, maximum, average event utilization (msec): "
                    "%5.3f, %5.3f, %5.3f\n",
                  entry_point (),
                  (min_utilization_.sec () * 1000000.0 + min_utilization_.usec ()) /
                    1000.0,
                  (max_utilization_.sec () * 1000000.0 + max_utilization_.usec ()) /
                    1000.0,
                  (total_utilization_.sec () * 1000000.0 +
                   total_utilization_.usec ()) / total_pushes_ / 1000.0));
    }
  ACE_OS::fprintf (stderr, "Consumer processed %llu iterations\n",
                   iterations_ / 1ul);
}


// ************************************************************

Utilization_Supplier::Utilization_Supplier (const u_int total_messages,
                                    const int timestamp)
  : total_messages_ (total_messages),
    timestamp_ (timestamp),
    total_sent_ (0),
    master_ (0)
{
}

int
Utilization_Supplier::open_supplier (const char *name, int master)
{
  master_ = master;

  // We look up name for our qos requirements.
  ACE_Scheduler::status_t result =
    ACE_Scheduler::instance ().get_rt_info (name, rt_info_);

  switch (result)
    {
    case ACE_Scheduler::SUCCEEDED :
      // Found.
      break;

    case ACE_Scheduler::UNKNOWN_TASK :
      // Not found, but created.  We want to be notified 4 times a second.
      rt_info_->set (1,1,1, timeout_interval * 10000 /* 100 ns/msec */,
		     ACE_Scheduler::RT_Info::VERY_LOW,
		     ACE_Scheduler::RT_Info::NO_QUANTUM);
      break;

    case ACE_Scheduler::FAILED :
    default :
      // Error.
      ACE_ERROR_RETURN ((LM_ERROR, "Utilization_Supplier: get_rt_info failed.\n"), -1);
    }

  ACE_TRY
    {
      SUPPLIER_NS::RegisterService
        (entry_point (),
         //      ACE_Naming::NOBJECT, 
         NOBJECT, this);

      // Look up the event service in the name server.
      channel_admin_ = EVENT_CHANNEL_NS::BindService ("Event Service");
      if (channel_admin_ == 0)
        ACE_ERROR_RETURN ((LM_ERROR, "%p bind failed.\n",
                           "Utilization_Supplier::open"), -1);

      ACE_SupplierQOS_Factory publications;
      publications.insert (ACE_ES_EVENT_NOTIFICATION, rt_info_, 1);
      publications.insert (ACE_ES_EVENT_SHUTDOWN, rt_info_, 1);

      // = Connect as a supplier.
      supplier_admin_ = channel_admin_->for_suppliers (ACE_TRY_ENV);
      ACE_CHECK_ENV;
      consumers_ = supplier_admin_->obtain_push_consumer (ACE_TRY_ENV);
      ACE_CHECK_ENV;
      consumers_->connect_push_supplier (this,
                                         publications.get_SupplierQOS (),
                                         ACE_TRY_ENV);
      ACE_CHECK_ENV;
    }
  ACE_CATCH (const ACE_EventChannel::SUBSCRIPTION_ERROR, se)
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Utilization_Supplier::open: subscribe failed.\n"), -1);
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR,
                         "Utilization_Supplier::open: unexpected exception.\n"), -1);
    }
  ACE_ENDTRY;

  return 0;
}

void
Utilization_Supplier::disconnect_push_consumer (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, "Supplier-consumer received disconnect from channel.\n"));
}

void
Utilization_Supplier::disconnect_push_supplier (CORBA::Environment &)
{
  ACE_DEBUG ((LM_DEBUG, "Supplier received disconnect from channel.\n"));
}

int
Utilization_Supplier::start_generating_events (void)
{
  const ACE_hrtime_t now = ACE_OS::gethrtime ();
  test_start_time_.set (now / 1000000000, (now / 1 % 1000000000) / 1000);

  ACE_TRY
    {
      ACE_ConsumerQOS_Factory dependencies;
      dependencies.start_disjunction_group ();
      dependencies.insert (ACE_ES_EVENT_INTERVAL_TIMEOUT,
			   timeout_interval * 10000 /* 100 ns/msec */,
			   rt_info_);
      if (!master_)
	dependencies.insert (ACE_ES_EVENT_SHUTDOWN, rt_info_);

      // = Connect as a consumer.
      consumer_admin_ = channel_admin_->for_consumers (ACE_TRY_ENV);
      ACE_CHECK_ENV;
      suppliers_ = consumer_admin_->obtain_push_supplier (ACE_TRY_ENV);
      ACE_CHECK_ENV;
      suppliers_->connect_push_consumer (this,
                                         dependencies.get_ConsumerQOS (),
                                         ACE_TRY_ENV);
      ACE_CHECK_ENV;
    }
  ACE_CATCHANY
    {
      ACE_ERROR_RETURN ((LM_ERROR, 
                         "Utilization_Supplier::generate_events:"
                         " unexpected exception.\n"), -1);
    }
  ACE_ENDTRY;

  return 0;
}

void 
Utilization_Supplier::push (const CosEventComm::EventSet &events,
			CORBA::Environment &)
{
  if (!master_ && events[0].type_ == ACE_ES_EVENT_SHUTDOWN)
    this->shutdown ();
  else if (events[0].type_ == ACE_ES_EVENT_INTERVAL_TIMEOUT)
    {
      // Create the event to send.
      CosEventComm::Event event;
      event.source_ = this;
      event.type_ = ACE_ES_EVENT_NOTIFICATION;
      ++total_sent_;

      if ( ! started_scavenger )
        {
          started_scavenger = 1;

          // Start the Scavenger.
          start_scavenger.wait ();
        }

      if (timestamp_)
	{
	  const ACE_hrtime_t now = ACE_OS::gethrtime ();
	  // David, time_ is now a long.  I'm not sure if this calculation correct now.
	  //event.time_.set (now / 1000000000, (now % 1000000000) / 1000);
	  event.time_ = now;
	}

      ACE_TIMEPROBE_RESET;
      ACE_TIMEPROBE ("start with new event in Supplier");

      ACE_TRY
	{
	  if (short_circuit_EC)
	    {
	      for (u_int cons = 0; cons < consumers; ++cons)
		{
		  // This constructor is fast.
		  const CosEventComm::EventSet es (1, 1, &event);
		  consumer [cons]->push (es, ACE_TRY_ENV);
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

	      ACE_TIMEPROBE ("time to read high-res clock and "
			     "compare an int with 0");
	      consumers_->push (event, ACE_TRY_ENV);
	    }
	  ACE_CHECK_ENV;
	} 
      ACE_CATCH (CosEventComm::Disconnected, d)
	{
	  ACE_ERROR ((LM_ERROR, "(%t) Utilization_Supplier::push: disconnected.\n"));
	}
      ACE_CATCHANY
	{
	  ACE_ERROR ((LM_ERROR, "(%t) %s Utilization_Supplier::push:"
		      " unexpected exception.\n",
		      entry_point ()));
	}
      ACE_ENDTRY;

      // Check if we're done.
      if (master_ && (total_sent_ >= total_messages_))
	this->shutdown ();
    }
  else
    {
      ACE_ERROR ((LM_ERROR, "(%t) %s received unexpected events: ", 
                  entry_point ()));
      ::dump_sequence (events);
      return;
    }
}

void
Utilization_Supplier::shutdown (void)
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
  test_stop_time_.set (now / 1000000000, (now / 1 % 1000000000) / 1000);

  static int total_iterations = 1;
  if (--total_iterations > 0)
    {
      total_sent_ = 0;
      return;
    }

  ACE_TRY
    {
      if (master_)
	{
	  // Create the shutdown message.
	  CosEventComm::Event event;
	  event.source_ = this;
	  event.type_ = ACE_ES_EVENT_SHUTDOWN;

	  // Push the shutdown event.
	  consumers_->push (event, ACE_TRY_ENV);
	  ACE_CHECK_ENV;
	}

      // Disconnect from the channel.
      consumers_->disconnect_push_consumer (ACE_TRY_ENV);
      ACE_CHECK_ENV;

      // Disconnect from the push supplier.
      suppliers_->disconnect_push_supplier (ACE_TRY_ENV);
      ACE_CHECK_ENV;

      if (master_)
	channel_admin_->destroy (ACE_TRY_ENV);
      ACE_CHECK_ENV;
    }
  ACE_CATCHANY
    {
      ACE_ERROR ((LM_ERROR, "(%t) %s Utilization_Supplier::shutdown:"
                  " unexpected exception.\n",
                  entry_point ()));
    }
  ACE_ENDTRY;
}


void
Utilization_Supplier::print_stats () /* const */
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
// class Scavenger
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
Scavenger::Scavenger (const int priority)
  : ACE_Task<ACE_NULL_SYNCH> (ACE_Thread_Manager::instance ()),
    priority_ (priority),
    iterations_ (0)
{
}

Scavenger::~Scavenger ()
{
}

int
Scavenger::open (void *arg)
{
  ACE_UNUSED_ARG (arg);

  long flags = THR_BOUND | THR_SCHED_FIFO;

  // Become an active object.
  if (this->activate (flags, 1, 0, this->priority_) == -1)
    {
      ACE_DEBUG ((LM_ERROR,
                  "(%t) task activation at priority %d failed, exiting\n%a",
                  priority_, -1));
    }

  return 0;
}

int
Scavenger::svc (void)
{
  // Make sure that its thread has the proper priority.
  ACE_hthread_t thr_handle;
  ACE_Thread::self (thr_handle);
  int prio;

  if (ACE_Thread::getprio (thr_handle, prio) == -1)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "getprio failed"), -1);
    }

  ACE_DEBUG ((LM_DEBUG, "(%t) SCAVENGER prio = %d, priority_ = %d\n", 
	      prio, this->priority_));

  if (this->priority_ != prio)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "improper priority: %d\n", prio), -1);
    }

  // Wait here until the Consumer is running.
  start_scavenger.wait ();

  for (u_long i = 0;
       ! shutting_down &&
         i < timeout_interval * 1000 / 40.2 * total_messages * 3;
       ++i)
    {
      u_long n = 1279ul;  /* takes about 40.2 usecs */
      ACE::is_prime (n,
                     2ul /* min_factor */,
                     n/2 /* max_factor */);

      iterations_ += 1;      // ACE_U_LongLong doesn't have ++
    }

  if (! shutting_down)
    {
      ACE_OS::fprintf (stderr, "Scavenger timed out!!!!\n");
    }

  return 0;
}

void
Scavenger::print_status ()
{
  ACE_OS::fprintf (stderr, "Scavenger processed %llu iterations\n",
                   iterations_ / 1ul);
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
  ACE_Get_Opt get_opt (argc, argv, "c:djm:s:t:?");
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
          cerr << argv [0] << ": number of consumers must be > 0" << endl;
          return 1;
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
          cerr << argv [0] << ": count must be > 0" << endl;
          return 1;
        }
      break;
    case 's':
      if ((temp = ACE_OS::atoi (get_opt.optarg)) > 0)
        {
          suppliers = (u_int) temp;
        }
      else
        {
          cerr << argv [0] << ": number of suppliers must be > 0" << endl;
          return 1;
        }
      break;
    case 't':
      if (ACE_OS::atoi (get_opt.optarg) >= 0)
        {
          timeout_interval = ACE_OS::atoi (get_opt.optarg);
        }
      else
        {
          cerr << argv [0] << ": count must be >= 0" << endl;
          return 1;
        }
      break;
    case '?':
      cout << "usage: " << argv [0] << " " << usage << endl;
      ACE_OS::exit (0);
      break;
    default:
      cerr << argv [0] << ": unknown arg, " << (char) opt << endl;
      cerr << "usage: " << argv [0] << " " << usage << endl;
      return 1;
    }
  }

  switch (argc - get_opt.optind) {
  case 0:
    // OK
    break;
  default:
    cerr << argv [0] << ": too many arguments" << endl;
    cerr << "usage: " << argv [0] << " " << usage << endl;
    return 1;
  }

  return 0;
}

// ************************************************************
// This function tells the scheduler to generate a schedule.  It will
// only call it once, so this function can be called multiple times.
static void
Generate_Schedule (void)
{
  static int once = 0;
  static ACE_Thread_Mutex m;

  if (once == 0)
    {
      ACE_Guard<ACE_Thread_Mutex> ace_mon (m);

      // Double-check.
      if (once == 0)
	{
	  once = 1;
	  ACE_Scheduler::status_t result = ACE_Scheduler::instance ().schedule ();

	  if (result != ACE_Scheduler::SUCCEEDED)
	    ACE_ERROR ((LM_ERROR, "ACE_Scheduler::schedule failed.\n"));
	  else
            {
              if (Scheduler_Runtime::config ())
                {
	          ACE_DEBUG ((LM_ERROR, "ACE_Scheduler::schedule succeeded.\n"));
                }
            }
	}
    }
}

///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
// function main
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
int
main (int argc, char *argv [])
{
  int min_priority = ACE_Sched_Params::priority_min (ACE_SCHED_FIFO,
                                                     ACE_SCOPE_THREAD);

  Scavenger scavenger (min_priority);

  if (ACE_OS::sched_params (
        ACE_Sched_Params (
          ACE_SCHED_FIFO,
          ACE_Sched_Params::next_priority (ACE_SCHED_FIFO,
                                           min_priority,
                                           ACE_SCOPE_PROCESS),
          ACE_SCOPE_PROCESS)) != 0)
    {
      if (ACE_OS::last_error () == EPERM)
        {
          ACE_DEBUG ((LM_MAX, "Utilization: user is not superuser, "
                              "so remain in time-sharing class\n"));
        }
      else
        {
          ACE_DEBUG ((LM_ERROR, "%p\n", "Utilization"));
          ACE_OS::exit (-1);
        }
    }

  u_int i;

  if (get_options (argc, argv)) ACE_OS::exit (-1);

  ACE_Scheduler::instance ().output_level (1);

  // Allocate the timeprobe instance now, so we don't measure
  // the cost of doing it later.
  ACE_TIMEPROBE_RESET;

  // Set the scheduler output name.
  char *orb_argv[3] = { "main", "Scheduler_Utilization_Runtime.cpp", 0 };
  int orb_argc = 3;

  // Initialize ORB.
  UPSingleProcessorOrb::ORB_init (orb_argc, orb_argv, "the_ORB");

  // Register Event Service with Naming Service.
  CosEventChannelAdmin::EventChannel_ptr ec;
  ACE_NEW_RETURN (ec, ACE_EventChannel, -1);

  EVENT_CHANNEL_NS::RegisterService ("Event Service", NOBJECT, ec);

  // Create supplier(s).
  Utilization_Supplier **supplier;
  ACE_NEW_RETURN (supplier, Utilization_Supplier *[suppliers], -1);
  for (i = 0; i < suppliers; ++i)
   {
     ACE_NEW_RETURN (supplier [i],
                     Utilization_Supplier (total_messages, measure_jitter),
                     -1);
      char supplier_name [BUFSIZ];
      sprintf (supplier_name, "supplier-%d", i+1);
      if (supplier [i]->open_supplier (supplier_name, int (i == 0)) == -1)
        ACE_ERROR_RETURN ((LM_ERROR, "Supplier open failed.\n"), -1);
   }

  // Create consumers.
  ACE_NEW_RETURN (consumer, Utilization_Consumer *[consumers], -1);
  for (i = 0; i < consumers; ++i)
    {
      ACE_NEW_RETURN (consumer [i], Utilization_Consumer (measure_jitter), -1);
      char buf [BUFSIZ];
      sprintf (buf, "consumer-%d", i+1);

      if ((consumer [i]->open_consumer (buf,
                               supplier [i % suppliers]->entry_point ()) ==
           -1))
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


  scavenger.open ();

  ACE_OS::sleep (1);

  // Enter the main event loop.
  UPSingleProcessorOrb::Instance ()->impl_is_ready ();

  scavenger.print_status ();

  // Graceful ORB shutdown.
  UPSingleProcessorOrb::Instance ()->Close ();

  ::Generate_Schedule ();

  for (i = 0; i < suppliers; ++i)
    {
      supplier [i]->print_stats ();
      delete supplier [i];
    }
  delete [] supplier;

  for (i = 0; i < consumers; ++i)
    {
      consumer [i]->print_stats ();
      delete consumer [i];
    }
  delete [] consumer;

  delete ec;

  ACE_TIMEPROBE_FINI;

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)
# if ! defined (__ACE_INLINE__)
    template class UUORB_UtilitiesT<ACE_AbstractEventChannel>;
    template class UUORB_UtilitiesT<ACE_AbstractPushSupplier>;
# endif /* ! __ACE_INLINE_ */
#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)
# if ! defined (__ACE_INLINE__)
#   pragma instantiate UUORB_UtilitiesT<ACE_AbstractEventChannel>
#   pragma instantiate UUORB_UtilitiesT<ACE_AbstractPushSupplier>
# endif /* ! __ACE_INLINE_ */
#endif /* ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
