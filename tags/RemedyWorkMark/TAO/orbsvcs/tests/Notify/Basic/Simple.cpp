//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "Simple.h"



//***************************************************************************

Event_AnyPushConsumer::Event_AnyPushConsumer (Simple_Test *test_client)
  : test_client_ (test_client)
{
}

void
Event_AnyPushConsumer::push (const CORBA::Any & data)
{
  int event_num;
  data >>= event_num;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "Received event# %d\n",
                event_num));

  this->test_client_->on_event_received ();
}

//***************************************************************************

Event_AnyPushSupplier::Event_AnyPushSupplier (Simple_Test* test_client)
  : test_client_ (test_client)
{
}

Event_AnyPushSupplier::~Event_AnyPushSupplier (void)
{
}

//***************************************************************************

Simple_Test::Simple_Test (void)
  : event_count_ (5)
{
}

Simple_Test::~Simple_Test (void)
{
}

int
Simple_Test::init (int argc,
                   ACE_TCHAR* argv [])
{
  // Initialized the base class.
  Notify_Test_Client::init (argc,
                            argv);

  // Create all participents.
  this->create_EC ();

  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    this->ec_->new_for_suppliers (this->ifgop_,
                                  adminid);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  consumer_admin_ =
    this->ec_->new_for_consumers (this->ifgop_,
                                  adminid);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  ACE_NEW_RETURN (this->consumer_,
                  Event_AnyPushConsumer (this),
                  -1);
  this->consumer_->init (root_poa_.in ());
  this->consumer_->connect (this->consumer_admin_.in ());

  Event_AnyPushConsumer* consumer2;
  ACE_NEW_RETURN (consumer2,
                  Event_AnyPushConsumer (this),
                  -1);
  consumer2->init (root_poa_.in ());
  consumer2->connect (this->consumer_admin_.in ());

  ACE_NEW_RETURN (this->supplier_,
                  Event_AnyPushSupplier (this),
                  -1);
  this->supplier_->init (root_poa_.in ());

  this->supplier_->connect (this->supplier_admin_.in ());

  consumer_start( 0 );

  return 0;
}

int
Simple_Test::parse_args (int argc,
                         ACE_TCHAR *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc,
                                 argv);

    const ACE_TCHAR *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-events"))))
        {
          this->event_count_ = ACE_OS::atoi (current_arg);
          // The number of events to send/receive.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-?")) == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "usage: %s "
                      "-events event_count\n",
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
Simple_Test::create_EC (void)
{
  CosNotifyChannelAdmin::ChannelID id;

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Simple_Test::on_event_received (void)
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
Simple_Test::run_test (void)
{
  CORBA::Any data;

  for (int i = 0; i < this->event_count_; ++i)
    {
      data <<= (CORBA::Long)i;

      this->supplier_->send_event (data);
    }
}

void
Simple_Test::end_test (void)
{
  consumer_done( 0 );
}

int
Simple_Test::check_results (void)
{
  // Destroy the channel
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

//***************************************************************************

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  Simple_Test events;

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
