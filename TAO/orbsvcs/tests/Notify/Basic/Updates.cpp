//$Id$

#include "Updates.h"

ACE_RCSID (Notify_Tests, Notify_Test_Updates_Client, "$Id$")

#define NOTIFY_UPDATES_TEST_DOMAIN "Test_Domain"
#define NOTIFY_UPDATES_TEST_TYPE "Test_Type"

  int BUFFER_TEST_COUNT = 5;

int offers_added_, offers_removed_, subscriptions_added_, subscriptions_removed_;
// Check if these are equal to <BUFFER_TEST_COUNT> at the end of the test.

Update_StructuredPushConsumer::~Update_StructuredPushConsumer ()
{
}

void
Update_StructuredPushConsumer::offer_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &/*ACE_TRY_ENV*/
      )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  if (added.length () > 0)
    ::offers_added_ = added.length ();

  if (removed.length () > 0)
    ::offers_removed_ = removed.length ();
}

/***************************************************************************/

Update_StructuredPushSupplier::Update_StructuredPushSupplier (Notify_Test_Updates_Client* test_client)
  :test_client_ (test_client)
{
}

Update_StructuredPushSupplier::~Update_StructuredPushSupplier ()
{
}

void Update_StructuredPushSupplier::subscription_change (
        const CosNotification::EventTypeSeq & added,
        const CosNotification::EventTypeSeq & removed,
        CORBA::Environment &/*ACE_TRY_ENV*/
        )
  ACE_THROW_SPEC ((
                   CORBA::SystemException,
                   CosNotifyComm::InvalidEventType
                   ))
{
  if (added.length () > 0)
    ::subscriptions_added_ = added.length ();

  if (removed.length () > 0)
    ::subscriptions_removed_ = removed.length ();

  //  this->test_client_->shutdown (ACE_TRY_ENV);
}

/***************************************************************************/
Notify_Test_Updates_Client::Notify_Test_Updates_Client (void)
{
  count_ = BUFFER_TEST_COUNT;
}

Notify_Test_Updates_Client::~Notify_Test_Updates_Client ()
{
}

void
Notify_Test_Updates_Client::init_concrete (int /*argc*/, char */*argv*/ [], CORBA::Environment &ACE_TRY_ENV)
{
  // Create all participents ...
  CosNotifyChannelAdmin::AdminID adminid;

  supplier_admin_ =
    ec_->new_for_suppliers (this->ifgop_, adminid, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (supplier_admin_.in ()));

  consumer_admin_ =
    ec_->new_for_consumers (this->ifgop_, adminid, ACE_TRY_ENV);
  ACE_CHECK;

  ACE_ASSERT (!CORBA::is_nil (consumer_admin_.in ()));

  consumer_ = new Update_StructuredPushConsumer ();
  consumer_->init (root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  consumer_->connect (this->consumer_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  supplier_ = new Update_StructuredPushSupplier (this);
  supplier_->init (root_poa_.in (), ACE_TRY_ENV);
  ACE_CHECK;

  supplier_->connect (this->supplier_admin_.in (), ACE_TRY_ENV);
  ACE_CHECK;
}

void
Notify_Test_Updates_Client::run_test (CORBA::Environment &ACE_TRY_ENV)
{
  int update_count = this->count_;

  CosNotification::EventTypeSeq added (update_count), removed (update_count);
  added.length (update_count);
  removed.length (0);

  char update_test_buf[BUFSIZ];

  int i = 0;
  for (; i < update_count; ++i)
    {
      ACE_OS::sprintf (update_test_buf, "%s_%d", NOTIFY_UPDATES_TEST_DOMAIN, i);
      added[i].domain_name =  CORBA::string_dup (update_test_buf);

      ACE_OS::sprintf (update_test_buf, "%s_%d", NOTIFY_UPDATES_TEST_TYPE, i);
      added[i].type_name = CORBA::string_dup (update_test_buf);
    }

  // test added
  this->supplier_->get_proxy_consumer ()->offer_change (added, removed, ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_->get_proxy_supplier ()->subscription_change (added, removed,
                                                               ACE_TRY_ENV);
  ACE_CHECK;

  // test removed

  added.length (0);
  removed.length (update_count);

  for (i = 0; i < update_count; ++i)
    {
      ACE_OS::sprintf (update_test_buf, "%s_%d", NOTIFY_UPDATES_TEST_DOMAIN, i);
      removed[i].domain_name =  CORBA::string_dup (update_test_buf);

      ACE_OS::sprintf (update_test_buf, "%s_%d", NOTIFY_UPDATES_TEST_TYPE, i);
      removed[i].type_name = CORBA::string_dup (update_test_buf);
    }

  this->supplier_->get_proxy_consumer ()->offer_change (added, removed, ACE_TRY_ENV);
  ACE_CHECK;

  this->consumer_->get_proxy_supplier ()->subscription_change (added, removed,
                                                               ACE_TRY_ENV);
  ACE_CHECK;
}

/***************************************************************************/
int
main (int argc, char* argv[])
{
  Notify_Test_Updates_Client client;

  int result = Notify_Test_Client::main (argc, argv, client);

  ACE_DEBUG((LM_DEBUG, "offers_added_ = %d, offers_removed_ = %d, subscriptions_added_= %d, subscriptions_removed_ = %d\n",
             offers_added_, offers_removed_, subscriptions_added_, subscriptions_removed_));

  if (result == 0)
    {
      if (offers_added_ == BUFFER_TEST_COUNT &&
          offers_removed_ == BUFFER_TEST_COUNT &&
          subscriptions_added_ == BUFFER_TEST_COUNT &&
          subscriptions_removed_ == BUFFER_TEST_COUNT)
        return 0;
    }

  ACE_DEBUG ((LM_DEBUG, "Updates test failed!\n"));
  return 1;
}
