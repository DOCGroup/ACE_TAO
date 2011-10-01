// -*- C++ -*-

//=============================================================================
/**
 *  @file    Account_i.h
 *
 *  $Id$
 *
 *  This class implements the Bank IDL interface.
 *
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
  // = Initialization and termination methods.
  /// Constructor.
  Account_i (void);

  /// Constructor.
  Account_i (const char *, CORBA::Float);

  /// Destructor.
  virtual ~Account_i (void);

  /// Get the current balance in the account.
  virtual CORBA::Float balance (void);

  /// Get the name of the <Account> holder.
  virtual char *name (void);

  /// Set the name of the <Account> holder.
  virtual void name (const char *name);

  /// Deposit money in the account.
  virtual void deposit (CORBA::Float);

  /// Withdraw money in the account.
  virtual void withdraw (CORBA::Float);

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  /// balance for this account
  CORBA::Float balance_;

  /// Name of the <Account> holder.
  CORBA::String_var name_;
};

#endif /* ACCOUNT_I_H */
