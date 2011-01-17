//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "Events.h"



/***************************************************************************/

Event_StructuredPushConsumer::Event_StructuredPushConsumer (Events *test_client)
  : test_client_ (test_client)
{
}

void
Event_StructuredPushConsumer::push_structured_event (
    const CosNotification::StructuredEvent & notification
  )
{
  int event_num;
  notification.filterable_data[0].value >>= event_num;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "Received event# %d\n",
                event_num));

  this->test_client_->on_event_received ();
}

/***************************************************************************/

Event_StructuredPushSupplier::Event_StructuredPushSupplier (
    Events* test_client
  )
  : test_client_ (test_client)
{
}

Event_StructuredPushSupplier::~Event_StructuredPushSupplier (void)
{
}

/***************************************************************************/
Events::Events (void)
  : use_default_admin_ (0), event_count_ (5)
{
}

Events::~Events (void)
{
}

int
Events::init (int argc,
                   ACE_TCHAR* argv [])
{
  // Initialize the base class.
  Notify_Test_Client::init (argc,
                            argv);

  // Create all participents.
  this->create_EC ();

  CosNotifyChannelAdmin::AdminID adminid;

  if (use_default_admin_ == 1)
    {
      this->supplier_admin_ =
        this->ec_->default_supplier_admin ();
    }
  else
    {
      this->supplier_admin_ =
        this->ec_->new_for_suppliers (this->ifgop_,
                                      adminid);
    }

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  if (use_default_admin_ == 1)
    {
      this->consumer_admin_ =
        this->ec_->default_consumer_admin ();
    }
  else
    {
      this->consumer_admin_ =
        this->ec_->new_for_consumers (this->ifgop_,
                                      adminid);
    }

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  ACE_NEW_RETURN (this->consumer_,
                  Event_StructuredPushConsumer (this),
                  -1);
  this->consumer_->init (root_poa_.in ());
  this->consumer_->connect (this->consumer_admin_.in ());

  Event_StructuredPushConsumer* consumer2 = 0;
  ACE_NEW_RETURN (consumer2,
                  Event_StructuredPushConsumer (this),
                  -1);
  consumer2->init (root_poa_.in ());
  consumer2->connect (this->consumer_admin_.in ());

  ACE_NEW_RETURN (this->supplier_,
                  Event_StructuredPushSupplier (this),
                  -1);
  this->supplier_->init (root_poa_.in ());

  this->supplier_->connect (this->supplier_admin_.in ());

  consumer_start( 0 );

  return 0;
}

int
Events::parse_args (int argc,
                         ACE_TCHAR *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc,
                                 argv);
    const ACE_TCHAR *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-use_default_admin")) == 0)
        {
          this->use_default_admin_ = 1;
          arg_shifter.consume_arg ();

        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-events"))))
        {
          this->event_count_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-?")) == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-use_default_admin "
                     "-events event_count\n",
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
Events::create_EC (void)
{
  CosNotifyChannelAdmin::ChannelID id;

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id);

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

  ec_->set_admin (admin);
  */

  /****************************************************************/
  ACE_ASSERT (!CORBA::is_nil (this->ec_.in ()));
}

void
Events::on_event_received (void)
{
  ++this->result_count_;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "event count = #%d\n",
                this->result_count_.value ()));

  if (this->result_count_ == 2 * this->event_count_)
    {
      this->end_test ();
    }
}

void
Events::run_test (void)
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

  for (int i = 0; i < this->event_count_; ++i)
    {
      event.filterable_data[0].value <<= (CORBA::Long)i;

      // any
      event.remainder_of_body <<= (CORBA::Long)i;

      qos[0].name = CORBA::string_dup (CosNotification::Priority);
      qos[0].value <<= (CORBA::Short)prio++;

      this->supplier_->send_event (event);
    }
}

void
Events::end_test (void)
{
  consumer_done( 0 );
}

int
Events::check_results (void)
{
  // Destroy the channel.
  this->ec_->destroy ();

  if (this->result_count_ == 2 * this->event_count_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Events test success\n"));
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Events test failed!\n"));
      return 1;
    }
}

/***************************************************************************/

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Events events;

  if (events.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  try
    {
      events.init (argc,
                   argv);

      events.run_test ();

      events.ORB_run( );
    }
  catch (const CORBA::Exception& se)
    {
      se._tao_print_exception ("Error: ");
      return 1;
    }

  return events.check_results ();
}
