//$Id$

#include "Bank_Client_i.h"
#include "ace/Get_Opt.h"
#include "ace/Read_Buffer.h"
#include "ace/OS_NS_unistd.h"

// This is the interface program that accesses the remote object

// Constructor.
Bank_Client_i::Bank_Client_i (void)
{
  //no-op
}

//Destructor.
Bank_Client_i::~Bank_Client_i (void)
{
  //no-op
}

int
Bank_Client_i::run (const char *name,
                    int argc,
                    ACE_TCHAR *argv[])
{
  // Initialize the client.
  if (client_.init (name, argc, argv) == -1)
    return -1;

  try
    {
      this->check_accounts ();
      if (client_.do_shutdown () == 1)
        client_->shutdown ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nException caught in run\n")));
    }

  return 0;
}

int
Bank_Client_i::check_accounts (void)
{
  try
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nTests for account with same name\n")));
      this->test_for_same_name ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nTests for account with different names\n")));
      this->test_for_different_name ();

      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("\nTests for overdrafts\n")));
      this->test_for_overdraft ();
    }
  catch (const CORBA::Exception&)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("From Bank_Client_i::check_accounts()")));
    }
  return 0;
}

// This method tests whether an account with a
// a same name can be opened
void
Bank_Client_i::test_for_same_name (void)
{

  const char *name = "Name";
  CORBA::Float initial_bal = 0.00;

  Bank::Account_var acct_id1 = client_->open (name,
                                              initial_bal);

  Bank::Account_var acct_id2 = client_->open (name,
                                              initial_bal);

  ACE_ASSERT (acct_id1->_is_equivalent (acct_id2.in ()) != 0);

  client_->close (acct_id1.in ());
}

// This method tests whether an account with different names can be opened
void
Bank_Client_i::test_for_different_name (void)
{
  const char *name1 = "Name1";
  const char *name2 = "Name2";

  CORBA::Float initial_bal = 0.0;

  Bank::Account_var acct_id1 = client_->open (name1,
                                              initial_bal);

  Bank::Account_var acct_id2 = client_->open (name2,
                                              initial_bal);
  ACE_ASSERT (acct_id1->_is_equivalent (acct_id2.in ()) == 0);

  client_->close (acct_id1.in ());

  client_->close (acct_id2.in ());
}

// This method tests the Overdraft exception.
void
Bank_Client_i::test_for_overdraft (void)
{
  CORBA::Float initial_bal = 100.0;
  const char *name = "Name";
  Bank::Account_var acct_id = client_->open (name, initial_bal);
  acct_id->deposit (100.00);

  CORBA::Float bal = acct_id->balance ();

  try
    {
      acct_id->withdraw (bal + 20);
    }
  catch (Bank::Account::Overdraft &)
    {
      ACE_DEBUG ((LM_DEBUG,
                  ACE_TEXT ("Overdraft caught.\n")));
    }

  client_->close (acct_id.in ());
}
