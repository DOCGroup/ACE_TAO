// -*- C++ -*-

//=============================================================================
/**
 *  @file    AccountManager_i.h
 *
 *  This class implements the Bank::AccountManager IDL interface.
 *
 *  @author Vishal Kachroo  <vishal@cs.wustl.edu>
 */
//=============================================================================


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

/**
 * @class AccountManager_i
 *
 * @brief Account Manager object implementation.
 *
 * Implementation of a simple object that has two methods, one
 * that returns an Account Interface and the other that shuts
 * down the server.
 */
class AccountManager_i : public POA_Bank::AccountManager
{
public:
  /// Constructor.
  AccountManager_i () = default;

  /// Destructor.
  ~AccountManager_i () override = default;

  AccountManager_i (AccountManager_i const &) = delete;
  AccountManager_i &operator= (AccountManager_i const &) = delete;
  AccountManager_i (AccountManager_i &&) = delete;
  AccountManager_i &operator= (AccountManager_i &&) = delete;

  /// Return the Account interface with the given name from the server.
  /// Put the initial balance specified in the new account.
  Bank::Account_ptr open (char const *name,
                          CORBA::Float initial_balance) override;

  /// Close the given account.
  void close (Bank::Account_ptr) override;

  /// Shutdown the server.
  void shutdown () override;

  /// Set the ORB pointer.
  void orb (CORBA::ORB_ptr o);

  /// Set the POA pointer.
  void poa (PortableServer::POA_ptr poa);

  /// Set the ORB Manager.
  void set_orb_manager (TAO_ORB_Manager *orb_manager);

  /// The ORB manager.
  TAO_ORB_Manager *orb_manager_ {};

private:
  /// ORB pointer.
  CORBA::ORB_var orb_;

  /// POA pointer.
  PortableServer::POA_var poa_;

  using Account_i_var = TAO_Intrusive_Ref_Count_Handle<Account_i>;
  using MAP_MANAGER_TYPE = ACE_Hash_Map_Manager<ACE_CString,
                                                Account_i_var,
                                                ACE_Null_Mutex>;

  /**
   * Calls to <open> will create a new instance of <Account_i> and
   * bind into the hash map manager if <name> is unique, else it will
   * return a previously bound entry.
   */
  MAP_MANAGER_TYPE hash_map_;

};

#endif /* ACCOUNTMANAGER_I_H */
