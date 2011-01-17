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

#include "ace/Hash_Map_Manager.h"
#include "ace/ACE.h"
#include "ace/SString.h"
#include "ace/Null_Mutex.h"
#include "tao/Intrusive_Ref_Count_Handle_T.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL
class TAO_ORB_Manager;
TAO_END_VERSIONED_NAMESPACE_DECL

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

  virtual ~AccountManager_i (void);
  // Destructor.

  virtual Bank::Account_ptr open (const char *name,
                                  CORBA::Float initial_balance);
  // Return the Account interface with the given name from the server.
  // Put the initial balance specified in the new account.

  virtual void close (Bank::Account_ptr);
  // Close the given account.

  virtual void shutdown (void);
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

  PortableServer::POA_var poa_;
  // POA pointer.

  typedef TAO_Intrusive_Ref_Count_Handle<Account_i> Account_i_var;
  typedef ACE_Hash_Map_Manager<ACE_CString,
                               Account_i_var,
                               ACE_Null_Mutex> MAP_MANAGER_TYPE;

  MAP_MANAGER_TYPE hash_map_;
  // Calls to <open> will create a new instance of <Account_i> and
  // bind into the hash map manager if <name> is unique, else it will
  // return a previously bound entry.

  ACE_UNIMPLEMENTED_FUNC (void operator= (const AccountManager_i &))
  // Keeping g++2.7.2
};

#endif /* ACCOUNTMANAGER_I_H */
