//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "Simple.h"

ACE_RCSID (Notify_Tests, Simple, "$Id$")

/***************************************************************************/

Event_AnyPushConsumer::Event_AnyPushConsumer (Simple_Test *test_client)
  : test_client_ (test_client)
{
}

void
Event_AnyPushConsumer::push (const CORBA::Any & data  TAO_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosEventComm::Disconnected
                   ))
{
  int event_num;
  data >>= event_num;

  ACE_DEBUG((LM_DEBUG, "Received event# %d\n", event_num));

  this->test_client_->on_event_received ();
}

/***************************************************************************/

Event_AnyPushSupplier::Event_AnyPushSupplier (Simple_Test* test_client)
  :test_client_ (test_client)
{
}

Event_AnyPushSupplier::~Event_AnyPushSupplier ()
{
}

/***************************************************************************/
Simple_Test::Simple_Test (void)
  :event_count_ (5)
{
}

Simple_Test::~Simple_Test ()
{
}

void
Simple_Test::init (int argc, char* argv [] TAO_ENV_ARG_DECL)
{
  // init base class
  Notify_Test_Client::init (argc, argv TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Create all participents ...
  this->create_EC (TAO_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  consumer_ = new Event_AnyPushConsumer (this);
  consumer_->init (root_poa_.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  consumer_->connect (this->consumer_admin_.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  Event_AnyPushConsumer* consumer2;
  consumer2 = new Event_AnyPushConsumer (this);
  consumer2->init (root_poa_.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
  consumer2->connect (this->consumer_admin_.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier_ = new Event_AnyPushSupplier (this);
  supplier_->init (root_poa_.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier_->connect (this->supplier_admin_.in () TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  // Setup the CA to receive all type of events
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);
  removed.length (0);

  added[0].domain_name =  CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added, removed TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

int
Simple_Test::parse_args(int argc, char *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc, argv);

    const char *current_arg = 0;
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
Simple_Test::create_EC (TAO_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id
                                         TAO_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Simple_Test::on_event_received (void)
{
  ++this->result_count_;

  ACE_DEBUG ((LM_DEBUG, "event count = #%d\n", this->result_count_.value ()));

  if (this->result_count_ == 2*this->event_count_)
    {
      TAO_ENV_DECLARE_NEW_ENV;
      this->end_test (TAO_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Simple_Test::run_test (TAO_ENV_SINGLE_ARG_DECL)
{
  CORBA::Any data;

  for (int i = 0; i < this->event_count_; ++i)
    {
      data <<= (CORBA::Long)i;

      supplier_->send_event (data TAO_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Simple_Test::end_test (TAO_ENV_SINGLE_ARG_DECL)
{
  this->shutdown (TAO_ENV_SINGLE_ARG_PARAMETER); // break out of run
}

int
Simple_Test::check_results (void)
{
  // Destroy the channel
  TAO_ENV_DECLARE_NEW_ENV;
  this->ec_->destroy (TAO_ENV_SINGLE_ARG_PARAMETER);
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
  Simple_Test events;

  if (events.parse_args (argc, argv) == -1)
    return 1;

  ACE_TRY_NEW_ENV
    {
      events.init (argc, argv
                      TAO_ENV_ARG_PARAMETER); //Init the Client
      ACE_TRY_CHECK;

      events.run_test (TAO_ENV_SINGLE_ARG_PARAMETER);
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


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
