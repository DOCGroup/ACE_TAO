// $Id$

#include "Throughput.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "ace/OS.h"
#include "orbsvcs/Time_Utilities.h"
#include "orbsvcs/Notify/Notify_EventChannelFactory_i.h"
#include "orbsvcs/Notify/Notify_Default_CO_Factory.h"
#include "orbsvcs/Notify/Notify_Default_POA_Factory.h"
#include "orbsvcs/Notify/Notify_Default_Collection_Factory.h"
#include "orbsvcs/Notify/Notify_Default_EMO_Factory.h"

ACE_RCSID (Notify_Tests, Throughput, "$Id$")

/***************************************************************************/

Throughput_StructuredPushConsumer::Throughput_StructuredPushConsumer (Notify_Throughput *test_client)
  : test_client_ (test_client),
    push_count_ (0)
{
}


void
Throughput_StructuredPushConsumer::accumulate_into (ACE_Throughput_Stats &throughput) const
{
  throughput.accumulate (this->throughput_);
}

void
Throughput_StructuredPushConsumer::dump_stats (const char* msg, ACE_UINT32 gsf)
{
  this->throughput_.dump_results (msg, gsf);
}

void
Throughput_StructuredPushConsumer::push_structured_event (const CosNotification::StructuredEvent & notification, CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  TimeBase::TimeT Throughput_base_recorded;
  ACE_hrtime_t Throughput_base;

  notification.filterable_data[0].value >>= Throughput_base_recorded;

  ORBSVCS_Time::TimeT_to_hrtime (Throughput_base,
                                 Throughput_base_recorded);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->push_count_ == 0)
    this->throughput_start_ = ACE_OS::gethrtime ();

  ++this->push_count_;

  // Grab timestamp again.
  ACE_hrtime_t now = ACE_OS::gethrtime ();

  // Record statistics.
  this->throughput_.sample (now - this->throughput_start_,
                            now - Throughput_base);

  CORBA::Long done;
  notification.remainder_of_body >>= done;

  if (this->push_count_%1000 == 0)
    ACE_DEBUG ((LM_DEBUG,"(%P)(%t) event count = %d\n", this->push_count_));


  ACE_DECLARE_NEW_CORBA_ENV;
  test_client_->g_consumer_done_count++;
  if (test_client_->g_consumer_done_count.value () >= test_client_->expected_count_)
    test_client_->end_test (ACE_TRY_ENV);

}

/***************************************************************************/

Throughput_StructuredPushSupplier::Throughput_StructuredPushSupplier (Notify_Throughput* test_client)
  :test_client_ (test_client),
   push_count_ (0)
{
}

Throughput_StructuredPushSupplier::~Throughput_StructuredPushSupplier ()
{
}


void
Throughput_StructuredPushSupplier::accumulate_into (ACE_Throughput_Stats &throughput) const
{
  throughput.accumulate (this->throughput_);
}

void
Throughput_StructuredPushSupplier::dump_stats (const char* msg, ACE_UINT32 gsf)
{
  this->throughput_.dump_results (msg, gsf);
}

int
Throughput_StructuredPushSupplier::svc (void)
{
  // Initialize a time value to pace the test
  ACE_Time_Value tv (0, test_client_->burst_pause_);

  // operations:
  CosNotification::StructuredEvent event;

  // EventHeader

  // FixedEventHeader
  // EventType
  // string
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_type.type_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_name = CORBA::string_dup("myevent");

  // OptionalHeaderFields
  // PropertySeq
  // sequence<Property>: string name, any value
  CosNotification::PropertySeq& qos =  event.header.variable_header;
  qos.length (0); // put nothing here

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup("Throughput_base");

  event.remainder_of_body <<= (CORBA::Long)0;
  // The "remainder_of_body" carries a flag that indicates shutdown.
  // 0 - no, 1 = yes for shutdown.

  ACE_DECLARE_NEW_CORBA_ENV;

  this->throughput_start_ = ACE_OS::gethrtime ();

  for (int i = 0; i < test_client_->burst_count_; ++i)
    {
      for (int j = 0; j < test_client_->burst_size_; ++j)
        {
          // Record current time.
          ACE_hrtime_t start = ACE_OS::gethrtime ();
          TimeBase::TimeT Throughput_base;
          ORBSVCS_Time::hrtime_to_TimeT (Throughput_base,
                                         start);
          // any
          event.filterable_data[0].value <<= Throughput_base;

          this->proxy_consumer_->push_structured_event (event, ACE_TRY_ENV);
          ACE_CHECK_RETURN (-1);

          ACE_hrtime_t end = ACE_OS::gethrtime ();
          this->throughput_.sample (end - this->throughput_start_,
                                    end - start);
        }

      ACE_OS::sleep (tv);
    }

  // send the shutdown message.
  event.remainder_of_body <<= (CORBA::Long)1;

  ACE_hrtime_t start = ACE_OS::gethrtime ();
  TimeBase::TimeT Throughput_base;
  ORBSVCS_Time::hrtime_to_TimeT (Throughput_base,
                                 start);
  event.filterable_data[0].value <<= Throughput_base;

  this->proxy_consumer_->push_structured_event (event, ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  test_client_->g_consumer_done_count++;
  return 0;
}

/***************************************************************************/
Notify_Throughput::Notify_Throughput (void)
  : colocated_ec_ (0),
    burst_count_ (100),
    burst_pause_ (10000),
    burst_size_ (100),
    consumer_count_ (5),
    supplier_count_ (5),
    run_timeout_ (10000),
    g_consumer_done_count (0),
    nthreads_ (10),
    expected_count_ (0)
{
}

Notify_Throughput::~Notify_Throughput ()
{
}

void
Notify_Throughput::init (int argc, char* argv [], CORBA::Environment &ACE_TRY_ENV)
{

  expected_count_ = burst_count_*burst_size_*supplier_count_*consumer_count_;

  ACE_DEBUG ((LM_DEBUG, "expected count = %d\n", expected_count_));

  // init base class
  Notify_Test_Client::init_ORB (argc, argv, ACE_TRY_ENV);
  ACE_CHECK;

  worker_.orb (this->orb_.in ());

  if (worker_.activate (THR_NEW_LWP | THR_JOINABLE,
                        this->nthreads_) != 0)
    ACE_ERROR ((LM_ERROR,
                "Cannot activate client threads\n"));
  // Create all participents ...
  this->create_EC (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  consumers_ = new Throughput_StructuredPushConsumer*[this->consumer_count_];
  suppliers_ = new Throughput_StructuredPushSupplier*[this->supplier_count_];

  // ----

  int i = 0;

  for (i = 0; i < this->consumer_count_; ++i)
    {
      consumers_[i] = new Throughput_StructuredPushConsumer (this);
      consumers_[i]->init (root_poa_.in (), ACE_TRY_ENV);
      ACE_CHECK;
      consumers_[i]->connect (this->consumer_admin_.in (), ACE_TRY_ENV);
      ACE_CHECK;
    }

  for (i = 0; i < this->supplier_count_; ++i)
    {
      suppliers_[i] = new Throughput_StructuredPushSupplier (this);
      suppliers_[i]->
        TAO_Notify_StructuredPushSupplier::init (root_poa_.in (), ACE_TRY_ENV);
      ACE_CHECK;
      suppliers_[i]->connect (this->supplier_admin_.in (), ACE_TRY_ENV);
      ACE_CHECK;
    }

  // ----

  // Setup the CA to receive all type of events
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);
  removed.length (0);

  added[0].domain_name =  CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added, removed, ACE_TRY_ENV);
  ACE_CHECK;
}

int
Notify_Throughput::parse_args(int argc, char *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc, argv);

    const char *current_arg = 0;
    while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp ("-colocated_ec") == 0)
        {
          this->colocated_ec_ = 1;
          arg_shifter.consume_arg ();

          // Init Factories
          TAO_Notify_Default_CO_Factory::init_svc ();
          TAO_Notify_Default_POA_Factory::init_svc ();
          TAO_Notify_Default_Collection_Factory::init_svc ();
          TAO_Notify_Default_EMO_Factory::init_svc ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-consumers")))
        {
          this->consumer_count_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-suppliers")))
        {
          this->supplier_count_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-burst_size")))
        {
          this->burst_size_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-burst_count")))
        {
          this->burst_count_ = ACE_OS::atoi (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-burst_pause")))
        {
          this->burst_pause_ = ACE_OS::atoi (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-events event_count "
                     "-colocated_ec "
                     "consumers [count]"
                     "suppliers [count]\n",
                     argv[0], argv[0]));

          arg_shifter.consume_arg ();

          return -1;
        }
      else
        {
            arg_shifter.ignore_arg ();
        }
    }
    return 0;
}

void
Notify_Throughput::create_EC (CORBA::Environment &ACE_TRY_ENV)
{
  if (this->colocated_ec_ == 1)
    {
      this->notify_factory_ =
        TAO_Notify_EventChannelFactory_i::create (this->root_poa_.in (),
                                                  ACE_TRY_ENV);
      ACE_CHECK;

      ACE_ASSERT (!CORBA::is_nil (this->notify_factory_.in ()));
    }
  else
    {
      this->resolve_naming_service (ACE_TRY_ENV);
      ACE_CHECK;
      this->resolve_Notify_factory (ACE_TRY_ENV);
      ACE_CHECK;
    }

  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id,
                                         ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Notify_Throughput::run_test (CORBA::Environment &ACE_TRY_ENV)
{

  ACE_DEBUG ((LM_DEBUG, "colocated_ec_ %d ,"
              "burst_count_ %d, "
              "burst_pause_ %d, "
              "burst_size_  %d, "
              "consumer_count_ %d "
              "supplier_count_ %d "
              "run_timeout_  %d \n",
              colocated_ec_,
              burst_count_ ,
              burst_pause_ ,
              burst_size_ ,
              consumer_count_ ,
              supplier_count_ ,
              run_timeout_));

  for (int i = 0; i < this->supplier_count_; ++i)
    {
      suppliers_[i]->
        TAO_Notify_StructuredPushSupplier::init (root_poa_.in (), ACE_TRY_ENV);
      ACE_CHECK;

      if (suppliers_[i]->activate (THR_NEW_LWP | THR_JOINABLE) != 0)
        ACE_ERROR ((LM_ERROR,
                    "Cannot activate client threads\n"));
    }


  worker_.thr_mgr ()->wait ();

}

void
Notify_Throughput::end_test (CORBA::Environment &ACE_TRY_ENV)
{
  dump_results ();

  ACE_DEBUG ((LM_DEBUG, "calling shutdown\n"));
  this->orb_->shutdown (0, ACE_TRY_ENV);
}


void
Notify_Throughput::dump_results (void)
{
  ACE_Throughput_Stats throughput;
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  char buf[BUFSIZ];
  for (int j = 0; j < this->consumer_count_; ++j)
    {
      ACE_OS::sprintf (buf, "Consumer [%02d]", j);

      this->consumers_[j]->dump_stats (buf, gsf);
      this->consumers_[j]->accumulate_into (throughput);
    }
  ACE_DEBUG ((LM_DEBUG, "\n"));

  ACE_Throughput_Stats suppliers;
  for (int i = 0; i < this->supplier_count_; ++i)
    {
      ACE_OS::sprintf (buf, "Supplier [%02d]", i);

      this->suppliers_[i]->dump_stats (buf, gsf);
      this->suppliers_[i]->accumulate_into (suppliers);
    }

  ACE_DEBUG ((LM_DEBUG, "\nTotals:\n"));
  throughput.dump_results ("Notify_Consumer/totals", gsf);

  ACE_DEBUG ((LM_DEBUG, "\n"));
  suppliers.dump_results ("Notify_Supplier/totals", gsf);
}

/***************************************************************************/

int
main (int argc, char* argv[])
{
  ACE_High_Res_Timer::calibrate ();

  Notify_Throughput events;

  if (events.parse_args (argc, argv) == -1)
    return 1;

  ACE_TRY_NEW_ENV
    {
      events.init (argc, argv,
                      ACE_TRY_ENV); //Init the Client
      ACE_TRY_CHECK;

      events.run_test (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      events.end_test (ACE_TRY_ENV);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "Events user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "Events system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}


// ****************************************************************

Worker::Worker (void)
{
}

void
Worker::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

int
Worker::svc (void)
{
  this->orb_->run ();
  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
