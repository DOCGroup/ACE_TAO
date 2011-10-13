//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "ConnectDisconnect.h"



#define CD_IMPLEMENT_ENTITY(X)\
\
CD_##X::CD_##X (ConnectDisconnect* cd, int id) \
  :CD_Entity (cd, id) \
{ \
}

CD_Entity::CD_Entity (ConnectDisconnect* cd, int id)
  :id_ (id),
   cd_ (cd)
{
}

CD_Entity::~CD_Entity ()
{
  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG, "Entity #%d destroyed\n", this->id_));
    }

  cd_->on_entity_destroyed ();
}

CD_IMPLEMENT_ENTITY(PushConsumer)
CD_IMPLEMENT_ENTITY(StructuredPushConsumer)
CD_IMPLEMENT_ENTITY(SequencePushConsumer)

CD_IMPLEMENT_ENTITY(PushSupplier)
CD_IMPLEMENT_ENTITY(StructuredPushSupplier)
CD_IMPLEMENT_ENTITY(SequencePushSupplier)

//*****************************************************************************************************

ConnectDisconnect::ConnectDisconnect (void)
  :any_consumer_ (0),
   structured_consumer_ (0),
   sequence_consumer_ (0),
   any_supplier_ (0),
   structured_supplier_ (0),
   sequence_supplier_ (0),
   count_ (3),
   consumers_ (3),
   suppliers_ (3)
{
}

ConnectDisconnect::~ConnectDisconnect ()
{
  delete [] this->any_consumer_;
  delete [] this->structured_consumer_;
  delete [] this->sequence_consumer_;

  delete [] this->any_supplier_;
  delete [] this->structured_supplier_;
  delete [] this->sequence_supplier_;
}

void
ConnectDisconnect::on_entity_destroyed (void)
{
  this->result_count_++;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG,
                "result_count = %d\n",
                this->result_count_.value ()));
}

int
ConnectDisconnect::init (int argc,
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

  // How many are we counting..?
  this->expected_count_ = count_ * (consumers_ * 3 + suppliers_ * 3);

  // Create the consumers and suppliers.

  // Arrays of Consumers.
  ACE_NEW_RETURN (this->any_consumer_,
                  TAO_Notify_Tests_PushConsumer*[this->consumers_],
                  -1);
  ACE_NEW_RETURN (this->structured_consumer_,
                 TAO_Notify_Tests_StructuredPushConsumer*[this->consumers_],
                 -1);
  ACE_NEW_RETURN (this->sequence_consumer_,
                  TAO_Notify_Tests_SequencePushConsumer*[this->consumers_],
                  -1);

  // Arrays of Suppliers.
  ACE_NEW_RETURN (this->any_supplier_,
                  TAO_Notify_Tests_PushSupplier*[this->suppliers_],
                  -1);
  ACE_NEW_RETURN (this->structured_supplier_,
                  TAO_Notify_Tests_StructuredPushSupplier*[this->suppliers_],
                  -1);
  ACE_NEW_RETURN (this->sequence_supplier_,
                  TAO_Notify_Tests_SequencePushSupplier*[this->suppliers_],
                  -1);

  consumer_start( 0 );
  return 0;
}

int
ConnectDisconnect::parse_args(int argc, ACE_TCHAR *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc,
                              argv);

    const ACE_TCHAR *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-count"))))
        {
          this->count_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-consumers"))))
        {
          this->consumers_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (0 != (current_arg = arg_shifter.get_the_parameter (ACE_TEXT("-suppliers"))))
        {
          this->suppliers_ = ACE_OS::atoi (current_arg);
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp (ACE_TEXT("-?")) == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-count testcount\n"
                     "-consumers number_of_consumers"
                     "-suppliers number_of_suppliers",
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
ConnectDisconnect::create_EC (void)
{
  CosNotifyChannelAdmin::ChannelID id;

  this->ec_ = notify_factory_->create_channel (this->initial_qos_,
                                               this->initial_admin_,
                                               id);

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
ConnectDisconnect::run_test (void)
{
  for (int iterations = 0; iterations < count_; ++iterations)
    {
      int i = 0;

      for (i = 0; i < this->consumers_; ++i)
        {
          // Create and connect Any consumers.
          ACE_NEW (this->any_consumer_[i],
                   CD_PushConsumer (this,
                                    i));
          this->any_consumer_[i]->init (root_poa_.in ());
          this->any_consumer_[i]->connect (this->consumer_admin_.in ());

          // Create and connect Structured consumers.
          ACE_NEW (this->structured_consumer_[i],
                   CD_StructuredPushConsumer (this,
                                              i));
          this->structured_consumer_[i]->init (root_poa_.in ());
          this->structured_consumer_[i]->connect (this->consumer_admin_.in ());

          // Create and connect Sequence consumers.
          ACE_NEW (this->sequence_consumer_[i],
                   CD_SequencePushConsumer (this,
                                            i));
          this->sequence_consumer_[i]->init (root_poa_.in ());
          this->sequence_consumer_[i]->connect (this->consumer_admin_.in ());
        }

      for (i = 0; i < this->suppliers_; ++i)
        {
          ACE_NEW (this->any_supplier_[i],
                   CD_PushSupplier (this,
                                    i));
          this->any_supplier_[i]->init (root_poa_.in ());
          this->any_supplier_[i]->connect (this->supplier_admin_.in ());

          // Create and connect Structured suppliers.
          ACE_NEW (this->structured_supplier_[i],
                   CD_StructuredPushSupplier (this,
                                              i));
          this->structured_supplier_[i]->init (root_poa_.in ());
          this->structured_supplier_[i]->connect (this->supplier_admin_.in ());

          // Create and connect Sequence suppliers.
          ACE_NEW (this->sequence_supplier_[i],
                   CD_SequencePushSupplier (this,
                                            i));
          this->sequence_supplier_[i]->init (root_poa_.in ());
          this->sequence_supplier_[i]->connect (this->supplier_admin_.in ());
        }

      for (i = 0; i < this->consumers_; ++i)
        {
          // Disconnnect Any consumers.
          this->any_consumer_[i]->disconnect ();

          // Disconnect Structured Consumers.
          this->structured_consumer_[i]->disconnect (
                                           );

          // Disconnect Sequence Consumers.
          this->sequence_consumer_[i]->disconnect (
                                     );
        }

      for (i = 0; i < this->suppliers_; ++i)
        {
          // Disconnnect Any suppliers.
          this->any_supplier_[i]->disconnect ();

          // Disconnect Structured Suppliers.
          this->structured_supplier_[i]->disconnect (
                                           );

          // Disconnect Sequence Suppliers.
          this->sequence_supplier_[i]->disconnect (
                                         );
        }
    }
}

void
ConnectDisconnect::end_test (void)
{
  consumer_done( 0 );
}

int
ConnectDisconnect::check_results (void)
{
  ACE_DEBUG ((LM_DEBUG, "result_count_ = %d", this->result_count_.value ()));
  ACE_DEBUG ((LM_DEBUG, " expected_count_ = %d\n", this->expected_count_));

  if (this->result_count_ != this->expected_count_)
    {
      ACE_DEBUG ((LM_DEBUG, "ConnectDisconnect test failed\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG, "ConnectDisconnect test succeeded\n"));
    }

  // Destroy the channel.
  this->ec_->destroy ();

  return 0;
}

/***************************************************************************/

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  ConnectDisconnect client;

  if (client.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  try
    {
      client.init (argc, argv);

      client.run_test ();

      client.end_test ();
    }
  catch (const CORBA::Exception& se)
    {
      se._tao_print_exception ("Error: ");
      return 1;
    }

  int status = 0;

  try
    {
      status = client.check_results ();
    }
  catch (const CORBA::Exception& se)
    {
      se._tao_print_exception ("Error: ");
      status = 1;
    }

  return status;
}

