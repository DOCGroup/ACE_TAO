//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ConnectDisconnect.h"

ACE_RCSID (Notify_Tests, ConnectDisconnect, "$Id$")

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
  ACE_DEBUG ((LM_DEBUG, "Entity #%d destroyed\n", this->id_));
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
   // sequence_consumer_ (0),
   any_supplier_ (0),
   structured_supplier_ (0),
   // sequence_supplier_ (0),
   count_ (10),
   consumers_ (10),
   suppliers_ (10)
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

  ACE_DEBUG ((LM_DEBUG,
              "result_count = %d\n",
              this->result_count_.value ()));
}

int
ConnectDisconnect::init (int argc,
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

  // How many are we counting..?
  this->expected_count_ = count_ * (consumers_ * 3 + suppliers_ * 3);

  // this->expected_count_ = 10*(10*2 + 0);

  // Create the consumers and suppliers.

  // Arrays of Consumers.
  ACE_NEW_RETURN (this->any_consumer_,
                  TAO_Notify_PushConsumer*[this->consumers_],
                  -1);
  ACE_NEW_RETURN (this->structured_consumer_,
                 TAO_Notify_StructuredPushConsumer*[this->consumers_],
                 -1);
  ACE_NEW_RETURN (this->sequence_consumer_,
                  TAO_Notify_SequencePushConsumer*[this->consumers_],
                  -1);

  // Arrays of Suppliers.
  ACE_NEW_RETURN (this->any_supplier_,
                  TAO_Notify_PushSupplier*[this->suppliers_],
                  -1);
  ACE_NEW_RETURN (this->structured_supplier_,
                  TAO_Notify_StructuredPushSupplier*[this->suppliers_],
                  -1);
  ACE_NEW_RETURN (this->sequence_supplier_,
                  TAO_Notify_SequencePushSupplier*[this->suppliers_],
                  -1);

  return 0;
}

int
ConnectDisconnect::parse_args(int argc, char *argv[])
{
  ACE_Arg_Shifter arg_shifter (argc,
                              argv);

    const char *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-count")))
        {
          this->count_ = ACE_OS::atoi (current_arg);
          // The number of counsumers to create.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-consumers")))
        {
          this->consumers_ = ACE_OS::atoi (current_arg);
          // The number of suppliers to create.
          arg_shifter.consume_arg ();
        }
      else if ((current_arg = arg_shifter.get_the_parameter ("-suppliers")))
        {
          this->suppliers_ = ACE_OS::atoi (current_arg);
          // The number of iterations to connect disconnect.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG((LM_DEBUG,
                     "usage: %s "
                     "-count testcount \n"
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
ConnectDisconnect::create_EC (ACE_ENV_SINGLE_ARG_DECL)
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
ConnectDisconnect::run_test (ACE_ENV_SINGLE_ARG_DECL)
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
          this->any_consumer_[i]->init (root_poa_.in ()
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          this->any_consumer_[i]->connect (this->consumer_admin_.in ()
                                           ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Create and connect Structured consumers.
          ACE_NEW (this->structured_consumer_[i],
                   CD_StructuredPushConsumer (this,
                                              i));
          this->structured_consumer_[i]->init (root_poa_.in ()
                                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          this->structured_consumer_[i]->connect (this->consumer_admin_.in ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Create and connect Sequence consumers.
          ACE_NEW (this->sequence_consumer_[i],
                   CD_SequencePushConsumer (this,
                                            i));
          this->sequence_consumer_[i]->init (root_poa_.in ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          this->sequence_consumer_[i]->connect (this->consumer_admin_.in ()
                                                ACE_ENV_ARG_PARAMETER);
          sequence_consumer_[i]->connect (this->consumer_admin_.in () ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      for (i = 0; i < this->suppliers_; ++i)
        {
          ACE_NEW (this->any_supplier_[i],
                   CD_PushSupplier (this,
                                    i));
          this->any_supplier_[i]->init (root_poa_.in ()
                                        ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          this->any_supplier_[i]->connect (this->supplier_admin_.in ()
                                           ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Create and connect Structured suppliers.
          ACE_NEW (this->structured_supplier_[i],
                   CD_StructuredPushSupplier (this,
                                              i));
          this->structured_supplier_[i]->init (root_poa_.in ()
                                               ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          this->structured_supplier_[i]->connect (this->supplier_admin_.in ()
                                                  ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;

          // Create and connect Sequence suppliers.
          ACE_NEW (this->sequence_supplier_[i],
                   CD_SequencePushSupplier (this,
                                            i));
          this->sequence_supplier_[i]->init (root_poa_.in ()
                                             ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
          this->sequence_supplier_[i]->connect (this->supplier_admin_.in ()
                                                ACE_ENV_ARG_PARAMETER);
          ACE_CHECK;
        }

      for (i = 0; i < this->consumers_; ++i)
        {
          // Disconnnect Any consumers.
          this->any_consumer_[i]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          // Disconnect Structured Consumers.
          this->structured_consumer_[i]->disconnect (
                                             ACE_ENV_SINGLE_ARG_PARAMETER
                                           );
          ACE_CHECK;

          // Disconnect Sequence Consumers.
          this->sequence_consumer_[i]->disconnect (
                                           ACE_ENV_SINGLE_ARG_PARAMETER
                                         );
          ACE_CHECK;
        }

      for (i = 0; i < this->suppliers_; ++i)
        {
          // Disconnnect Any suppliers.
          this->any_supplier_[i]->disconnect (ACE_ENV_SINGLE_ARG_PARAMETER);
          ACE_CHECK;

          // Disconnect Structured Suppliers.
          this->structured_supplier_[i]->disconnect (
                                             ACE_ENV_SINGLE_ARG_PARAMETER
                                           );
          ACE_CHECK;

          // Disconnect Sequence Suppliers.
          this->sequence_supplier_[i]->disconnect (
                                           ACE_ENV_SINGLE_ARG_PARAMETER
                                         );
          ACE_CHECK;
        }
    }
}

void
ConnectDisconnect::end_test (ACE_ENV_SINGLE_ARG_DECL)
{
  this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
}

int
ConnectDisconnect::check_results (void)
{
  ACE_DEBUG ((LM_DEBUG,
              "result_count_ = %d",
              this->result_count_.value ()));
  ACE_DEBUG ((LM_DEBUG,
              " expected_count_ = %d\n",
              this->expected_count_));

  if (this->result_count_ != this->expected_count_)
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ConnectDisconnect test failed\n"));
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "ConnectDisconnect test succeded\n"));
    }

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
  ConnectDisconnect client;

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
                           "ConnectDisconnect user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "ConnectDisconnect system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return client.check_results ();
}


#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
