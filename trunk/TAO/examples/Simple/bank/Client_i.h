// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple
//
// = FILENAME
//    Client_i.h
//
// = DESCRIPTION
//    This class implements a simple CORBA client that accesses a Bank
//    server.
//
// = AUTHORS
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#include "BankC.h"
#include "orbsvcs/Naming/Naming_Utils.h"

class Client_i
{
  // = TITLE
  //     Simple Client implementation.
  //
  // = DESCRIPTION
  //     Class wrapper for a client that gets the server IOR and then
  //     makes several calls to the server before optionally shutting
  //     it down.
public:
  // = Initialization and termination methods.
  Client_i (void);
  // Constructor.

  ~Client_i (void);
  // Destructor.

  int run (void);
  // Execute client example code.

  int init (int argc, char *argv[]);
  // Initialize the client communication endpoint with server.

private:
  int read_ior (char *filename);
  // Function to read the server IOR from a file.

  int parse_args (void);
  // Parses the arguments passed on the command line.

  CORBA::Float check_accounts (void);
  // Checks the various operations of the account.

  void deposit (Bank::Account_ptr server,
                CORBA::Float deposit_amount,
                CORBA::Environment &env);
  // Deposit in the account.

  void withdraw (Bank::Account_ptr server,
                 CORBA::Float withdrawl_amount);
  // Withdraw from the account.

  Bank::Account_ptr open (const char *name,
                          CORBA::Float initial_balance,
                          CORBA::Environment &env);
  // Open a new account with the given name and initial balance.


  void close (Bank::Account_ptr account,
              CORBA::Environment &env);
  // Close a given Account.

  void test_for_different_name (CORBA::Environment &env);
  // Tests if accounts opened with different names return a different account reference.

  void test_for_same_name (CORBA::Environment &env);
  // Tests if accounts opened with the same name return the same object reference.

  void test_for_overdraft (CORBA::Environment &env);
  // Tests for the Overdraft Exception when the client tries to
  // withdraw more money than the current balance.

  int obtain_initial_references (void);
  // To initialize the naming service and get a ptr to server.

  int argc_;
  // # of arguments on the command line.

  char **argv_;
  // arguments from command line.

  char *ior_;
  // IOR of the obj ref of the server.

  u_int loop_count_;
  // Number of times to invoke the <time> operation.

  int shutdown_;
  // Flag for server shutdown.

  CORBA::Environment env_;
  // Environment variable.

  Bank::Account_var server1_;
  // Account Server object ptr #1.

  Bank::Account_var server2_;
  // Account Serverobject ptr #2.

  Bank::AccountManager_var accountmanager_server_;
  // Account Manager server object ptr.

  TAO_Naming_Client my_name_client_;
  // An instance of the name client used for resolving the factory
  // objects.

  CORBA::ORB_var orb_;
  // Remember our orb.

  CORBA::Float initial_balance_;
  // Filled up by a command line argument.

  char *account_holder_name1_;
  // Name of the first Account Holder.

  char *account_holder_name2_;
  // Name of the second Account Holder.

};
