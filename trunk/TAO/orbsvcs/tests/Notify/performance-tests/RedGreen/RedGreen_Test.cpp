/* -*- C++ -*- $Id$*/

#include "RedGreen_Test.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Synch.h"
#include "ace/OS.h"
#include "orbsvcs/Time_Utilities.h"

ACE_RCSID(Notify, RedGreen_Test, "$Id$")

#define NOTIFY_FACTORY_NAME "NotifyEventChannelFactory"
#define NAMING_SERVICE_NAME "NameService"

#define DOMAIN_GREEN "DOMAIN_GREEN"
#define DOMAIN_RED "DOMAIN_RED"

#define TYPE_GREEN "TYPE_GREEN"
#define TYPE_RED "TYPE_RED"

ACE_Atomic_Op <TAO_SYNCH_MUTEX, int> g_result_count = 0;
ACE_hrtime_t g_throughput_start_;

int
RedGreen_Test::parse_args(int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

    const char *current_arg = 0;
    while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-burst_size")))
        {
          this->burst_size_ = ACE_OS::atoi (current_arg);
          ACE_DEBUG ((LM_DEBUG, "Burst size = %d\n", burst_size_));
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-burst_size [count]\n",
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

RedGreen_Test::RedGreen_Test (void)
  :burst_size_ (10),
   nthreads_ (2)
{
  // No-Op.
  ifgop_ = CosNotifyChannelAdmin::OR_OP;
}

RedGreen_Test::~RedGreen_Test ()
{
  if (!CORBA::is_nil (ec_.in ()))
    this->ec_->destroy ();
}

void
RedGreen_Test::init (int argc, char *argv [], CORBA::Environment &ACE_TRY_ENV)
{
  init_ORB (argc, argv, ACE_TRY_ENV);
  ACE_CHECK;
  resolve_naming_service (ACE_TRY_ENV);
  ACE_CHECK;
  resolve_Notify_factory (ACE_TRY_ENV);
  ACE_CHECK;
  create_EC (ACE_TRY_ENV);
  ACE_CHECK;
  create_supplieradmin (ACE_TRY_ENV);
  ACE_CHECK;
  create_consumeradmin (ACE_TRY_ENV);
  ACE_CHECK;
  create_consumers (ACE_TRY_ENV);
  ACE_CHECK;
  create_suppliers (ACE_TRY_ENV);
  ACE_CHECK;
}

void
RedGreen_Test::run (CORBA::Environment &ACE_TRY_ENV)
{
  this->send_events (ACE_TRY_ENV);
  ACE_CHECK;

  worker_.thr_mgr ()->wait ();
}

void
RedGreen_Test::done (void)
{
  dump_results ();
        worker_.done ();
}

void
RedGreen_Test::init_ORB (int argc,
                      char *argv [],
                      CORBA::Environment &ACE_TRY_ENV)
{
  this->orb_ = CORBA::ORB_init (argc,
                                argv,
                                "",
                                ACE_TRY_ENV);
  ACE_CHECK;

  CORBA::Object_ptr poa_object  =
    this->orb_->resolve_initial_references("RootPOA",
                                           ACE_TRY_ENV);
  ACE_CHECK;

  if (CORBA::is_nil (poa_object))
    {
      ACE_ERROR ((LM_ERROR,
                  " (%P|%t) Unable to initialize the POA.\n"));
      return;
    }
  this->root_poa_ =
    PortableServer::POA::_narrow (poa_object, ACE_TRY_ENV);
  ACE_CHECK;

  PortableServer::POAManager_var poa_manager =
    root_poa_->the_POAManager (ACE_TRY_ENV);
  ACE_CHECK;

  poa_manager->activate (ACE_TRY_ENV);
  ACE_CHECK;

  worker_.orb (this->orb_.in ());

  if (worker_.activate (THR_NEW_LWP | THR_JOINABLE,
                        this->nthreads_) != 0)
    ACE_ERROR ((LM_ERROR,
                "Cannot activate client threads\n"));
}

void
RedGreen_Test::resolve_naming_service (CORBA::Environment &ACE_TRY_ENV)
{
  CORBA::Object_var naming_obj =
    this->orb_->resolve_initial_references (NAMING_SERVICE_NAME,
                                            ACE_TRY_ENV);
  ACE_CHECK;

  // Need to check return value for errors.
  if (CORBA::is_nil (naming_obj.in ()))
    ACE_THROW (CORBA::UNKNOWN ());

  this->naming_context_ =
    CosNaming::NamingContext::_narrow (naming_obj.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
RedGreen_Test::resolve_Notify_factory (CORBA::Environment &ACE_TRY_ENV)
{
  CosNaming::Name name (1);
  name.length (1);
  name[0].id = CORBA::string_dup (NOTIFY_FACTORY_NAME);

  CORBA::Object_var obj =
    this->naming_context_->resolve (name,
                                   ACE_TRY_ENV);
  ACE_CHECK;

  this->notify_factory_ =
    CosNotifyChannelAdmin::EventChannelFactory::_narrow (obj.in (),
                                                         ACE_TRY_ENV);
  ACE_CHECK;
}

void
RedGreen_Test::create_EC (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id,
                                         ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
RedGreen_Test::create_supplieradmin (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));
}

void
RedGreen_Test:: create_consumeradmin (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::AdminID adminid;

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));
}

void
RedGreen_Test::create_consumers (CORBA::Environment &ACE_TRY_ENV)
{
  normal_consumer_ = new RedGreen_Test_StructuredPushConsumer (this);
  normal_consumer_->connect (this->consumer_admin_.in (),
                        ACE_TRY_ENV);
  ACE_CHECK;

  slow_consumer_ = new SlowConsumer (this);
  slow_consumer_->connect (this->consumer_admin_.in (),
                        ACE_TRY_ENV);
  ACE_CHECK;
}

void
RedGreen_Test::create_suppliers (CORBA::Environment &ACE_TRY_ENV)
{
  supplier_ = new RedGreen_Test_StructuredPushSupplier ();
  supplier_->connect (this->supplier_admin_.in (),
                        ACE_TRY_ENV);
  ACE_CHECK;
}

void
RedGreen_Test::send_events (CORBA::Environment &ACE_TRY_ENV)
{
  // Setup the Consumer 1 to receive event_type : "DOMAIN_GREEN", "DOMAIN_GREEN"
  CosNotification::EventTypeSeq added_1(1);
  CosNotification::EventTypeSeq removed_1 (0);

  added_1[0].domain_name =  CORBA::string_dup (DOMAIN_GREEN);
  added_1[0].type_name = CORBA::string_dup (TYPE_GREEN);
  added_1.length (1);
  removed_1.length (0);

  this->normal_consumer_->get_proxy_supplier ()->subscription_change (added_1, removed_1,
                                                                      ACE_TRY_ENV);
  ACE_CHECK;

  // Setup the Consumer 2 to receive event_type : "DOMAIN_RED", "TYPE_RED"
  CosNotification::EventTypeSeq added_2(1);
  CosNotification::EventTypeSeq removed_2 (0);

  added_2[0].domain_name =  CORBA::string_dup (DOMAIN_RED);
  added_2[0].type_name = CORBA::string_dup (TYPE_RED);
  added_2.length (1);
  removed_2.length (0);

  this->slow_consumer_->get_proxy_supplier ()->subscription_change (added_2, removed_2,
                                                           ACE_TRY_ENV);
  ACE_CHECK;

  // Create the events - one of each type

  // Event 2
  CosNotification::StructuredEvent green_event;
  green_event.header.fixed_header.event_type.domain_name =
    CORBA::string_dup(DOMAIN_GREEN);
  green_event.header.fixed_header.event_type.type_name =
    CORBA::string_dup(TYPE_GREEN);
  green_event.header.fixed_header.event_name = CORBA::string_dup("");
  green_event.header.variable_header.length (0); // put nothing here
  green_event.filterable_data.length (0);
  green_event.remainder_of_body <<= (CORBA::Long)10;

  // event 3
  CosNotification::StructuredEvent red_event;
  red_event.header.fixed_header.event_type.domain_name =
    CORBA::string_dup(DOMAIN_RED);
  red_event.header.fixed_header.event_type.type_name =
    CORBA::string_dup(TYPE_RED);
  red_event.header.fixed_header.event_name = CORBA::string_dup("");
  red_event.header.variable_header.length (0); // put nothing here
  red_event.filterable_data.length (0);
  red_event.remainder_of_body <<= (CORBA::Long)10;

  g_throughput_start_ = ACE_OS::gethrtime ();

  // let supplier 1 send all these events
  for (int i = 0; i < this->burst_size_; ++i)
    {
      supplier_->send_event (red_event, ACE_TRY_ENV);
      ACE_CHECK;

      supplier_->send_event (green_event, ACE_TRY_ENV);
      ACE_CHECK;
    }
}


void
RedGreen_Test::dump_results (void)
{
  ACE_Throughput_Stats throughput;
  ACE_UINT32 gsf = ACE_High_Res_Timer::global_scale_factor ();
  char buf[BUFSIZ];

  ACE_OS::sprintf (buf, "Normal Consumer [%02d]", 1);
  normal_consumer_->dump_stats (buf, gsf);
  normal_consumer_->accumulate_into (throughput);

  ACE_OS::sprintf (buf, "Slow Consumer [%02d]", 2);
  slow_consumer_->dump_stats (buf, gsf);
  slow_consumer_->accumulate_into (throughput);

  ACE_DEBUG ((LM_DEBUG, "\n"));

  ACE_Throughput_Stats suppliers;

  ACE_OS::sprintf (buf, "Supplier [%02d]", 1);

  this->supplier_->dump_stats (buf, gsf);
  this->supplier_->accumulate_into (suppliers);

  ACE_DEBUG ((LM_DEBUG, "\nTotals:\n"));
  throughput.dump_results ("Notify_Consumer/totals", gsf);

  ACE_DEBUG ((LM_DEBUG, "\n"));
  suppliers.dump_results ("Notify_Supplier/totals", gsf);
}

/*****************************************************************/
RedGreen_Test_StructuredPushConsumer::RedGreen_Test_StructuredPushConsumer (RedGreen_Test* RedGreen_Test)
  : RedGreen_Test_ (RedGreen_Test),
    push_count_ (0)
{
}

RedGreen_Test_StructuredPushConsumer::~RedGreen_Test_StructuredPushConsumer ()
{
}

void
RedGreen_Test_StructuredPushConsumer::accumulate_into (ACE_Throughput_Stats &throughput) const
{
  throughput.accumulate (this->throughput_);
}

void
RedGreen_Test_StructuredPushConsumer::dump_stats (const char* msg, ACE_UINT32 gsf)
{
  this->throughput_.dump_results (msg, gsf);
}

void
RedGreen_Test_StructuredPushConsumer::connect (CosNotifyChannelAdmin::ConsumerAdmin_ptr consumer_admin, CORBA::Environment &ACE_TRY_ENV)
{
  // Activate the consumer with the default_POA_
  CosNotifyComm::StructuredPushConsumer_var objref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxySupplier_var proxysupplier =
    consumer_admin->obtain_notification_push_supplier (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_supplier_id_, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxysupplier.in ()));

  // narrow
  this->proxy_supplier_ =
    CosNotifyChannelAdmin::StructuredProxyPushSupplier::
    _narrow (proxysupplier.in (), ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_supplier_.in ()));

  proxy_supplier_->connect_structured_push_consumer (objref.in (),
                                                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
RedGreen_Test_StructuredPushConsumer::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  this->proxy_supplier_->
    disconnect_structured_push_supplier(ACE_TRY_ENV);
  ACE_CHECK;
}

void
RedGreen_Test_StructuredPushConsumer::offer_change (const CosNotification::EventTypeSeq & /*added*/, const CosNotification::EventTypeSeq & /*removed*/, CORBA::Environment & //ACE_TRY_ENV
)
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosNotifyComm::InvalidEventType
      ))
{
  // No-Op.
}

void
RedGreen_Test_StructuredPushConsumer::push_structured_event (const CosNotification::StructuredEvent & notification, CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  ACE_GUARD (TAO_SYNCH_MUTEX, ace_mon, this->lock_);
  const char* domain_name =
    notification.header.fixed_header.event_type.domain_name;

  const char* type_name =
    notification.header.fixed_header.event_type.type_name;

  ACE_DEBUG ((LM_DEBUG, "Consumer %d event, domain = %s, type =  %s\n",
              this->proxy_supplier_id_, domain_name, type_name));

  TimeBase::TimeT latency_base_recorded;
  ACE_hrtime_t latency_base;

  notification.filterable_data[0].value >>= latency_base_recorded;

  ORBSVCS_Time::TimeT_to_hrtime (latency_base,
                                 latency_base_recorded);

  ++this->push_count_;

  // Grab timestamp again.
  ACE_hrtime_t now = ACE_OS::gethrtime ();

  // Record statistics.
  this->throughput_.sample (now - g_throughput_start_,
                            now - latency_base);


  if (++g_result_count == 2*RedGreen_Test_->burst_size_)
    RedGreen_Test_->done ();

}

void
RedGreen_Test_StructuredPushConsumer::disconnect_structured_push_consumer (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // No-Op.
}

CosNotifyChannelAdmin::StructuredProxyPushSupplier_ptr
RedGreen_Test_StructuredPushConsumer::get_proxy_supplier (void)
{
  return proxy_supplier_.in ();
}

/*****************************************************************/

SlowConsumer::SlowConsumer (RedGreen_Test* RedGreen_Test)
  :RedGreen_Test_StructuredPushConsumer (RedGreen_Test)
{


}

void
SlowConsumer::push_structured_event (
        const CosNotification::StructuredEvent & notification,
        CORBA::Environment &ACE_TRY_ENV
      )
      ACE_THROW_SPEC ((
        CORBA::SystemException,
        CosEventComm::Disconnected
       ))
{
  // Slow it down ...
  ACE_OS::sleep (1);

  RedGreen_Test_StructuredPushConsumer::push_structured_event (notification, ACE_TRY_ENV);
}

/*****************************************************************/

RedGreen_Test_StructuredPushSupplier::RedGreen_Test_StructuredPushSupplier (void)
{
}

RedGreen_Test_StructuredPushSupplier::~RedGreen_Test_StructuredPushSupplier ()
{
}

void
RedGreen_Test_StructuredPushSupplier::accumulate_into (ACE_Throughput_Stats &throughput) const
{
  throughput.accumulate (this->throughput_);
}

void
RedGreen_Test_StructuredPushSupplier::dump_stats (const char* msg, ACE_UINT32 gsf)
{
  this->throughput_.dump_results (msg, gsf);
}

void
RedGreen_Test_StructuredPushSupplier::connect (CosNotifyChannelAdmin::SupplierAdmin_ptr supplier_admin, CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyComm::StructuredPushSupplier_var objref =
    this->_this (ACE_TRY_ENV);
  ACE_CHECK;

  CosNotifyChannelAdmin::ProxyConsumer_var proxyconsumer =
    supplier_admin->obtain_notification_push_consumer (CosNotifyChannelAdmin::STRUCTURED_EVENT, proxy_consumer_id_, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxyconsumer.in ()));

  // narrow
  this->proxy_consumer_ =
    CosNotifyChannelAdmin::StructuredProxyPushConsumer::_narrow (proxyconsumer.in (), ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (proxy_consumer_.in ()));

  proxy_consumer_->connect_structured_push_supplier (objref.in (),
                                                     ACE_TRY_ENV);
  ACE_CHECK;
}

void
RedGreen_Test_StructuredPushSupplier::disconnect (CORBA::Environment &ACE_TRY_ENV)
{
  ACE_ASSERT (!CORBA::is_nil (this->proxy_consumer_.in ()));

  this->proxy_consumer_->disconnect_structured_push_consumer(ACE_TRY_ENV);
}

void
RedGreen_Test_StructuredPushSupplier::subscription_change (const CosNotification::EventTypeSeq & /*added*/, const CosNotification::EventTypeSeq & /*removed */, CORBA::Environment & /*ACE_TRY_ENV*/
      )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  //No-Op.
}

void
RedGreen_Test_StructuredPushSupplier::send_event (CosNotification::StructuredEvent& event, CORBA::Environment &ACE_TRY_ENV)
{
  event.filterable_data.length (1);
  event.filterable_data[0].name = CORBA::string_dup("latency_base");

  // Record current time.
  ACE_hrtime_t start = ACE_OS::gethrtime ();

  TimeBase::TimeT latency_base;
  ORBSVCS_Time::hrtime_to_TimeT (latency_base,
                                 start);
  // any
  event.filterable_data[0].value <<= latency_base;
  proxy_consumer_->push_structured_event (event, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_hrtime_t end = ACE_OS::gethrtime ();

  this->throughput_.sample (end - g_throughput_start_,
                            end - start);
  ACE_CHECK;
}

void
RedGreen_Test_StructuredPushSupplier::disconnect_structured_push_supplier (CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException
                   ))
{
  // No-Op.
}

/*****************************************************************/

Worker::Worker (void)
:done_(0)
{
}

void
Worker::orb (CORBA::ORB_ptr orb)
{
  orb_ = CORBA::ORB::_duplicate (orb);
}

void
Worker::done (void)
{
        done_ = 1;
}

int
Worker::svc (void)
{
  while (!this->done_)
                if (this->orb_->work_pending ())
                        this->orb_->perform_work ();

        return 0;
}

// ****************************************************************

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
