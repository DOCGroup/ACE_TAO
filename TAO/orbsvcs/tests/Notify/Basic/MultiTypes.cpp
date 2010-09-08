//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "MultiTypes.h"



//*****************************************************************************************************

MultiTypes_PushConsumer::MultiTypes_PushConsumer (MultiTypes* client)
  :client_ (client)
{
}

void
MultiTypes_PushConsumer::push (const CORBA::Any & /*data*/
                               )
{
  client_->on_received_event (this);
}

/***************************************************************************/

MultiTypes_StructuredPushConsumer::MultiTypes_StructuredPushConsumer (MultiTypes* client)
  :client_ (client)
{
}

void
MultiTypes_StructuredPushConsumer::push_structured_event (const CosNotification::StructuredEvent & /*notification*/
                                      )
{
  client_->on_received_event (this);
}

/***************************************************************************/

MultiTypes_SequencePushConsumer::MultiTypes_SequencePushConsumer (MultiTypes* client)
  :client_ (client)
{
}

// TODO: if the batch contains more than one event this counts only one received event
// Since this should *never* happen, I'm not fixing it now.
void
MultiTypes_SequencePushConsumer::push_structured_events (const CosNotification::EventBatch & /*notifications*/
                                                         )
{
  client_->on_received_event (this);
}

/***************************************************************************/

MultiTypes::MultiTypes (void)
  :any_consumer_ (0),
   structured_consumer_ (0),
   sequence_consumer_ (0),
   any_supplier_ (0),
   structured_supplier_ (0),
   sequence_supplier_ (0),
   any_event_count_ (0),
   struct_event_count_ (0),
   seq_event_count_ (0),
   disconnect_on_last_event_ (0)
{

}

MultiTypes::~MultiTypes ()
{
}

void
MultiTypes::on_received_event (MultiTypes_PushConsumer* consumer)
{
  ++any_event_count_;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "PushConsumer received event #%d\n", any_event_count_.value ()));


  if (disconnect_on_last_event_ == 1)
  {
    consumer->disconnect ();
    if (TAO_debug_level)
      ACE_DEBUG ((LM_DEBUG, "PushConsumer has been disconnected.\n"));
    consumer = 0;
  }
}

void
MultiTypes::on_received_event (MultiTypes_StructuredPushConsumer* consumer)
{
  ++struct_event_count_;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "StructuredPushConsumer received event #%d\n", struct_event_count_.value ()));


  if (disconnect_on_last_event_ == 1)
  {
    consumer->disconnect ();
    if (TAO_debug_level)
      ACE_DEBUG ((LM_DEBUG, "StructuredPushConsumer has been disconnected.\n"));
    consumer = 0;
  }
}

void
MultiTypes::on_received_event (MultiTypes_SequencePushConsumer* consumer)
{
  ++seq_event_count_;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "SequencePushConsumer received event #%d\n", seq_event_count_.value ()));


  if (disconnect_on_last_event_ == 1)
  {
    consumer->disconnect ();

    if (TAO_debug_level)
      ACE_DEBUG ((LM_DEBUG, "SequencePushConsumer has been disconnected.\n"));
    consumer = 0;
  }
}

int
MultiTypes::init (int argc,
                         ACE_TCHAR* argv [])
{
  // Initialize the base class.
  Notify_Test_Client::init (argc,
                            argv);

  // Create all participants.
  this->create_EC ();

  CosNotifyChannelAdmin::AdminID adminid;

  this->supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_,
                            adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  this->consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_,
                            adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  // Create the consumers and suppliers.
  any_consumer_ = new MultiTypes_PushConsumer (this);
  structured_consumer_ = new MultiTypes_StructuredPushConsumer (this);
  sequence_consumer_ = new MultiTypes_SequencePushConsumer (this);

  any_supplier_ = new TAO_Notify_Tests_PushSupplier;
  structured_supplier_ = new TAO_Notify_Tests_StructuredPushSupplier ();
  sequence_supplier_ = new TAO_Notify_Tests_SequencePushSupplier ();

  // Init and connect all consumers.
  structured_consumer_->init (root_poa_.in ());

  structured_consumer_->connect (this->consumer_admin_.in ());

  any_consumer_->init (root_poa_.in ());

  any_consumer_->connect (this->consumer_admin_.in ());


  sequence_consumer_->init (root_poa_.in ());

  sequence_consumer_->connect (this->consumer_admin_.in ());

  // Init and connect all suppliers.
  any_supplier_->init (root_poa_.in ());

  any_supplier_->connect (this->supplier_admin_.in ());

  structured_supplier_->init (root_poa_.in ());

  structured_supplier_->connect (this->supplier_admin_.in ());

  sequence_supplier_->init (root_poa_.in ());

  sequence_supplier_->connect (this->supplier_admin_.in ());

  consumer_start( 0 );

  return 0;
}

int
MultiTypes::parse_args(int, ACE_TCHAR **)
{
  // Doesn't accept any arguments
  return 0;
}

void
MultiTypes::create_EC (void)
{
  CosNotifyChannelAdmin::ChannelID id;

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
MultiTypes::run_test (void)
{
  // Send an Any, all consumers should receive it.
  CORBA::Any any;
  any <<= (CORBA::Long)0;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Sending Any Event..\n"));
  any_supplier_->send_event (any);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Waiting for consumers to receive the 1 event..\n"));
  this->wait_for_all_consumers (1);

  /*****************************************************************************/

  // Reset Count
  any_event_count_ = 0;
  struct_event_count_ = 0;
  seq_event_count_ = 0;

  // Send Structured Event, all consumers should receive it.
  CosNotification::StructuredEvent event;
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup("*");
  event.header.fixed_header.event_type.type_name = CORBA::string_dup("*");
  event.header.fixed_header.event_name = CORBA::string_dup("myevent");

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Sending Structured Event..\n"));
  structured_supplier_->send_event (event);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Waiting for consumers to receive the 1 event..\n"));
  this->wait_for_all_consumers (1);

  /*****************************************************************************/

  // Reset Count
  any_event_count_ = 0;
  struct_event_count_ = 0;
  seq_event_count_ = 0;

  // Send Structured Events, all consumers should receive it.
  CosNotification::EventBatch events;
  events.length (2);

  event.header.fixed_header.event_name = CORBA::string_dup("myevent_1");

  events[0] = event;

  event.header.fixed_header.event_name = CORBA::string_dup("myevent_2");

  events[1] = event;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Sending Sequence Event..\n"));
  sequence_supplier_->send_events (events);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Waiting for consumers to receive the 2 events..\n"));
  this->wait_for_all_consumers (2);

  /*****************************************************************************/
  // Reset Count
  any_event_count_ = 0;
  struct_event_count_ = 0;
  seq_event_count_ = 0;

  // set flag to disconnect consumers when event received.
  disconnect_on_last_event_ = 1;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Sending LAST Any Event, Consumers will attempt disconnect..\n"));
  any_supplier_->send_event (any);

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Waiting for consumers to receive the 1 event..\n"));
  this->wait_for_all_consumers (1);

  ACE_DEBUG ((LM_DEBUG, "MultiTypes test has run successfully!\n"));
}

void
MultiTypes::wait_for_all_consumers (int expected_count_per_consumer)
{
  while (true)
    {
      if (any_event_count_.value () >= expected_count_per_consumer &&
          struct_event_count_.value () >= expected_count_per_consumer &&
          seq_event_count_.value () >= expected_count_per_consumer)
      {
        break;
      }

      ACE_Time_Value tv (0, 100 * 1000);
      this->orb_->run(tv);
    }
}

void
MultiTypes::end_test (void)
{
  consumer_done( 0 );
}

int
MultiTypes::check_results (void)
{
  // Destroy the channel.
  this->ec_->destroy ();

  return 0;
}

/***************************************************************************/

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  MultiTypes client;

  if (client.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  try
    {
      client.init (argc,
                   argv);

      client.run_test ();

      client.end_test ();
    }
  catch (const CORBA::Exception& se)
    {
      se._tao_print_exception ("Error: ");
      return 1;
    }

  return client.check_results ();
}
