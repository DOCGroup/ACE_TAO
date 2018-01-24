/* -*- C++ -*- */

//=============================================================================
/**
*  @file FT_Naming_Replication_Manager.h
*
*
*  Based on the replicator class used in the ImR.
*
*/
//=============================================================================

#ifndef FT_REPLICATOR_H
#define FT_REPLICATOR_H

#include "ace/config-lite.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FT_NamingReplicationS.h"
#include "ace/Bound_Ptr.h"
#include "ace/Vector_T.h"
#include "ace/Task.h"

#include <set>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

class TAO_FT_Naming_Replication_Manager;
class TAO_FT_Naming_Server;

class FT_Update_Replicant_i : public virtual POA_FT_Naming::UpdatePushNotifier
{
public:
  FT_Update_Replicant_i (TAO_FT_Naming_Replication_Manager &owner);

  virtual void register_peer (::FT_Naming::UpdatePushNotifier_ptr peer,
                              const ::FT_Naming::ReplicaInfo & info,
                              CORBA::ULongLong seq_num);

  virtual void notify_update (CORBA::ULongLong seq_num,
                              FT_Naming::UpdateInfoSeq& info);

private:
  TAO_FT_Naming_Replication_Manager &owner_;
};

/**
* @class Replicator
*
* @brief Manages the notification between the peers whenever a
* context or object group changes state. The goal is to ensure
* the servicing ORB never blocks for want of a channel to notify
* the peer.
*
* This goal is attained by running a separate ORB in a separate thread,
* and using the ORB's reactor notification mechanism as a way to ensure
* the application ORB never blocks.
*/

class TAO_FT_Naming_Replication_Manager : public ACE_Task_Base
{
public:
  friend class FT_Update_Replicant_i;

  typedef FT_Naming::UpdatePushNotifier_var Replica_var;
  typedef FT_Naming::UpdatePushNotifier_ptr Replica_ptr;

  TAO_FT_Naming_Replication_Manager (TAO_FT_Naming_Server &owner);

  virtual ~TAO_FT_Naming_Replication_Manager(void);

  virtual int svc (void);
  virtual int handle_exception (ACE_HANDLE );

  void stop (void);
  void send_context_update (const ACE_CString &ctx,
                            FT_Naming::ChangeType update);
  void send_objgrp_update (PortableGroup::ObjectGroupId id,
                           FT_Naming::ChangeType update);

  void init_orb (void);

  bool init_peer (void );
  bool peer_init_i (void );
  void send_registration (bool use_combo);
  bool update_peer_registration ();

  bool peer_available (void);
  char * ior (void);

private:
  Replica_var me_;
  Replica_var peer_;
  CORBA::ULongLong seq_num_;
  CORBA::ULongLong replica_seq_num_;
  TAO_FT_Naming_Server &server_;
  CORBA::ORB_var orb_;
  ACE_Reactor *reactor_;
  TAO_SYNCH_MUTEX lock_;
  bool notified_;
  FT_Naming::UpdateInfoSeq to_send_;
  ACE_CString endpoint_;
  ACE_Time_Value update_delay_;
  ACE_CString replica_ior_;
  bool send_combos_;
  bool refresh_peer_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif /* FT_REPLICATOR_H */
