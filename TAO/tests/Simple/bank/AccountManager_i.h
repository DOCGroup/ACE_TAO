// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple/Bank
//
// = FILENAME
//    AccountManager_i.h
//
// = DESCRIPTION
//    This class implements the Bank::AccountManager IDL interface.
//
// = AUTHOR
//    Vishal Kachroo  <vishal@cs.wustl.edu>
//
// ============================================================================

#if !defined (ACCOUNTMANAGER_I_H)
#define	ACCOUNTMANAGER_I_H

#include "BankS.h"

class AccountManager_i:
  public POA_Bank::AccountManager
{
  // = TITLE
  //    Bank Object Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one that
  //    returns an Account Interface and the other that shuts down the server

public:
  // = Initialization and termination methods.
  AccountManager_i (void);
  // Constructor

  ~AccountManager_i (void);
  // Destructor

  Bank::Account_ptr open (const char *,CORBA::Environment &env);
  // Return the Account interface with the given name from the server.

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown the server

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.
};

#endif /* ACCOUNTMANAGER_I_H */
