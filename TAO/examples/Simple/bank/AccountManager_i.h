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

#ifndef ACCOUNTMANAGER_I_H
#define ACCOUNTMANAGER_I_H

#include "BankS.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "Account_i.h"
#include "tao/PortableServer/ORB_Manager.h"

#include "ace/Hash_Map_Manager.h"
#include "ace/OS.h"
#include "ace/ACE.h"

class AccountManager_i : public POA_Bank::AccountManager
{
  // = TITLE
  //    Account Manager object implementation.
  //
  // = DESCRIPTION
  //    Implementation of a simple object that has two methods, one
  //    that returns an Account Interface and the other that shuts
  //    down the server.
public:
  // = Initialization and termination methods.
  AccountManager_i (void);
  // Constructor.

  AccountManager_i (AccountManager_i&);
  // Copy constructor for old g++.

  ~AccountManager_i (void);
  // Destructor.

  virtual Bank::Account_ptr open (const char *name,
                                  CORBA::Float initial_balance
                                  TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Return the Account interface with the given name from the server.
  // Put the initial balance specified in the new account.

  virtual void close (Bank::Account_ptr
                      TAO_ENV_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Close the given account.

  virtual void shutdown (TAO_ENV_SINGLE_ARG_DECL)
    ACE_THROW_SPEC ((CORBA::SystemException));
  // Shutdown the server.

  void orb (CORBA::ORB_ptr o);
  // Set the ORB pointer.

  void poa (PortableServer::POA_ptr poa);
  // Set the POA pointer.

  void set_orb_manager (TAO_ORB_Manager *orb_manager);
  // Set the ORB Manager.

  TAO_ORB_Manager *orb_manager_;
  // The ORB manager.

private:
  CORBA::ORB_var orb_;
  // ORB pointer.

  PortableServer::POA_ptr poa_;
  // POA pointer.

  //TAO_ORB_Manager orb_manager_;
  // The ORB manager.

  ACE_Hash_Map_Manager<ACE_CString,
                       Account_i *,
                       ACE_Null_Mutex> hash_map_;
  // Calls to <open> will create a new instance of <Account_i> and
  // bind into the hash map manager if <name> is unique, else it will
  // return a previously bound entry.

  ACE_UNIMPLEMENTED_FUNC (void operator= (const AccountManager_i &))
  // Keeping g++2.7.2
};

#endif /* ACCOUNTMANAGER_I_H */
