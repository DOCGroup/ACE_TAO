//$Id$

#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "tao/debug.h"
#include "Updates.h"

ACE_RCSID (Notify_Tests, Updates, "$Id$")

Updates_StructuredPushConsumer::Updates_StructuredPushConsumer (Updates *test_client)
  : test_client_ (test_client)
{
}

void
Updates_StructuredPushConsumer::offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed
        ACE_ENV_ARG_DECL_NOT_USED
      )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "StructuredPushConsumer::offer_change invoked:\n"));
  this->test_client_->types_changed (added, removed);
}

/***************************************************************************/

Updates_StructuredPushSupplier::Updates_StructuredPushSupplier (Updates* test_client)
  :test_client_ (test_client)
{
}

Updates_StructuredPushSupplier::~Updates_StructuredPushSupplier ()
{
}

void
Updates_StructuredPushSupplier::subscription_change (
    const CosNotification::EventTypeSeq & added,
    const CosNotification::EventTypeSeq & removed
    ACE_ENV_ARG_DECL_NOT_USED
  )
  ACE_THROW_SPEC ((CORBA::SystemException,
                   CosNotifyComm::InvalidEventType))
{
  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "StructuredPushSupplier::subscription_change invoked:\n"));
  this->test_client_->types_changed (added, removed);
}

/***************************************************************************/

Updates::Updates (void)
  : added_count_ (0), removed_count_ (0)
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
                  Updates_StructuredPushConsumer (this),
                  -1);
  consumer_->init (root_poa_.in ()
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  consumer_->connect (this->consumer_admin_.in ()
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  ACE_NEW_RETURN (supplier_,
                 Updates_StructuredPushSupplier (this),
                 -1);
  supplier_->init (root_poa_.in ()
                   ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

  supplier_->connect (this->supplier_admin_.in ()
                      ACE_ENV_ARG_PARAMETER);
  ACE_CHECK_RETURN (-1);

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
Updates::types_changed (const CosNotification::EventTypeSeq & added,
                        const CosNotification::EventTypeSeq & removed)
{
  if (TAO_debug_level)
    {
      ACE_DEBUG ((LM_DEBUG, "Added Types.. \n"));
      this->print_event_types (added);

      ACE_DEBUG ((LM_DEBUG, "Removed Types.. \n"));
      this->print_event_types (removed);
    }

  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);

  this->added_count_ += added.length ();
  this->removed_count_ += removed.length ();
}

void
Updates::print_event_types (const CosNotification::EventTypeSeq &types)
{
  for (CORBA::ULong i = 0; i < types.length (); ++i)
    {
      ACE_DEBUG ((LM_DEBUG, "(%s, %s)\n", types[i].domain_name.in (), types[i].type_name. in()));
    }
}

void
Updates::add_type (CosNotification::EventTypeSeq& type_seq, const char* type)
{
  // Make space.
  int index = type_seq.length ();
  type_seq.length (index + 1);

  type_seq[index].domain_name =  CORBA::string_dup (type);
  type_seq[index].type_name = CORBA::string_dup (type);
}

void
Updates::wait_for_updates (int expected_added, int expected_removed)
{
  while (1)
    {
      if (added_count_ == expected_added &&
          removed_count_ == expected_removed)
        break;

      {
        if (this->orb_->work_pending ())
          this->orb_->perform_work ();
      }
    }
}

void
Updates::reset_counts (void)
{
  ACE_GUARD (TAO_SYNCH_MUTEX, mon, this->lock_);
  this->added_count_ = 0;
  this->removed_count_ = 0;
}

void
Updates::run_test (ACE_ENV_SINGLE_ARG_DECL)
{
  this->test_subscription_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  if (TAO_debug_level)
    ACE_DEBUG ((LM_DEBUG, "Finished testing subscription_change!\n"));

  this->test_offer_change (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  ACE_DEBUG ((LM_DEBUG, "Updates test has run successfully!\n"));
}

void
Updates::test_subscription_change (ACE_ENV_SINGLE_ARG_DECL)
{
  {
    // reset counts.
    this->reset_counts ();

    /// Currently we're subscribed for "*"
    /// Add RED, GREEN and BLUE
    /// Remove ORANGE and PINK

    CosNotification::EventTypeSeq added, removed;

    this->add_type (added, "RED");
    this->add_type (added, "GREEN");
    this->add_type (added, "BLUE");

    this->add_type (removed, "ORANGE");
    this->add_type (removed, "PINK");

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "Calling subscription_change with added types:\n"));
        this->print_event_types (added);
        ACE_DEBUG ((LM_DEBUG, "Calling subscription_change with removed types:\n"));
        this->print_event_types (removed);
      }

    this->consumer_->get_proxy_supplier ()->subscription_change (added,
                                                                 removed
                                                                 ACE_ENV_ARG_PARAMETER
                                                                 );
    ACE_CHECK;

    this->wait_for_updates (3, 0); // The supplier should receive Added (RED, GREEN, BLUE)

    // Get the subscriptions visible to the supplier.
    CosNotification::EventTypeSeq_var obtained =
      this->supplier_->get_proxy_consumer ()->obtain_subscription_types (CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "obtain_subscription_types \n"));
        this->print_event_types (obtained.in ());
      }
  }

  {
    // reset counts.
    this->reset_counts ();

    // Now, add PURPLE, RED and GREEN
    CosNotification::EventTypeSeq added, removed;

    this->add_type (added, "PURPLE");
    this->add_type (added, "GREEN");
    this->add_type (added, "BLUE");

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "Calling subscription_change with added types:\n"));
        this->print_event_types (added);
        ACE_DEBUG ((LM_DEBUG, "Calling subscription_change with removed types:\n"));
        this->print_event_types (removed);
      }

    this->consumer_->get_proxy_supplier ()->subscription_change (added,
                                                                 removed
                                                                 ACE_ENV_ARG_PARAMETER
                                                                 );
    ACE_CHECK;

    this->wait_for_updates (1, 0); // The supplier should receive Added (PURPLE).

    // Get the subscriptions visible to the supplier.
    CosNotification::EventTypeSeq_var obtained =
      this->supplier_->get_proxy_consumer ()->obtain_subscription_types (CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "obtain_subscription_types \n"));
        this->print_event_types (obtained.in ());
      }
  }

  {
    // reset counts.
    this->reset_counts ();

    // Next, Remove everything by subcribing to "*"
    CosNotification::EventTypeSeq added, removed;

    this->add_type (added, "*");

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "Calling subscription_change with added types:\n"));
        this->print_event_types (added);
        ACE_DEBUG ((LM_DEBUG, "Calling subscription_change with removed types:\n"));
        this->print_event_types (removed);
      }

    this->consumer_->get_proxy_supplier ()->subscription_change (added,
                                                                 removed
                                                                 ACE_ENV_ARG_PARAMETER
                                                                 );
    ACE_CHECK;

    this->wait_for_updates (0, 4);
    // The supplier should receive Remove {RED, GREEN} out of the 4 actally removed (RED, GREEN, BLUE, PURPLE) becaue that whats it offered for.

    // Get the subscriptions visible to the supplier.
    CosNotification::EventTypeSeq_var obtained =
      this->supplier_->get_proxy_consumer ()->obtain_subscription_types (CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "obtain_subscription_types \n"));
        this->print_event_types (obtained.in ());
      }
  }

}

void
Updates::test_offer_change (ACE_ENV_SINGLE_ARG_DECL)
{
  {
    // reset counts.
    this->reset_counts ();

    /// Currently we're subscribed for "*"
    /// Add RED, GREEN and BLUE
    /// Remove ORANGE and PINK

    CosNotification::EventTypeSeq added, removed;

    this->add_type (added, "RED");
    this->add_type (added, "GREEN");
    this->add_type (added, "BLUE");

    this->add_type (removed, "ORANGE");
    this->add_type (removed, "PINK");

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "Calling offer_change with added types:\n"));
        this->print_event_types (added);
        ACE_DEBUG ((LM_DEBUG, "Calling offer_change with removed types:\n"));
        this->print_event_types (removed);
      }

    this->supplier_->get_proxy_consumer ()->offer_change (added,
                                                          removed
                                                          ACE_ENV_ARG_PARAMETER
                                                          );
    ACE_CHECK;

    this->wait_for_updates (3, 0); // The consumer should receive Added (RED, GREEN, BLUE)

    // Get the offers visible to the supplier.
    CosNotification::EventTypeSeq_var obtained =
      this->consumer_->get_proxy_supplier ()->obtain_offered_types (CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "obtain_offer_types \n"));
        this->print_event_types (obtained.in ());
      }
  }

  {
    // reset counts.
    this->reset_counts ();

    // Now, add PURPLE, RED and GREEN
    CosNotification::EventTypeSeq added, removed;

    this->add_type (added, "PURPLE");
    this->add_type (added, "GREEN");
    this->add_type (added, "BLUE");

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "Calling offer_change with added types:\n"));
        this->print_event_types (added);
        ACE_DEBUG ((LM_DEBUG, "Calling offer_change with removed types:\n"));
        this->print_event_types (removed);
      }

    this->supplier_->get_proxy_consumer ()->offer_change (added,
                                                          removed
                                                          ACE_ENV_ARG_PARAMETER
                                                                 );
    ACE_CHECK;

    this->wait_for_updates (1, 0); // The consumer should receive Added (PURPLE).

    // Get the offers visible to the consumer.
    CosNotification::EventTypeSeq_var obtained =
      this->consumer_->get_proxy_supplier ()->obtain_offered_types (CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "obtain_offer_types \n"));
        this->print_event_types (obtained.in ());
      }
  }

  {
    // reset counts.
    this->reset_counts ();

    // Next, Remove everything by subcribing to "*"
    CosNotification::EventTypeSeq added, removed;

    this->add_type (added, "*");

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "Calling offer_change with added types:\n"));
        this->print_event_types (added);
        ACE_DEBUG ((LM_DEBUG, "Calling offer_change with removed types:\n"));
        this->print_event_types (removed);
      }

    this->supplier_->get_proxy_consumer ()->offer_change (added,
                                                          removed
                                                          ACE_ENV_ARG_PARAMETER
                                                          );
    ACE_CHECK;

    this->wait_for_updates (0, 4);
    // The consumer should receive Remove {RED, GREEN} out of the 4 actally removed (RED, GREEN, BLUE, PURPLE) becaue that whats it offered for.

    // Get the offers visible to the consumer.
    CosNotification::EventTypeSeq_var obtained =
      this->consumer_->get_proxy_supplier ()->obtain_offered_types (CosNotifyChannelAdmin::ALL_NOW_UPDATES_ON);

    if (TAO_debug_level)
      {
        ACE_DEBUG ((LM_DEBUG, "obtain_offer_types \n"));
        this->print_event_types (obtained.in ());
      }
  }

}

/***************************************************************************/

int
main (int argc, char* argv[])
{
  Updates updates;

  ACE_TRY_NEW_ENV
    {
      updates.init (argc,
                    argv
                    ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;

      updates.run_test (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
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

  return 0;
}

#if defined (ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION)


#elif defined (ACE_HAS_TEMPLATE_INSTANTIATION_PRAGMA)


#endif /*ACE_HAS_EXPLICIT_TEMPLATE_INSTANTIATION */
