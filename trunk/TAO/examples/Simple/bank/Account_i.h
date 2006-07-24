// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple/Bank
//
// = FILENAME
//    Account_i.h
//
// = DESCRIPTION
//    This class implements the Bank IDL interface.
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#ifndef ACCOUNT_I_H
#define ACCOUNT_I_H

#include "BankS.h"

class Account_i : public POA_Bank::Account
{
  // = TITLE
  //    Bank object implementation.
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one
  //    that return the current balance on the server and the other
  //    that shuts down the server.
public:
  // = Initialization and termination methods.
  Account_i (void);
  // Constructor.

  Account_i (const char *, CORBA::Float);
  // Constructor.

  ~Account_i (void);
  // Destructor.

  virtual CORBA::Float balance (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));

  // Get the current balance in the account.

  virtual char *name (ACE_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Get the name of the <Account> holder.

  virtual void name (const char *name
                     ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Set the name of the <Account> holder.

  virtual void deposit (CORBA::Float
                        ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Deposit money in the account.

  virtual void withdraw (CORBA::Float
                         ACE_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException,
                     Bank::Account::Overdraft));
  // Withdraw money in the account.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.

  CORBA::Float balance_;
  // balance for this account

  CORBA::String_var name_;
  // Name of the <Account> holder.
};

#endif /* ACCOUNT_I_H */
