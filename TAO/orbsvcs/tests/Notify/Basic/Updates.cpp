//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "Updates.h"

ACE_RCSID (Notify_Tests, Updates, "$Id$")

Update_StructuredPushConsumer::Update_StructuredPushConsumer (
    Updates *test_client
  )
  : test_client_ (test_client)
{
}

void
Update_StructuredPushConsumer::offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL
      )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  if (added.length () > 0)
    {
      test_client_->offers_added_ = added.length ();
      this->test_client_->end_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (removed.length () > 0)
    {
      test_client_->offers_removed_ = removed.length ();
      this->test_client_->end_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

/***************************************************************************/

Update_StructuredPushSupplier::Update_StructuredPushSupplier (
    Updates* test_client
  )
  :test_client_ (test_client)
{
}

Update_StructuredPushSupplier::~Update_StructuredPushSupplier ()
{
}

void
Update_StructuredPushSupplier::subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyComm::InvalidEventType))
{
  if (added.length () > 0)
    {
      test_client_->subscriptions_added_ = added.length ();
      this->test_client_->end_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }

  if (removed.length () > 0)
    {
      test_client_->subscriptions_removed_ = removed.length ();
      this->test_client_->end_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

/***************************************************************************/
Updates::Updates (void)
  : domain_name_ ("Test_Domain"),
    type_name_ ("Test_Type"),
    update_count_ (5)
{
}

Updates::~Updates ()
{
}

int
Updates::init (int argc,
               char* argv []
               ACE_ENV_ARG_DECL)
{
  // Initialize base class.
  Notify_Test_Client::init (argc,
                            argv
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  // Create all participents.
  this->create_EC (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_,
                            adminid
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_,
                            adminid
                            ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  ACE_NEW_RETURN (consumer_,
                  Update_StructuredPushConsumer (this),
                  -1);
  consumer_->init (root_poa_.in ()
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  consumer_->connect (this->consumer_admin_.in ()
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_NEW_RETURN (supplier_,
                 Update_StructuredPushSupplier (this),
                 -1);
  supplier_->init (root_poa_.in ()
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  supplier_->connect (this->supplier_admin_.in ()
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  return 0;
}

int
Updates::parse_args (int argc,
                     char *argv[])
{
    ACE_Arg_Shifter arg_shifter (argc,
                                 argv);

    const char *current_arg = 0;

    while (arg_shifter.is_anything_left ())
    {
      if ((current_arg = arg_shifter.get_the_parameter ("-updates")))
        {
          this->update_count_ = ACE_OS::atoi (current_arg);
          // The number of updates required.
          arg_shifter.consume_arg ();
        }
      else if (arg_shifter.cur_arg_strncasecmp ("-?") == 0)
        {
          ACE_DEBUG ((LM_DEBUG,
                      "usage: %s "
                      "-updates update_count \n",
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
Updates::create_EC (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotifyChannelAdmin::ChannelID id;

  ec_ = notify_factory_->create_channel (initial_qos_,
                                         initial_admin_,
                                         id
                                         ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (ec_.in ()));
}

void
Updates::run_test (ACE_ENV_SINGLE_ARG_DECL)
{
  CosNotification::EventTypeSeq added (update_count_), removed (update_count_);
  added.length (update_count_);
  removed.length (0);

  char update_test_buf[BUFSIZ];

  int i = 0;

  for (; i < update_count_; ++i)
    {
      ACE_OS::sprintf (update_test_buf, "%s_%d", this->domain_name_, i);
      added[i].domain_name =  CORBA::string_dup (update_test_buf);

      ACE_OS::sprintf (update_test_buf, "%s_%d", this->type_name_, i);
      added[i].type_name = CORBA::string_dup (update_test_buf);
    }

  // Test added.
  this->supplier_->get_proxy_consumer ()->offer_change (
                                              added,
                                              removed
                                              ACE_ENV_ARG_PARAMETER
                                            );
  ACE_CHECK;

  this->consumer_->get_proxy_supplier ()->subscription_change (
                                              added,
                                              removed
                                              ACE_ENV_ARG_PARAMETER
                                            );
  ACE_CHECK;

  // Test removed.

  added.length (0);
  removed.length (update_count_);

  for (i = 0; i < update_count_; ++i)
    {
      ACE_OS::sprintf (update_test_buf,
                       "%s_%d",
                       this->domain_name_,
                       i);
      removed[i].domain_name =  CORBA::string_dup (update_test_buf);

      ACE_OS::sprintf (update_test_buf,
                       "%s_%d",
                       this->type_name_,
                       i);
      removed[i].type_name = CORBA::string_dup (update_test_buf);
    }

  this->supplier_->get_proxy_consumer ()->offer_change (
                                              added,
                                              removed
                                              ACE_ENV_ARG_PARAMETER
                                            );
  ACE_CHECK;

  this->consumer_->get_proxy_supplier ()->subscription_change (
                                              added,
                                              removed
                                              ACE_ENV_ARG_PARAMETER
                                            );
  ACE_CHECK;
}

void
Updates::end_test (ACE_ENV_SINGLE_ARG_DECL)
{
  if (++this->result_count_ == 4)
    {
      this->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_CHECK;
    }
}

int
Updates::check_results (void)
{
  // Destroy the channel.
  ACE_DECLARE_NEW_CORBA_ENV;
  this->ec_->destroy (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_DEBUG ((LM_DEBUG,
              "offers_added_ = %d, offers_removed_ = %d, "
              "subscriptions_added_= %d, subscriptions_removed_ = %d\n",
              offers_added_,
              offers_removed_,
              subscriptions_added_,
              subscriptions_removed_));

  if (offers_added_ == update_count_ &&
      offers_removed_ == update_count_ &&
      subscriptions_added_ == update_count_ &&
      subscriptions_removed_ == update_count_)
    {
      return 0;
    }
  else
    {
      ACE_DEBUG ((LM_DEBUG,
                  "Updates test failed!\n"));
      return 1;
    }
}

/***************************************************************************/

int
main (int argc, char* argv[])
{
  Updates updates;

  if (updates.parse_args (argc, argv) == -1)
    {
      return 1;
    }

  ACE_TRY_NEW_ENV
    {
      updates.init (argc,
                    argv
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      updates.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      updates.ORB_run ();
    }
  ACE_CATCH (CORBA::UserException, ue)
    {
      ACE_PRINT_EXCEPTION (ue,
                           "Updates user error: ");
      return 1;
    }
  ACE_CATCH (CORBA::SystemException, se)
    {
      ACE_PRINT_EXCEPTION (se,
                           "Updates system error: ");
      return 1;
    }
  ACE_ENDTRY;

  return updates.check_results ();
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
