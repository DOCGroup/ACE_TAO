#include "Throughput.h"

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "ace/Dynamic_Service.h"
#include "tao/Strategies/advanced_resource.h"
#include "tao/Messaging/Messaging.h"
#include "orbsvcs/Notify/Service.h"
#include "orbsvcs/Time_Utilities.h"
#include <cstddef>
#include <new>

/***************************************************************************/

Throughput_StructuredPushConsumer::Throughput_StructuredPushConsumer (
    Notify_Throughput& test_client
  )
  : test_client_ (test_client)
{
}

void
Throughput_StructuredPushConsumer::accumulate_into (
    ACE_Throughput_Stats& throughput) const
{
  throughput.accumulate (this->throughput_);
}

void
Throughput_StructuredPushConsumer::dump_stats (
  ACE_TCHAR const* msg,
  ACE_High_Res_Timer::global_scale_factor_type gsf)
{
  this->throughput_.dump_results (msg, gsf);
}

void
Throughput_StructuredPushConsumer::push_structured_event (
    CosNotification::StructuredEvent const& notification
  )
{
  // Extract payload.
  char const* msg {};
  CORBA::Boolean const ok = (notification.remainder_of_body >>= msg);

  if (!ok)
    ACE_DEBUG ((LM_DEBUG, "(%t) Error extracting message body\n"));

  TimeBase::TimeT Throughput_base_recorded {};
  ACE_hrtime_t Throughput_base {};

  notification.filterable_data[0].value >>= Throughput_base_recorded;

  ORBSVCS_Time::TimeT_to_hrtime (Throughput_base,
                                 Throughput_base_recorded);

  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);

  if (this->push_count_ == 0)
    {
    this->throughput_start_ = ACE_OS::gethrtime ();
    }

  ++this->push_count_;

  // Grab timestamp again.
  ACE_hrtime_t const now = ACE_OS::gethrtime ();

  // Record statistics.
  this->throughput_.sample (now - this->throughput_start_,
                            now - Throughput_base);

  if (this->push_count_ % 1000 == 0)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%P)(%t) event count = %d\n",
                  this->push_count_));
    }

  if (this->push_count_ == test_client_.perconsumer_count_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "(%t)expected count reached\n"));
      test_client_.peer_done ();
    }
}

/***************************************************************************/

Throughput_StructuredPushSupplier::Throughput_StructuredPushSupplier (
    Notify_Throughput& test_client
  )
  : test_client_ (test_client)
{
}

void
Throughput_StructuredPushSupplier::accumulate_into (
    ACE_Throughput_Stats& throughput) const
{
  throughput.accumulate (this->throughput_);
}

void
Throughput_StructuredPushSupplier::dump_stats (
  ACE_TCHAR const* msg,
  ACE_High_Res_Timer::global_scale_factor_type gsf)
{
  this->throughput_.dump_results (msg, gsf);
}

int
Throughput_StructuredPushSupplier::svc ()
{
  // Initialize a time value to pace the test.
  ACE_Time_Value const tv (0, test_client_.options_.burst_pause);

  // Operations.
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
  CosNotification::PropertySeq& qos = event.header.variable_header;
  qos.length (0); // put nothing here

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup("Throughput_base");

  event.remainder_of_body <<= test_client_.payload_.c_str ();

  this->throughput_start_ = ACE_OS::gethrtime ();

  for (int i {}; i < test_client_.options_.burst_count; ++i)
    {
      for (int j {}; j < test_client_.options_.burst_size; ++j)
        {
          // Record current time.
          ACE_hrtime_t const start = ACE_OS::gethrtime ();
          TimeBase::TimeT Throughput_base {};
          ORBSVCS_Time::hrtime_to_TimeT (Throughput_base,
                                         start);
          // Any.
          event.filterable_data[0].value <<= Throughput_base;

          this->proxy_->push_structured_event (event);

          ACE_hrtime_t const end = ACE_OS::gethrtime ();
          this->throughput_.sample (end - this->throughput_start_,
                                    end - start);
        }

      ACE_OS::sleep (tv);
    }

  ACE_DEBUG ((LM_DEBUG, "(%P) (%t) Supplier done\n"));
  test_client_.peer_done ();
  return 0;
}

/***************************************************************************/
Notify_Throughput::Notify_Throughput ()
  : condition_ (lock_)
{
  this->perconsumer_count_ =
    this->options_.burst_size * this->options_.burst_count * this->options_.supplier_count;
  this->peer_done_count_ = this->options_.consumer_count + this->options_.supplier_count;
}

Notify_Throughput::~Notify_Throughput ()
{
  this->orb_->shutdown (false);
}

int
Notify_Throughput::init (int argc, ACE_TCHAR* argv [])
{
  // Initialize base class.
  Notify_Test_Client::init_ORB (argc,
                                argv);

#if (TAO_HAS_CORBA_MESSAGING == 1)
  CORBA::Object_var manager_object =
    orb_->resolve_initial_references ("ORBPolicyManager");

  CORBA::PolicyManager_var policy_manager =
    CORBA::PolicyManager::_narrow (manager_object.in ());

  CORBA::Any sync_scope;
  sync_scope <<= Messaging::SYNC_WITH_TARGET;

  CORBA::PolicyList policy_list (1);
  policy_list.length (1);
  policy_list[0] =
    orb_->create_policy (Messaging::SYNC_SCOPE_POLICY_TYPE,
                        sync_scope);
  policy_manager->set_policy_overrides (policy_list,
                                        CORBA::SET_OVERRIDE);
#else
  ACE_DEBUG ((LM_DEBUG,
              "CORBA Messaging disabled in this configuration,"
              " test may not be optimally configured\n"));
#endif /* TAO_HAS_MESSAGING */

  worker_.orb (this->orb_.in ());

  if (worker_.activate (THR_NEW_LWP | THR_JOINABLE,
                        1) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot activate client threads\n"));
    }

  // Create all participents ...
  this->create_EC ();

  CosNotifyChannelAdmin::AdminID adminid {};

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  try
    {
      this->consumers_.reserve (this->options_.consumer_count > 0
                                  ? static_cast<std::size_t> (this->options_.consumer_count)
                                  : 0);
      this->suppliers_.reserve (this->options_.supplier_count > 0
                                  ? static_cast<std::size_t> (this->options_.supplier_count)
                                  : 0);
    }
  catch (std::bad_alloc const&)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to allocate peer lists\n"), -1);
    }

  for (int i {}; i < this->options_.consumer_count; ++i)
    {
      Throughput_StructuredPushConsumer* consumer {};
      ACE_NEW_RETURN (consumer,
                      Throughput_StructuredPushConsumer (*this),
                      -1);
      consumer->init (root_poa_.in ());

      consumer->connect (this->consumer_admin_.in ());
      this->consumers_.push_back (consumer);
    }

  for (int i {}; i < this->options_.supplier_count; ++i)
    {
      Throughput_StructuredPushSupplier* supplier {};
      ACE_NEW_RETURN (supplier,
                      Throughput_StructuredPushSupplier (*this),
                      -1);
      supplier->TAO_Notify_Tests_StructuredPushSupplier::init (
                         root_poa_.in ());
      supplier->connect (this->supplier_admin_.in ());
      this->suppliers_.push_back (supplier);
    }

  return 0;
}

int
Notify_Throughput::parse_args (int argc, ACE_TCHAR *argv [])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  ACE_TCHAR const* current_arg {};
  while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-collocated_ec")) == 0)
        {
          this->options_.collocated_ec = true;
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-consumers"))) != nullptr)
        {
          this->options_.consumer_count = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-suppliers"))) != nullptr)
        {
          this->options_.supplier_count = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-burst_size"))) != nullptr)
        {
          this->options_.burst_size = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-burst_count"))) != nullptr)
        {
          this->options_.burst_count = ACE_OS::atoi (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-burst_pause"))) != nullptr)
        {
          this->options_.burst_pause = ACE_OS::atoi (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-payload"))) != nullptr)
        {
          this->options_.payload_size = ACE_OS::atoi (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-EC"))) != nullptr)
        {
          this->options_.ec_name = ACE_TEXT_ALWAYS_CHAR (current_arg);
          //
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ExpectedCount"))) != nullptr)
        {
          this->perconsumer_count_ = ACE_OS::atoi (current_arg);
          this->options_.expected_count_set = true;
          //
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-?")) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                     "usage: %s "
                     "-collocated_ec, "
                     "-consumers [count], "
                     "-suppliers [count], "
                     "-burst_size [size], "
                     "-burst_count [count], "
                     "-burst_pause [time(uS)], "
                     "-payload [size]"
                     "-EC [Channel Name]"
                     "-ExpectedCount [count]\n",
                     argv[0], argv[0]));

          arg_shifter.consume_arg ();

          return -1;
        }
      else
        {
          arg_shifter.ignore_arg ();
        }
    }
  // Recalculate.
  if (!this->options_.expected_count_set)
    this->perconsumer_count_ = this->options_.burst_size * this->options_.burst_count *
      this->options_.supplier_count;
  this->peer_done_count_ = this->options_.consumer_count + this->options_.supplier_count;
  if (this->options_.payload_size < 0)
    ACE_ERROR_RETURN ((LM_ERROR, "Payload size must be non-negative\n"), -1);
  try
    {
      this->payload_.assign (static_cast<std::size_t> (this->options_.payload_size), 'x');
    }
  catch (std::bad_alloc const&)
    {
      ACE_ERROR_RETURN ((LM_ERROR, "Unable to allocate payload\n"), -1);
    }
  return 0;
}

void
Notify_Throughput::create_EC ()
{
  if (this->options_.collocated_ec)
    {
      TAO_Notify_Service* const notify_service =
        ACE_Dynamic_Service<TAO_Notify_Service>::instance (TAO_NOTIFICATION_SERVICE_NAME);

      if (notify_service == nullptr)
        {
          ACE_DEBUG ((LM_DEBUG, "Service not found! check conf. file\n"));
          return;
        }

      // Activate the factory
      this->notify_factory_ =
        notify_service->create (this->root_poa_.in ());

      ACE_ASSERT (!CORBA::is_nil (this->notify_factory_.in ()));
    }
  else
    {
      this->resolve_naming_service ();
      this->resolve_Notify_factory ();
    }

  // A channel name was specified, use that to resolve the service.
  if (!this->options_.ec_name.empty ())
    {
      CosNaming::Name name (1);
      name.length (1);
      name[0].id = CORBA::string_dup (this->options_.ec_name.c_str ());

      CORBA::Object_var obj =
        this->naming_context_->resolve (name);

      this->ec_ =
        CosNotifyChannelAdmin::EventChannel::_narrow (obj.in ());
    }
  else
    {
      CosNotifyChannelAdmin::ChannelID id {};

      ec_ = notify_factory_->create_channel (initial_qos_,
                                             initial_admin_,
                                             id);
    }

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Notify_Throughput::run_test ()
{
  ACE_DEBUG ((LM_DEBUG, "collocated_ec_ %d ,"
              "burst_count_ %d, "
              "burst_pause_ %d, "
              "burst_size_  %d, "
              "payload_size_ %d, "
              "consumer_count_ %d,  "
              "supplier_count_ %d "
              "expected count %d\n",
              this->options_.collocated_ec,
              this->options_.burst_count,
              this->options_.burst_pause,
              this->options_.burst_size,
              this->options_.payload_size,
              this->options_.consumer_count,
              this->options_.supplier_count,
              perconsumer_count_));

  for (int i {}; i < this->options_.supplier_count; ++i)
    {
      this->suppliers_[static_cast<std::size_t> (i)]->
        TAO_Notify_Tests_StructuredPushSupplier::init (root_poa_.in ());

      if (this->suppliers_[static_cast<std::size_t> (i)]->ACE_Task_Base::activate (
            THR_NEW_LWP | THR_JOINABLE) != 0)
        {
          ACE_ERROR ((LM_ERROR,
                      "Cannot activate client threads\n"));
        }
    }

  // Wait till we're signalled done.
  {
    ACE_DEBUG ((LM_DEBUG, "(%t)Waiting for shutdown signal in main..\n"));
    ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, lock_);

    while (this->peer_done_count_ != 0)
      {
        condition_.wait ();
      }
  }

  if (this->options_.ec_name.empty ()) // we are not using a global EC
    {
      // Destroy the ec.
      this->ec_->destroy ();
    }

  // Signal the workers.
  this->worker_.done_.store (true, std::memory_order_relaxed);
}

void
Notify_Throughput::peer_done ()
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, lock_);

  if (--this->peer_done_count_ == 0)
    {
      ACE_DEBUG ((LM_DEBUG, "calling shutdown\n"));
      condition_.broadcast ();
    }
}

void
Notify_Throughput::dump_results ()
{
  ACE_Throughput_Stats throughput;
  ACE_High_Res_Timer::global_scale_factor_type gsf =
    ACE_High_Res_Timer::global_scale_factor ();
  ACE_TCHAR buf[BUFSIZ];

  for (int j {}; j < this->options_.consumer_count; ++j)
    {
      ACE_OS::sprintf (buf, ACE_TEXT ("Consumer [%02d]"), j);

      this->consumers_[static_cast<std::size_t> (j)]->dump_stats (buf, gsf);
      this->consumers_[static_cast<std::size_t> (j)]->accumulate_into (throughput);
    }

  ACE_DEBUG ((LM_DEBUG, "\n"));

  ACE_Throughput_Stats suppliers;

  for (int i {}; i < this->options_.supplier_count; ++i)
    {
      ACE_OS::sprintf (buf, ACE_TEXT ("Supplier [%02d]"), i);

      this->suppliers_[static_cast<std::size_t> (i)]->dump_stats (buf, gsf);
      this->suppliers_[static_cast<std::size_t> (i)]->accumulate_into (suppliers);
    }

  ACE_DEBUG ((LM_DEBUG, "\nTotals:\n"));
  throughput.dump_results (ACE_TEXT ("Notify_Consumer/totals"), gsf);

  ACE_DEBUG ((LM_DEBUG, "\n"));
  suppliers.dump_results (ACE_TEXT ("Notify_Supplier/totals"), gsf);
}

/***************************************************************************/

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ACE_High_Res_Timer::calibrate ();

  Notify_Throughput events;

  if (events.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  try
    {
      events.init (argc, argv); // Init the Client

      events.run_test ();

      ACE_DEBUG ((LM_DEBUG, "Waiting for threads to exit...\n"));
      ACE_Thread_Manager::instance ()->wait ();
      events.dump_results ();

      ACE_DEBUG ((LM_DEBUG, "ending main...\n"));
    }
  catch (CORBA::Exception const& se)
    {
      se._tao_print_exception ("Error: ");
      return 1;
    }

  return 0;
}

// ****************************************************************

void
Worker::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

int
Worker::svc ()
{
  ACE_Time_Value tv (5);

  do
    {
      this->orb_->run (tv);
    }
  while (!this->done_.load (std::memory_order_relaxed));

  ACE_DEBUG ((LM_DEBUG, "(%P) (%t) done\n"));

  return 0;
}
