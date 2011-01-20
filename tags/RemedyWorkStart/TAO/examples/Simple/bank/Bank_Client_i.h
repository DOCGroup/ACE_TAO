// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple/bank
//
// = FILENAME
//    Bank_Client_i.h
//
// = DESCRIPTION
//    This class implements the  interface calls for RMI.
//
// = AUTHOR
//
//   Balachandran Natarajan <bala@cs.wustl.edu>
//
// ============================================================================

#ifndef BANK_CLIENT_I_H
#define BANK_CLIENT_I_H

#include "../Simple_util.h"
#include "BankC.h"

class Bank_Client_i
{
  // = TITLE
  //   Grid_Client interface subclass.
  //
  // = DESCRIPTION
  //   This class implements the interface between the interface
  //   objects and the client .
public:
  // = Initialization and termination methods.
  Bank_Client_i (void);
  // Constructor

  ~Bank_Client_i (void);
  // Destructor

  int run (const char *, int, ACE_TCHAR *[]);
  // Execute the methods.

private:
  int check_accounts (void);
  // Method that calls all the test functions

  void test_for_different_name (void);
  // Tests if accounts opened with different names return a different account
  // reference.

  void test_for_same_name (void);
  // Tests if accounts opened with the same name return the same
  // object reference.

  void test_for_overdraft (void);
  // Tests for the Overdraft Exception when the client tries to
  // withdraw more money than the current balance.

  Client<Bank::AccountManager> client_;
  // Instantiate the client object.
};

#endif /* TIME_CLIENT_I_H */
