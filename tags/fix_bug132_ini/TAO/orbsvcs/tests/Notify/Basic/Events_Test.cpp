//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "Events_Test.h"

ACE_RCSID (Notify_Tests, Events_Test, "$Id$")

/***************************************************************************/

Event_StructuredPushConsumer::Event_StructuredPushConsumer (Events_Test *test_client)
  : test_client_ (test_client)
{
}

void
Event_StructuredPushConsumer::push_structured_event (const CosNotification::StructuredEvent & notification, CORBA::Environment &/*ACE_TRY_ENV*/)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  int event_num;
  notification.filterable_data[0].value >>= event_num;

  ACE_DEBUG((LM_DEBUG, "Received event# %d\n", event_num));

  this->test_client_->on_event_received ();
}

/***************************************************************************/

Event_StructuredPushSupplier::Event_StructuredPushSupplier (Events_Test* test_client)
  :test_client_ (test_client)
{
}

Event_StructuredPushSupplier::~Event_StructuredPushSupplier ()
{
}

/***************************************************************************/
Events_Test::Events_Test (void)
  :event_count_ (5)
{
}

Events_Test::~Events_Test ()
{
}

void
Events_Test::init (int argc, char* argv [], CORBA::Environment &ACE_TRY_ENV)
{
  // init base class
  Notify_Test_Client::init (argc, argv, ACE_TRY_ENV);
  ACE_CHECK;

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

  consumer_ = new Event_StructuredPushConsumer (this);
  consumer_->init (root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;
  consumer_->connect (this->consumer_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  Event_StructuredPushConsumer* consumer2;
  consumer2 = new Event_StructuredPushConsumer (this);
  consumer2->init (root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;
  consumer2->connect (this->consumer_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  supplier_ = new Event_StructuredPushSupplier (this);
  supplier_->init (root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  supplier_->connect (this->supplier_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;

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
Events_Test::parse_args(int argc, char *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc, argv);

    char *current_arg = 0;
    while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-events")))
        {
          this->event_count_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-events event_count \n",
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
Events_Test::create_EC (CORBA::Environment &ACE_TRY_ENV)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id,
                                         ACE_TRY_ENV);
  ACE_CHECK;

  /****************************************************************/
  /*
  CosNotification::AdminProperties admin(2);
  admin.length (2);

  admin[0].name =
    CORBA::string_dup(CosNotification::MaxQueueLength);

  admin[0].value <<= (CORBA::Long)5;

  admin[1].name =
    CORBA::string_dup(CosNotification::MaxConsumers);

  admin[1].value <<= (CORBA::Long)2;

  ec_->set_admin (admin, ACE_TRY_ENV);
  ACE_CHECK;*/

  /****************************************************************/
  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Events_Test::on_event_received (void)
{
  ++this->result_count_;

  ACE_DEBUG ((LM_DEBUG, "event count = #%d\n", this->result_count_.value ()));

  if (this->result_count_ == 2*this->event_count_)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      this->end_test (ACE_TRY_ENV);
    }
}

void
Events_Test::run_test (CORBA::Environment &ACE_TRY_ENV)
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

  for (int i = 0; i < this->event_count_; ++i)
    {
      event.filterable_data[0].value <<= (CORBA::Long)i;

      // any
      event.remainder_of_body <<= (CORBA::Long)i;

      qos[0].name = CORBA::string_dup (CosNotification::Priority);
      qos[0].value <<= (CORBA::Short)prio++;

      supplier_->send_event (event, ACE_TRY_ENV);
      ACE_CHECK;
    }
}

void
Events_Test::end_test (CORBA::Environment &ACE_TRY_ENV)
{
  this->shutdown (ACE_TRY_ENV); // break out of run
}

int
Events_Test::check_results (void)
{
  // Destroy the channel
  ACE_DECLARE_NEW_CORBA_ENV;
  this->ec_->destroy (ACE_TRY_ENV);
  ACE_CHECK_RETURN (-1);

  if (this->result_count_ == 2*this->event_count_)
    {
      ACE_DEBUG ((LM_DEBUG, "Events test success\n"));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "Events test failed!\n"));
      return 1;
    }
}

/***************************************************************************/

int
main (int argc, char* argv[])
{
  Events_Test events;

  if (events.parse_args (argc, argv) == -1)
    return 1;

  ACE_TRY_NEW_ENV
    {
      events.init (argc, argv,
                      ACE_TRY_ENV); //Init the Client
      ACE_TRY_CHECK;

      events.run_test (ACE_TRY_ENV);
      ACE_TRY_CHECK;

      events.ORB_run ();
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

  return events.check_results ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)

template class  ACE_Atomic_Op<TAO_SYNCH_MUTEX, int>;

#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)

#pragma instantiate ACE_Atomic_Op<TAO_SYNCH_MUTEX, int>

#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
