/* -*- C++ -*- */

//=============================================================================
/**
*  @file Replicator.h
*
*  This class manages the synchronizing of XML files and server status
*  shared between multiple Locators.
*
*/
//=============================================================================

#ifndef REPLICATOR_H
#define REPLICATOR_H

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "ImR_LocatorS.h"
#include "ace/Bound_Ptr.h"
#include "ace/Vector_T.h"
#include "ace/Task.h"

#include <set>

class Replicator;
class Shared_Backing_Store;
class Options;

class UPN_i : public virtual POA_ImplementationRepository::UpdatePushNotification
{
public:
  explicit UPN_i (Replicator &owner);

  /// provide the implementation for being notified of a
  /// server or activator update
  void notify_update (CORBA::ULongLong seq_num,
                      ImplementationRepository::UpdateInfoSeq const& info) override;

  /// provide the implementation for registering a peer replica
  /// @param replica the peer replica
  /// @param ft_imr_ior the fault tolerant ImR IOR (passed in
  ///        as the replica's ImR IOR, passed back as fault
  ///        tolerant ImR IOR)
  /// @param seq_num current sequence number to return to replica
  void register_replica
  (ImplementationRepository::UpdatePushNotification_ptr replica,
   char*& ft_imr_ior,
   CORBA::ULongLong_out seq_num) override;

private:
  Replicator &owner_;
};

/**
* @class Replicator
*
* @brief XML backing store containing all ImR persistent information in
* multiple files shared between multiple Locators
*
*/
class Replicator : public ACE_Task_Base
{
public:
  friend class UPN_i;

  using Replica_var = ImplementationRepository::UpdatePushNotification_var;
  using Replica_ptr = ImplementationRepository::UpdatePushNotification_ptr;
  using AAM_StatusMap = ACE_Hash_Map_Manager_Ex<ACE_CString,
    ImplementationRepository::AAM_Status,
    ACE_Hash<ACE_CString>,
    ACE_Equal_To<ACE_CString>,
    ACE_Null_Mutex>;

  Replicator (Shared_Backing_Store &repo, const Options& opts);

  ~Replicator() override;

  int svc () override;
  int handle_exception (ACE_HANDLE ) override;

  void stop ();
  void send_access_state (const char *name, ImplementationRepository::AAM_Status state);
  void send_entity (ImplementationRepository::UpdateInfo &info);

  void init_orb ();

  bool init_peer (const ACE_CString &filename );
  int send_registration (char *&ior );

  bool peer_available ();
  char * ior ();

private:
  Replica_var me_;
  Replica_var peer_;
  CORBA::ULongLong seq_num_;
  CORBA::ULongLong replica_seq_num_;
  Shared_Backing_Store &repo_;
  AAM_StatusMap prev_update_;
  CORBA::ORB_var orb_;
  ACE_Reactor *reactor_;
  TAO_SYNCH_MUTEX lock_;
  bool notified_;
  ImplementationRepository::UpdateInfoSeq to_send_;
  int debug_;
  ACE_CString endpoint_;
  ACE_Time_Value update_delay_;
};

#endif /* REPLICATOR_H */
