// $Id$

#include "AdminProperties_Test.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "Notify_StructuredPushConsumer.h"
#include "Notify_StructuredPushSupplier.h"

AdminProperties_Test::AdminProperties_Test (void)
  : max_queue_length_ (10),
    max_consumers_ (5),
    max_suppliers_ (5),
    reject_new_events_ (0),
    consumers_ (10),
    suppliers_ (10),
    event_count_ (30)
{
}

AdminProperties_Test::~AdminProperties_Test (void)
{
}

int
AdminProperties_Test::parse_args(int argc, char *argv[])
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
                     "-event_count [event_count]\n",
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
AdminProperties_Test::init (int argc,
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
AdminProperties_Test::run_test (ACE_ENV_SINGLE_ARG_DECL)
{
  this->create_suppliers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->create_consumers (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  this->send_events (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;
}

void
AdminProperties_Test::create_suppliers (ACE_ENV_SINGLE_ARG_DECL)
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
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in connecting supplier\n");
    }
  ACE_ENDTRY;
}

void
AdminProperties_Test::create_consumers (ACE_ENV_SINGLE_ARG_DECL)
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
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception in connecting consumer \n");
    }
  ACE_ENDTRY;
}

void
AdminProperties_Test::send_events (ACE_ENV_SINGLE_ARG_DECL)
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

  TAO_Notify_StructuredPushSupplier *supplier = 0;
  ACE_NEW (supplier,
           TAO_Notify_StructuredPushSupplier ());

  supplier->init (root_poa_.in ()
                  ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  supplier->connect (this->supplier_admin_.in ()
                     ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  int i = 0;
  ACE_TRY
    {
      for (i = 0 ; i < this->event_count_; ++i)
        {
          event.filterable_data[0].value <<= (CORBA::Long)i;

          // Any.
          event.remainder_of_body <<= (CORBA::Long)i;

          supplier->send_event (event ACE_ENV_ARG_PARAMETER);
          ACE_TRY_CHECK;
        }
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Exception sending event\n");
    }
  ACE_ENDTRY;
}

/***************************************************************************/

int
main (int argc, char* argv[])
{
  AdminProperties_Test test;

  if (test.parse_args (argc, argv) == -1)
    {
      return 1;
    }

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
