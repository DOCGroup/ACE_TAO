// $Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/OS_NS_unistd.h"
#include "tao/debug.h"
#include "Sequence.h"



/***************************************************************************/

SequencePushConsumer::SequencePushConsumer (Sequence *test_client)
  : test_client_ (test_client)
{
}

void
SequencePushConsumer::push_structured_events (
    const CosNotification::EventBatch &batch)
{
  this->test_client_->events_received_ += batch.length ();

  if (batch.length () > this->test_client_->consumer_batch_size_)
      ACE_DEBUG ((LM_ERROR,
                  "Error: Received more than max event batch %d\n",
                  batch.length ()));

  this->test_client_->on_event_received ();

  ACE_OS::sleep (this->test_client_->consumer_delay_);
}

/***************************************************************************/

SequencePushSupplier::SequencePushSupplier (
    Sequence* test_client)
  : test_client_ (test_client)
{
}

SequencePushSupplier::~SequencePushSupplier (void)
{
}

/***************************************************************************/
Sequence::Sequence (void)
  : event_count_ (15), supplier_batch_size_ (5), consumer_batch_size_ (3),
    pacing_ (2), order_policy_ (CosNotification::PriorityOrder), events_received_ (0),
    consumer_delay_ (1)
{
}

Sequence::~Sequence (void)
{
}

int
Sequence::init (int argc,
                   ACE_TCHAR* argv [])
{
  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Options: event count = %d\n"
                  "supplier batch size = %d\n"
                  "consumer batch size = %d\n"
                  "pacing = %d secs\n"
                  , event_count_
                  , supplier_batch_size_
                  , consumer_batch_size_
                  , pacing_));

      ACE_DEBUG ((LM_DEBUG, "consumer delay = %d\n", consumer_delay_.sec ()));
    }

  // Initialize the base class.
  Notify_Test_Client::init (argc,
                            argv);

  // Create all participents.
  this->create_EC ();

  CosNotifyChannelAdmin::AdminID adminid;

  this->supplier_admin_ =
    this->ec_->new_for_suppliers (this->ifgop_,
                                  adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  this->consumer_admin_ =
    this->ec_->new_for_consumers (this->ifgop_,
                                  adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  ACE_NEW_RETURN (this->consumer_,
                  SequencePushConsumer (this),
                  -1);
  this->consumer_->init (root_poa_.in ());
  this->consumer_->connect (this->consumer_admin_.in ());

  CosNotification::QoSProperties properties (3);
  properties.length (3);

  properties[0].name = CORBA::string_dup (CosNotification::MaximumBatchSize);
  properties[0].value <<= (CORBA::Long) this->consumer_batch_size_;
  properties[1].name = CORBA::string_dup (CosNotification::PacingInterval);
  properties[1].value <<= (TimeBase::TimeT) (this->pacing_ * 1000 * 10000);
  properties[2].name = CORBA::string_dup (CosNotification::OrderPolicy);
  properties[2].value <<= this->order_policy_;

  this->consumer_->get_proxy_supplier ()->set_qos (properties);

  ACE_NEW_RETURN (this->supplier_,
                  SequencePushSupplier (this),
                  -1);
  this->supplier_->init (root_poa_.in ());

  this->supplier_->connect (this->supplier_admin_.in ());

  consumer_start (0);

  return 0;
}

int
Sequence::parse_args (int argc,
                         ACE_TCHAR *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc,
                                 argv);
    const ACE_TCHAR *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-events"))))
        {
          this->event_count_ = ACE_OS::atoi (current_arg); // The number of events to send/receive.

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-SupplierBatchSize"))))
        {
          this->supplier_batch_size_ = ACE_OS::atoi (current_arg); // Supplier batch size

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ConsumerBatchSize"))))
        {
          this->consumer_batch_size_ = ACE_OS::atoi (current_arg); // Consumer batch size

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-ConsumerDelay"))))
        {
          this->consumer_delay_ = ACE_Time_Value (ACE_OS::atoi (current_arg), 0); // Consumer delay in secs.

          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-Pacing")))) // in seconds
        {
          this->pacing_ = (TimeBase::TimeT) ACE_OS::atoi (current_arg);

          arg_shifter.consume_arg ();
        }

      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-?")) == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-events event_count "
                     "-SupplierBatchSize size "
                     "-ConsumerBatchSize size "
                     "-ConsumerDelay delay "
                     "-Pacing pacing\n",
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
Sequence::create_EC (void)
{
  CosNotifyChannelAdmin::ChannelID id;

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id);
  ACE_ASSERT (!CORBA::is_nil (this->ec_.in ()));
}

void
Sequence::on_event_received (void)
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "Events received = %d\n",
                this->events_received_.value ()));

  if (this->events_received_.value () == this->event_count_)
    {
      this->end_test ();
    }
}

void
Sequence::run_test (void)
{
  // operations:
  CosNotification::StructuredEvent event;

  // EventHeader.

  // FixedEventHeader.
  // EventType.
  // string.
  event.header.fixed_header.event_type.domain_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_type.type_name = CORBA::string_dup("*");
  // string
  event.header.fixed_header.event_name = CORBA::string_dup("myevent");

  // OptionalHeaderFields.
  // PropertySeq.
  // sequence<Property>: string name, any value
  CosNotification::PropertySeq& qos =  event.header.variable_header;
  qos.length (1); // put nothing here

  // FilterableEventBody
  // PropertySeq
  // sequence<Property>: string name, any value
  event.filterable_data.length (3);
  event.filterable_data[0].name = CORBA::string_dup("threshold");

  event.filterable_data[1].name = CORBA::string_dup("temperature");
  event.filterable_data[1].value <<= (CORBA::Long)70;

  event.filterable_data[2].name = CORBA::string_dup("pressure");
  event.filterable_data[2].value <<= (CORBA::Long)80;

  CORBA::Short prio = CosNotification::LowestPriority;

  CosNotification::EventBatch batch;
  batch.length (this->supplier_batch_size_);
  CORBA::ULong batch_index = 0;

  for (int i = 0; i < this->event_count_; ++i)
    {
      event.filterable_data[0].value <<= (CORBA::Long)i;

      // any
      event.remainder_of_body <<= (CORBA::Long)i;

      qos[0].name = CORBA::string_dup (CosNotification::Priority);
      qos[0].value <<= (CORBA::Short)prio++;

      batch[batch_index] = event;
      batch_index++;

      if (batch_index == this->supplier_batch_size_)
        {
          batch.length (batch_index); // set the correct length

          if (TAO_debug_level)
            ACE_DEBUG ((LM_DEBUG, "Sending batch with %d events\n", batch.length ()));

          this->supplier_->send_events (batch);

          // reset
          batch.length (this->supplier_batch_size_);
          batch_index = 0;
        }
    } // for

  // send the last batch.
  if (batch_index > 0)
    {
      batch.length (batch_index); // set the correct length

      this->supplier_->send_events (batch);
    }

}

void
Sequence::end_test (void)
{
  consumer_done( 0 );
}

int
Sequence::check_results (void)
{
  // Destroy the channel.
  this->ec_->destroy ();

  if (this->events_received_.value () == this->event_count_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Sequence test success\n"));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Sequence test failed!\n"));
      return 1;
    }
}

/***************************************************************************/

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Sequence events;

  if (events.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  try
    {
      events.init (argc, argv);

      events.run_test ();

      events.ORB_run( );
    }
  catch (const CORBA::Exception& se)
    {
      se._tao_print_exception ("Error: ");
      return 1;
    }

  int status = 0;

  try
    {
      status = events.check_results ();
    }
  catch (const CORBA::Exception& se)
    {
      se._tao_print_exception ("Error: ");
      status = 1;
    }

  return status;
}
