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
                    char *argv[])
{
  // Initialize the client.
  if (client.init (name,argc, argv) == -1)
    return -1;

  ACE_TRY_NEW_ENV
    {
      this->check_accounts ();
      if (client.shutdown () == 1)
        client->shutdown (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nException caught in run\n"));
    }
  ACE_ENDTRY;

  return 0;
}

int
Bank_Client_i::check_accounts (void )
{
  ACE_TRY_NEW_ENV
    {
      ACE_DEBUG ((LM_DEBUG,
                  "\nTests for account with same name\n"));
      this->test_for_same_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\nTests for account with different names\n"));
      this->test_for_different_name (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;

      ACE_DEBUG ((LM_DEBUG,
                  "\nTests for overdrafts\n"));
      this->test_for_overdraft (ACE_ENV_SINGLE_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_DEBUG ((LM_DEBUG,
                  "From Bank_Client_i::check_accounts()"));
    }
  ACE_ENDTRY;
  return 0;
}

// This method tests whether an account with a
// a same name can be opened

void
Bank_Client_i::test_for_same_name (ACE_ENV_SINGLE_ARG_DECL)
{

  const char *name = "Name";
  CORBA::Float initial_bal = 0.00;

  Bank::Account_var acct_id1 = client->open (name,
                                             initial_bal
                                             ACE_ENV_ARG_PARAMETER);

  Bank::Account_var acct_id2 = client->open (name,
                                             initial_bal
                                             ACE_ENV_ARG_PARAMETER);

  ACE_ASSERT (acct_id1->_is_equivalent ((CORBA::Object *) acct_id2.in ()) != 0);

  client->close (acct_id1.in ()
                 ACE_ENV_ARG_PARAMETER);

  client->close (acct_id2.in ()
                 ACE_ENV_ARG_PARAMETER);
}

// This method tests whether an account with different names can be opened

void
Bank_Client_i::test_for_different_name (ACE_ENV_SINGLE_ARG_DECL)
{
  const char *name1 = "Name1";
  const char *name2 = "Name2";

  CORBA::Float initial_bal = 0.0;

  Bank::Account_var acct_id1 = client->open (name1,
                                             initial_bal
                                             ACE_ENV_ARG_PARAMETER);

  Bank::Account_var acct_id2 = client->open (name2,
                                             initial_bal
                                             ACE_ENV_ARG_PARAMETER);
  ACE_ASSERT (acct_id1->_is_equivalent ((CORBA::Object *)acct_id2.in ()) == 0);

  client->close (acct_id1.in ()
                 ACE_ENV_ARG_PARAMETER);

  client->close (acct_id2.in ()
                 ACE_ENV_ARG_PARAMETER);
}

// This method tests the Overdraft exception.

void
Bank_Client_i::test_for_overdraft (ACE_ENV_SINGLE_ARG_DECL)
{

  CORBA::Float initial_bal = 100.0;
  const char *name = "Name";
  Bank::Account_var acct_id = client->open (name, initial_bal ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
  acct_id->deposit (100.00 ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;

  CORBA::Float bal = acct_id->balance (ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  acct_id->withdraw (bal + 20);

  client->close (acct_id.in () ACE_ENV_ARG_PARAMETER);
  ACE_CHECK;
}

