// -*- C++ -*-
// $Id$

// ============================================================================
//
// = LIBRARY
//    TAO/tests/Simple/Bank
//
// = FILENAME
//    Bank_i.h
//
// = DESCRIPTION
//    This class implements the Bank IDL interface.
//
// = AUTHOR
//    Vishal Kachroo <vishal@cs.wustl.edu>
//
// ============================================================================

#if !defined (BANK_I_H)
#define	BANK_I_H

#include "BankS.h"

class Bank_i:
  public POA_Bank::Account
{
  // = TITLE
  //    Bank Object Implementation
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one that
  //    return the current balance on the server and the other that
  //    shuts down the server.
public:
  // = Initialization and termination methods.
  Bank_i (void);
  // Constructor

  ~Bank_i (void);
  // Destructor

  virtual CORBA::Float balance (CORBA::Environment &env);
  // Return the current time/date on the server.

  virtual void shutdown (CORBA::Environment &env);
  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.
};

#endif /* BANK_I_H */
