//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "MultiTypes.h"

ACE_RCSID (Notify_Tests, MultiTypes, "$Id$")

//*****************************************************************************************************

MultiTypes_PushConsumer::MultiTypes_PushConsumer (MultiTypes* client)
  :client_ (client)
{
}

void
MultiTypes_PushConsumer::push (const CORBA::Any & /*data*/
                               ACE_ENV_ARG_DECL_NOT_USED
                               )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
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
                                      ACE_ENV_ARG_DECL_NOT_USED
                                      )
    ACE_THROW_SPEC ((
                     CORBA::SystemException,
                     CosEventComm::Disconnected
                     ))
{
  client_->on_received_event (this);
}

/***************************************************************************/

MultiTypes_SequencePushConsumer::MultiTypes_SequencePushConsumer (MultiTypes* client)
  :client_ (client)
{
}

void
MultiTypes_SequencePushConsumer::push_structured_events (const CosNotification::EventBatch & /*notifications*/
                                                         ACE_ENV_ARG_DECL_NOT_USED
                                                         )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
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

  ACE_DECLARE_NEW_CORBA_ENV;

  if (disconnect_on_last_event_ == 1)
    consumer->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
MultiTypes::on_received_event (MultiTypes_StructuredPushConsumer* consumer)
{
  ++struct_event_count_;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "StructuredPushConsumer received event #%d\n", struct_event_count_.value ()));

  ACE_DECLARE_NEW_CORBA_ENV;

  if (disconnect_on_last_event_ == 1)
    consumer->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
}

void
MultiTypes::on_received_event (MultiTypes_SequencePushConsumer* consumer)
{
  ++seq_event_count_;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "SequencePushConsumer received event #%d\n", seq_event_count_.value ()));

  ACE_DECLARE_NEW_CORBA_ENV;

  if (disconnect_on_last_event_ == 1)
    consumer->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
MultiTypes::init (int argc,
                         char* argv []
                         ACE_ENV_ARG_DECL)
{
  // Initialize the base class.
  Notify_Test_Client::init (argc,
                            argv
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);;

  // Create all participants.
  this->create_EC (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNotifyChannelAdmin::AdminID adminid;

  this->supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_,
                            adminid
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);;

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  this->consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_,
                            adminid
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  // Create the consumers and suppliers.
  any_consumer_ = new MultiTypes_PushConsumer (this);
  structured_consumer_ = new MultiTypes_StructuredPushConsumer (this);
  sequence_consumer_ = new MultiTypes_SequencePushConsumer (this);

  any_supplier_ = new TAO_Notify_PushSupplier;
  structured_supplier_ = new TAO_Notify_StructuredPushSupplier ();
  sequence_supplier_ = new TAO_Notify_SequencePushSupplier ();

  // Init and connect all consumers.
  structured_consumer_->init (root_poa_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  structured_consumer_->connect (this->consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  any_consumer_->init (root_poa_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  any_consumer_->connect (this->consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);


  sequence_consumer_->init (root_poa_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  sequence_consumer_->connect (this->consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Init and connect all suppliers.
  any_supplier_->init (root_poa_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  any_supplier_->connect (this->supplier_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  structured_supplier_->init (root_poa_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  structured_supplier_->connect (this->supplier_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  sequence_supplier_->init (root_poa_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  sequence_supplier_->connect (this->supplier_admin_.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
MultiTypes::parse_args(int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc,
                              argv);

    const char *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-count")))
        {
          // The number of counsumers to create.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-count testcount \n",
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
MultiTypes::create_EC (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::ChannelID id;

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id
                                               ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
MultiTypes::run_test (ACE_ENV_SINGLE_ARG_DECL)
{
  // Send an Any, all consumers should receive it.
  CORBA::Any any;
  any <<= (CORBA::Long)0;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Sending Any Event..\n"));
  any_supplier_->send_event (any ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Waiting for consumers to receive the event..\n"));
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
  structured_supplier_->send_event (event ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Waiting for consumers to receive the event..\n"));
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
  sequence_supplier_->send_events (events ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Waiting for consumers to receive the event..\n"));
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
  any_supplier_->send_event (any ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Waiting for consumers to receive the event..\n"));
  this->wait_for_all_consumers (1);

  ACE_DEBUG ((LM_DEBUG, "MultiTypes test has run successfully!\n"));
}

void
MultiTypes::wait_for_all_consumers (int expected_count_per_consumer)
{
  while (1)
    {
      if (any_event_count_.value () == expected_count_per_consumer &&
          struct_event_count_.value () == expected_count_per_consumer &&
          seq_event_count_.value () == expected_count_per_consumer)
        break;

      {
        if (this->orb_->work_pending ())
          this->orb_->perform_work ();
      }
    }
}

void
MultiTypes::end_test (ACE_ENV_SINGLE_ARG_DECL)
{
  this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
MultiTypes::check_results (void)
{
  // Destroy the channel.
  ACE_DECLARE_NEW_CORBA_ENV;
  this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

/***************************************************************************/

int
main (int argc, char* argv[])
{
  MultiTypes client;

  if (client.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  ACE_TRY_NEW_ENV
    {
      client.init (argc,
                   argv
                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      client.end_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "MultiTypes user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "MultiTypes system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return client.check_results ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
