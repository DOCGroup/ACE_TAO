// -*- C++ -*-

//=============================================================================
/**
 *  @file    Account_i.h
 *
 *  This class implements the Bank IDL interface.
 *
 *  @author Vishal Kachroo <vishal@cs.wustl.edu>
 */
//=============================================================================


#ifndef ACCOUNT_I_H
#define ACCOUNT_I_H

#include "BankS.h"

/**
 * @class Account_i
 *
 * @brief Bank object implementation.
 *
 * Implementation of a simple object that has two methods, one
 * that return the current balance on the server and the other
 * that shuts down the server.
 */
class Account_i : public POA_Bank::Account
{
public:
  /// Constructor.
  Account_i () = default;

  /// Constructor.
  Account_i (char const *, CORBA::Float);

  /// Destructor.
  ~Account_i () override = default;

  Account_i (Account_i const &) = delete;
  Account_i &operator= (Account_i const &) = delete;
  Account_i (Account_i &&) = delete;
  Account_i &operator= (Account_i &&) = delete;

  /// Get the current balance in the account.
  CORBA::Float balance () override;

  /// Get the name of the <Account> holder.
  char *name () override;

  /// Set the name of the <Account> holder.
  void name (char const *name) override;

  /// Deposit money in the account.
  void deposit (CORBA::Float) override;

  /// Withdraw money in the account.
  void withdraw (CORBA::Float) override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  /// balance for this account
  CORBA::Float balance_ {};

  /// Name of the <Account> holder.
  CORBA::String_var name_;
};

#endif /* ACCOUNT_I_H */
