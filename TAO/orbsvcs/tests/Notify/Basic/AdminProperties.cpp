// $Id$

#include "AdminProperties.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "tao/debug.h"

/***************************************************************************/

AdminProperties_Task::AdminProperties_Task (void)
  : supplier_ (0)
  , client_ (0)
{
}

void
AdminProperties_Task::init (TAO_Notify_Tests_StructuredPushSupplier *supplier, AdminProperties* client)
{
  supplier_  = supplier;
  client_ = client;
}

int
AdminProperties_Task::init (int argc, ACE_TCHAR *argv[])
{
  return ACE_Task_Base::init (argc, argv);
}

int
AdminProperties_Task::svc (void)
{
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
  event.header.variable_header.length (0); // put nothing here

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup("threshold");

  event.filterable_data[1].name = CORBA::string_dup("temperature");
  event.filterable_data[1].value <<= (CORBA::Long)70;

  event.filterable_data[2].name = CORBA::string_dup("pressure");
  event.filterable_data[2].value <<= (CORBA::Long)80;

  // @@ CORBA::Short prio = CosNotification::LowestPriority;

 int event_count = this->client_->event_count_;

 ACE_DEBUG ((LM_DEBUG, "\n1 supplier sending %d events...\n", event_count));


 for (int i = 0 ; i < event_count; ++i)
   {
     event.filterable_data[0].value <<= (CORBA::Long)i;

     event.remainder_of_body <<= (CORBA::Long)i;

     try
        {
          ACE_DEBUG((LM_DEBUG, "+"));
          this->supplier_->send_event (event);
        }
      catch (const CORBA::IMP_LIMIT&)
        {
          if (TAO_debug_level > 0)
            ACE_DEBUG ((LM_DEBUG, "\nEvent %d was not send due to Impl Limit reached\n", i));

          ++ this->client_->rejections_;
        }
      catch (const CORBA::Exception& ex)
        {
          ex._tao_print_exception (
            "Error: Exception sending event\n");
          return 1;
        }
   }

return 0;
}
/***************************************************************************/

AdminProperties_StructuredPushConsumer::AdminProperties_StructuredPushConsumer (AdminProperties* client)
  : client_ (client)
  , events_received_ (0)
{
  client_->consumer_start(this);
}

void
AdminProperties_StructuredPushConsumer::push_structured_event (const CosNotification::StructuredEvent & /*notification*/
                                                               )
{
  ++events_received_;

  if (events_received_ >= client_->max_queue_length_)
    client_->consumer_done(this);

  ACE_DEBUG((LM_DEBUG, "-"));

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "\nConsumer %x received event %d\n", this, events_received_.value ()));
}

/***************************************************************************/

AdminProperties::AdminProperties (void)
  : max_queue_length_ (10),
    max_consumers_ (3),
    max_suppliers_ (3),
    reject_new_events_ (0),
    consumers_ (4),
    suppliers_ (4),
    event_count_ (30),
    suppliers_connected_count_ (0),
    consumers_connected_count_ (0),
    rejections_ (0)
{
}

AdminProperties::~AdminProperties (void)
{
}

int
AdminProperties::parse_args(int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc, argv);

  const char *current_arg = 0;

  while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter ("-max_queue_length")))
        {
          this->max_queue_length_ = ACE_OS::atoi (current_arg);
          // Max. queue length.

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter ("-max_consumers")))
        {
          this->max_consumers_ = ACE_OS::atoi (current_arg);
          // Max consumers allowed to connect.
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter ("-max_suppliers")))
        {
          this->max_suppliers_ = ACE_OS::atoi (current_arg);
          // Max. number of suppliers allowed to connect.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-reject_new_events") == 0)
        {
          this->reject_new_events_ = 1;
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter ("-consumers")))
        {
          this->consumers_ = ACE_OS::atoi (current_arg);
          // Number of consumers to create.
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter ("-suppliers")))
        {
          this->suppliers_ = ACE_OS::atoi (current_arg);
          // Number of suppliers to create.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-max_queue_length [max_queue_length] "
                     "-max_consumers [max_consumers] "
                     "-max_suppliers [max_suppliers] "
                     "-reject_new_events [reject_new_events] "
                     "-consumers [consumers] "
                     "-suppliers [suppliers] "
                     "-event_count [event_count] ",
                     argv[0],
                     argv[0]));

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
AdminProperties::create_channel(bool reject)
{
  CosNotifyChannelAdmin::ChannelID id;

  initial_admin_.length (4);

  this->initial_admin_[0].name =
    CORBA::string_dup (CosNotification::MaxQueueLength);
  this->initial_admin_[0].value <<= this->max_queue_length_;


  this->initial_admin_[1].name =
    CORBA::string_dup (CosNotification::MaxSuppliers);
  this->initial_admin_[1].value <<= this->max_suppliers_;

  this->initial_admin_[2].name =
    CORBA::string_dup (CosNotification::MaxConsumers);
  this->initial_admin_[2].value <<= this->max_consumers_;


  this->initial_admin_[3].name =
    CORBA::string_dup (CosNotification::RejectNewEvents);
  this->initial_admin_[3].value <<= CORBA::Any::from_boolean (reject);

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));


  CosNotifyChannelAdmin::AdminID adminid;

  this->supplier_admin_ = ec_->new_for_suppliers (this->ifgop_,
                                                  adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  this->consumer_admin_ = ec_->new_for_consumers (this->ifgop_,
                                                  adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));
}

void
AdminProperties::run_test (void)
{
  bool reject = true;
  this->create_channel(reject);
  this->test_max_queue_length (reject);

  this->ec_->destroy();

  reject = false;
  this->create_channel(reject);
  this->test_max_queue_length (reject);

   this->test_max_clients ();

  this->ec_->destroy ();
}

void
AdminProperties::test_max_queue_length (bool reject)
{
  // Create the consumer
  AdminProperties_StructuredPushConsumer *consumer;
  ACE_NEW (consumer, AdminProperties_StructuredPushConsumer (this));
  consumer->init (root_poa_.in ());
  consumer->connect (this->consumer_admin_.in ());

  // Create the supplier
  TAO_Notify_Tests_StructuredPushSupplier *supplier = 0;
  ACE_NEW (supplier, TAO_Notify_Tests_StructuredPushSupplier ());
  supplier->init (root_poa_.in ());
  supplier->connect (this->supplier_admin_.in ());

  AdminProperties_Task supplier_task;

  // Init the Task to send events;
  supplier_task.init (supplier, this);

  if (supplier_task.activate (THR_NEW_LWP | THR_JOINABLE, 1) != 0)
    {
    ACE_ERROR ((LM_ERROR, "\nCannot activate supplier task\n"));
    }

  // All supplier events should be sent before the first consumer event is
  // received. This relies on our use of -ORBClientConnectionHandler RW.
  supplier_task.wait ();

  this->ORB_run();

  // consumer is destroyed by consumer->disconnect()
  CORBA::Long received_count = consumer->events_received_.value ();

  // disconnect the participants.
  consumer->disconnect ();
  supplier->disconnect ();

  // If the reject_new_events setting == true, then the supplier should
  // have received an imp_limit exception for each event it tried to push
  // after the maximum was reached.
  // If the reject_new_events setting == false, then the events should
  // have been discarded according to the DiscardPolicy, which for this
  // test we leave as AnyOrder.

  ACE_DEBUG ((LM_DEBUG, "\nSupplier sent %d events, consumer received %d events, max_queue_length = %d\n",
              event_count_, received_count, max_queue_length_));

  int expected_min = max_queue_length_;
  int expected_max = max_queue_length_ + max_consumers_;
  if (reject)
  {
    expected_max = event_count_ - rejections_;
    expected_min = expected_max;
  }

  if (reject && rejections_ != event_count_ - received_count)
    {
    ACE_ERROR ((LM_ERROR, "\nError: Expected %d rejections, but got %d\n",
      event_count_ - received_count, rejections_));
    return;
  }

  if (received_count < expected_min || received_count > expected_max)
  {
    ACE_ERROR ((LM_ERROR, "\nError: Expected %d to %d events, but received %d\n",
      expected_min, expected_max, received_count));
    }
}

void
AdminProperties::test_max_clients (void)
{
  this->create_suppliers ();

  this->create_consumers ();

  // check the results and print the assessment.
  if (this->consumers_connected_count_ > this->max_consumers_)
    ACE_DEBUG ((LM_ERROR, "\nConnected consumers %d, exceed MaxConsumers %d\n",
                this->consumers_connected_count_ > this->max_consumers_));

  if (this->suppliers_connected_count_ > this->max_suppliers_)
    ACE_DEBUG ((LM_ERROR, "\nConnected suppliers %d, exceed MaxSuppliers %d\n",
                this->suppliers_connected_count_ > this->max_suppliers_));
}

void
AdminProperties::create_suppliers (void)
{
  // Create the requested number of suppliers.
  // @@ CosNotifyChannelAdmin::AdminID adminid;
  // @@ CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
  // @@   CosNotifyChannelAdmin::OR_OP;

  int index = 0;

  try
    {
      TAO_Notify_Tests_StructuredPushSupplier *supplier;

      for (index = 0; index < this->suppliers_; ++index)
        {
          ACE_NEW (supplier,
                   TAO_Notify_Tests_StructuredPushSupplier ());
          supplier->init (root_poa_.in ());

          supplier->connect (this->supplier_admin_.in ());

          this->suppliers_connected_count_++;
        }
    }
  catch (const CORBA::IMP_LIMIT&)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG, "\nImpl Limit excpetion when connecting supplier\n"));
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("\nError: Exception in connecting supplier\n");
    }
}

void
AdminProperties::create_consumers (void)
{
  // Create the requested number of suppliers.
  // @@ CosNotifyChannelAdmin::AdminID adminid;
  // @@ CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
  // @@   CosNotifyChannelAdmin::OR_OP;

  int index = 0;

  try
    {
      TAO_Notify_Tests_StructuredPushConsumer *consumer;

      for (index = 0; index < this->consumers_; ++index)
        {
          ACE_NEW (consumer, TAO_Notify_Tests_StructuredPushConsumer ());
          consumer->init (root_poa_.in ());

          consumer->connect (this->consumer_admin_.in ());

          this->consumers_connected_count_++;
        }
    }
  catch (const CORBA::IMP_LIMIT&)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG, "\nImpl Limit exception when connecting consumer\n"));
    }
  catch (const CORBA::Exception& ex)
    {
        ex._tao_print_exception (
          "\nError: Exception in connecting consumer \n");
    }
}

/***************************************************************************/

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  AdminProperties test;

  try
    {
      test.init (argc, argv);

      test.run_test ();
    }
  catch (const CORBA::Exception& se)
    {
      se._tao_print_exception ("Error: ");
      return 1;
    }
  return 0;
}
