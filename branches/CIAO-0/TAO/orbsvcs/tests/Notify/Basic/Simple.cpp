//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "Simple.h"

ACE_RCSID (Notify_Tests, Simple, "$Id$")

//***************************************************************************

Event_AnyPushConsumer::Event_AnyPushConsumer (Simple_Test *test_client)
  : test_client_ (test_client)
{
}

void
Event_AnyPushConsumer::push (const CORBA::Any & data
                             ACE_ENV_ARG_DECL_NOT_USED)
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosEventComm::Disconnected))
{
  int event_num;
  data >>= event_num;

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
                   char* argv []
                   ACE_ENV_ARG_DECL)
{
  // Initialized the base class.
  Notify_Test_Client::init (argc,
                            argv
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create all participents.
  this->create_EC (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    this->ec_->new_for_suppliers (this->ifgop_,
                                  adminid
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  consumer_admin_ =
    this->ec_->new_for_consumers (this->ifgop_,
                                  adminid
                                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  ACE_NEW_RETURN (this->consumer_,
                  Event_AnyPushConsumer (this),
                  -1);
  this->consumer_->init (root_poa_.in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  this->consumer_->connect (this->consumer_admin_.in ()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  Event_AnyPushConsumer* consumer2;
  ACE_NEW_RETURN (consumer2,
                  Event_AnyPushConsumer (this),
                  -1);
  consumer2->init (root_poa_.in ()
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);
  consumer2->connect (this->consumer_admin_.in ()
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_NEW_RETURN (this->supplier_,
                  Event_AnyPushSupplier (this),
                  -1);
  this->supplier_->init (root_poa_.in ()
                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  this->supplier_->connect (this->supplier_admin_.in ()
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Setup the CA to receive all type of events
  CosNotification::EventTypeSeq added(1);
  CosNotification::EventTypeSeq removed (0);
  added.length (1);
  removed.length (0);

  added[0].domain_name =  CORBA::string_dup ("*");
  added[0].type_name = CORBA::string_dup ("*");

  this->consumer_admin_->subscription_change (added,
                                              removed
                                              ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Simple_Test::parse_args (int argc,
                         char *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc,
                                 argv);

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
          ACE_DEBUG ((LM_DEBUG,
                      "usage: %s "
                      "-events event_count \n",
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
Simple_Test::create_EC (ACE_ENV_SINGLE_ARG_DECL)
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
Simple_Test::on_event_received (void)
{
  ++this->result_count_;

  ACE_DEBUG ((LM_DEBUG,
              "event count = #%d\n",
              this->result_count_.value ()));

  if (this->result_count_ == 2 * this->event_count_)
    {
      ACE_DECLARE_NEW_CORBA_ENV;
      this->end_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Simple_Test::run_test (ACE_ENV_SINGLE_ARG_DECL)
{
  CORBA::Any data;

  for (int i = 0; i < this->event_count_; ++i)
    {
      data <<= (CORBA::Long)i;

      this->supplier_->send_event (data
                                   ACE_ENV_ARG_PARAMETER);
      ACE_CHECK;
    }
}

void
Simple_Test::end_test (ACE_ENV_SINGLE_ARG_DECL)
{
  this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
Simple_Test::check_results (void)
{
  // Destroy the channel
  ACE_DECLARE_NEW_CORBA_ENV;
  this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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
main (int argc, char* argv[])
{
  Simple_Test events;

  if (events.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  ACE_TRY_NEW_ENV
    {
      events.init (argc,
                   argv
                   ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      events.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
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
