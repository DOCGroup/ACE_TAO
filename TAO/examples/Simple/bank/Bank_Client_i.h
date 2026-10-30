// -*- C++ -*-

//=============================================================================
/**
 *  @file    Bank_Client_i.h
 *
 *  This class implements the  interface calls for RMI.
 *
 *  @author Balachandran Natarajan <bala@cs.wustl.edu>
 */
//=============================================================================


#ifndef BANK_CLIENT_I_H
#define BANK_CLIENT_I_H

#include "../Simple_util.h"
#include "BankC.h"

/**
 * @class Bank_Client_i
 *
 * @brief Grid_Client interface subclass.
 *
 * This class implements the interface between the interface
 * objects and the client .
 */
class Bank_Client_i
{
public:
  /// Constructor
  Bank_Client_i () = default;

  /// Destructor
  ~Bank_Client_i () = default;

  Bank_Client_i (Bank_Client_i const &) = delete;
  Bank_Client_i &operator= (Bank_Client_i const &) = delete;
  Bank_Client_i (Bank_Client_i &&) = delete;
  Bank_Client_i &operator= (Bank_Client_i &&) = delete;

  /// Execute the methods.
  int run (char const *, int, ACE_TCHAR *[]);

private:
  /// Method that calls all the test functions
  int check_accounts ();

  /// Tests if accounts opened with different names return a different account
  /// reference.
  void test_for_different_name ();

  /// Tests if accounts opened with the same name return the same
  /// object reference.
  void test_for_same_name ();

  /// Tests for the Overdraft Exception when the client tries to
  /// withdraw more money than the current balance.
  void test_for_overdraft ();

  /// Instantiate the client object.
  Client<Bank::AccountManager> client_;
};

#endif /* TIME_CLIENT_I_H */
