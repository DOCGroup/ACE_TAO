// $Id$

#include "AdminProperties.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"

/***************************************************************************/

AdminProperties_Task::AdminProperties_Task (void)
  :supplier_ (0), client_ (0)
{
}

void
AdminProperties_Task::init (TAO_Notify_StructuredPushSupplier *supplier, AdminProperties* client)
{
  supplier_  = supplier;
  client_ = client;
}

int
AdminProperties_Task::init (int argc, ACE_TCHAR *argv[])
{
  return ACE_Task_Base::init (argc,
                                  argv);
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

 ACE_DEBUG ((LM_DEBUG, "Supplier sending %d events...\n", event_count));

 ACE_DECLARE_NEW_CORBA_ENV;

 for (int i = 0 ; i < event_count; ++i)
   {
     event.filterable_data[0].value <<= (CORBA::Long)i;

     // Any.
     event.remainder_of_body <<= (CORBA::Long)i;

     ACE_TRY
        {
          this->supplier_->send_event (event ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
      ACE_CATCH (CORBA::IMP_LIMIT, impl_limit)
        {
          if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG, "Event %d was not send due to Impl Limit reached\n", i));

          this->client_->was_rejected_ = 1;
        }
      ACE_CATCHANY
        {
          ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, "Exception sending event\n");
        }
      ACE_ENDTRY;
   }

return 0;
}
/***************************************************************************/

AdminProperties_StructuredPushConsumer::AdminProperties_StructuredPushConsumer (AdminProperties* client)
  :client_ (client), events_received_ (0)
{
}

void
AdminProperties_StructuredPushConsumer::push_structured_event (const CosNotification::StructuredEvent & /*notification*/
                                                               ACE_ENV_ARG_DECL_NOT_USED
                                                               )
  ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosEventComm::Disconnected
                     ))
{
  ++events_received_;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Consumer %x received event %d\n", this, events_received_.value ()));

  ACE_OS::sleep (this->client_->consumer_delay_);
}

/***************************************************************************/

AdminProperties::AdminProperties (void)
  : max_queue_length_ (10),
    max_consumers_ (3),
    max_suppliers_ (3),
    reject_new_events_ (0),
    consumer_delay_ (0, 0),
    initial_delay_ (5, 0),
    consumers_ (4),
    suppliers_ (4),
    event_count_ (30),
    suppliers_connected_count_ (0),
    consumers_connected_count_ (0),
    was_rejected_ (0)
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
      if ((current_arg = arg_shifter.get_the_parameter ("-max_queue_length")))
        {
          this->max_queue_length_ = ACE_OS::atoi (current_arg);
          // Max. queue length.

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-max_consumers")))
        {
          this->max_consumers_ = ACE_OS::atoi (current_arg);
          // Max consumers allowed to connect.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-max_suppliers")))
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
      else if ((current_arg = arg_shifter.get_the_parameter ("-consumers")))
        {
          this->consumers_ = ACE_OS::atoi (current_arg);
          // Number of consumers to create.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-suppliers")))
        {
          this->suppliers_ = ACE_OS::atoi (current_arg);
          // Number of suppliers to create.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-ConsumerDelay")))
        {
          this->consumer_delay_ = ACE_Time_Value (ACE_OS::atoi (current_arg), 0); // Consumer delay in secs.

          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-InitialDelay")))
        {
          this->initial_delay_ = ACE_Time_Value (ACE_OS::atoi (current_arg), 0); // Initial delay in secs.

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
                     "-event_count [event_count] "
                     "-ConsumerDelay [delay_in_sec] "
                     "-InitialDelay [delay_in_secs]\n",
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

int
AdminProperties::init (int argc,
                            char *argv []
                            ACE_ENV_ARG_DECL)
{
  // Initialize base class.
  Notify_Test_Client::init (argc,
                            argv
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNotifyChannelAdmin::ChannelID id;

  // Initialize the admin object.
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
  this->initial_admin_[3].value <<= CORBA::Any::from_boolean (
                                        this->reject_new_events_
                                      );

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));


  CosNotifyChannelAdmin::AdminID adminid;

  this->supplier_admin_ = ec_->new_for_suppliers (this->ifgop_,
                                                  adminid
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  this->consumer_admin_ = ec_->new_for_consumers (this->ifgop_,
                                                  adminid
                                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  return 0;
}

void
AdminProperties::run_test (ACE_ENV_SINGLE_ARG_DECL)
{
  this->test_max_queue_length (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

   this->test_max_clients (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // Destroy the EC and hence release server resources.
  this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
AdminProperties::test_max_queue_length (ACE_ENV_SINGLE_ARG_DECL)
{
  // Create the consumer
  AdminProperties_StructuredPushConsumer *consumer;

  ACE_NEW (consumer,
           AdminProperties_StructuredPushConsumer (this));

  consumer->init (root_poa_.in ()
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  consumer->connect (this->consumer_admin_.in ()
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Create the supplier
  TAO_Notify_StructuredPushSupplier *supplier = 0;
  ACE_NEW (supplier,
           TAO_Notify_StructuredPushSupplier ());

  supplier->init (root_poa_.in ()
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier->connect (this->supplier_admin_.in ()
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  AdminProperties_Task supplier_task;

  // Init the Task to send events;
  supplier_task.init (supplier, this);

  if (supplier_task.activate (THR_NEW_LWP | THR_JOINABLE,
                              1) != 0)
    {
      ACE_ERROR ((LM_ERROR,
                  "Cannot activate supplier task\n"));
    }

  ACE_OS::sleep (initial_delay_); // Consumers receive events in the main thread. delay receiving the 1st event.

  CORBA::Long previous_count = 0;

  do{
    previous_count = consumer->events_received_.value ();

    // we add a full 1 sec. to the waiting period for a new event.
    ACE_Time_Value waiting_period (this->consumer_delay_.sec () + 1, 0);

    this->orb_->run (waiting_period);

  }while (previous_count != consumer->events_received_.value ());

  // Test ended.
  // wait for the supplier task to finish.
  supplier_task.wait ();

  CORBA::Long received_count = consumer->events_received_.value ();

  // disconnect the participants.
  consumer->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
  supplier->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);

  // check the results and print the assessment.
  // we expect to see event_count_+1 events

  ACE_DEBUG ((LM_DEBUG, "Supplier send %d events, consumer received %d events, max_queue_length = %d\n",
              event_count_, received_count, max_queue_length_));

  if (received_count > this->max_queue_length_ + 1)
    {
      ACE_DEBUG ((LM_ERROR, "Error: MaxQueueLength exceeded, try increasing consumer delay (currently = %d)\n",
                  this->consumer_delay_.sec ()));

      if (this->reject_new_events_ == 1 && was_rejected_ == 0)
        ACE_DEBUG ((LM_ERROR, "RejectNewEvents failed\n"));
    }
}

void
AdminProperties::test_max_clients (ACE_ENV_SINGLE_ARG_DECL)
{
  this->create_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->create_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  // check the results and print the assessment.
  if (this->consumers_connected_count_ > this->max_consumers_)
    ACE_DEBUG ((LM_ERROR, "Connected consumers %d, exceed MaxConsumers %d\n",
                this->consumers_connected_count_ > this->max_consumers_));

  if (this->suppliers_connected_count_ > this->max_suppliers_)
    ACE_DEBUG ((LM_ERROR, "Connected suppliers %d, exceed MaxSuppliers %d\n",
                this->suppliers_connected_count_ > this->max_suppliers_));
}

void
AdminProperties::create_suppliers (ACE_ENV_SINGLE_ARG_DECL)
{
  // Create the requested number of suppliers.
  // @@ CosNotifyChannelAdmin::AdminID adminid;
  // @@ CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
  // @@   CosNotifyChannelAdmin::OR_OP;

  int index = 0;

  ACE_TRY
    {
      TAO_Notify_StructuredPushSupplier *supplier;

      for (index = 0; index < this->suppliers_; ++index)
        {
          ACE_NEW (supplier,
                   TAO_Notify_StructuredPushSupplier ());
          supplier->init (root_poa_.in ()
                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          supplier->connect (this->supplier_admin_.in ()
                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->suppliers_connected_count_++;
        }
    }
  ACE_CATCH (CORBA::IMP_LIMIT, impl_limit)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG, "Impl Limit excpetion when connecting supplier\n"));
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in connecting supplier\n");
    }
  ACE_ENDTRY;
}

void
AdminProperties::create_consumers (ACE_ENV_SINGLE_ARG_DECL)
{
  // Create the requested number of suppliers.
  // @@ CosNotifyChannelAdmin::AdminID adminid;
  // @@ CosNotifyChannelAdmin::InterFilterGroupOperator ifgop =
  // @@   CosNotifyChannelAdmin::OR_OP;

  int index = 0;

  ACE_TRY
    {
      TAO_Notify_StructuredPushConsumer *consumer;

      for (index = 0; index < this->consumers_; ++index)
        {
          ACE_NEW (consumer,
                   TAO_Notify_StructuredPushConsumer ());
          consumer->init (root_poa_.in ()
                          ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          consumer->connect (this->consumer_admin_.in ()
                             ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;

          this->consumers_connected_count_++;
        }
    }
  ACE_CATCH (CORBA::IMP_LIMIT, impl_limit)
    {
      if (TAO_debug_level)
        ACE_DEBUG ((LM_DEBUG, "Impl Limit excpetion when connecting consumer\n"));
    }
  ACE_CATCHANY
    {
      if (TAO_debug_level)
        ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                             "Exception in connecting consumer \n");
    }
  ACE_ENDTRY;
}

/***************************************************************************/

int
main (int argc, char* argv[])
{
  AdminProperties test;

  ACE_TRY_NEW_ENV
    {
      test.init (argc,
                 argv
                 ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      test.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
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
