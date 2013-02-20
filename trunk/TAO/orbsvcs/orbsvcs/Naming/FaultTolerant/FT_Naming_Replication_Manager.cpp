// -*- C++ -*-

//=============================================================================
/**
 * @file   FT_Naming_Replication_Manager.cpp
 *
 * $Id$
 *
 * @author Kevin Stanley <stanleyk@ociweb.com>
 */
//=============================================================================

#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Replication_Manager.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Server.h"
#include "tao/corba.h"
#include "ace/SStringfwd.h"

::FT_Naming::ReplicationManager_var
TAO_FT_Naming_Replication_Manager::peer_replica_ (0);

TAO_FT_Naming_Replication_Manager::TAO_FT_Naming_Replication_Manager (
     TAO_FT_Naming_Server *naming_svr,
     const char* repl_mgr_name)
  : naming_svr_ (naming_svr),
    repl_mgr_name_ (repl_mgr_name)
{
}


TAO_FT_Naming_Replication_Manager::~TAO_FT_Naming_Replication_Manager(void)
{
  this->reference_ = ::FT_Naming::ReplicationManager::_nil ();
}

void
TAO_FT_Naming_Replication_Manager::initialize (CORBA::ORB_ptr orb,
                                               PortableServer::POA_ptr repl_mgr_poa)
{
  ACE_UNUSED_ARG (orb);
  repl_mgr_poa_ = PortableServer::POA::_duplicate (repl_mgr_poa);
  PortableServer::ObjectId_var id =
    PortableServer::string_to_ObjectId (this->repl_mgr_name_.c_str ());
  CORBA::Object_var obj = repl_mgr_poa_->id_to_reference (id.in ());
  this->reference_ = ::FT_Naming::ReplicationManager::_narrow (obj.in ());
}

::FT_Naming::ReplicaInfo*
TAO_FT_Naming_Replication_Manager::register_replica (
  ::FT_Naming::ReplicationManager_ptr replica,
  const ::FT_Naming::ReplicaInfo & replica_info)
{
  ACE_TRACE ( ACE_TEXT("TAO_FT_Naming_Replication_Manager::register_replica"));

  ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                      ace_mon,
                      this->lock_,
                      CORBA::INTERNAL ());

  // Store a copy of the provided reference and other ReplicaInfo
  peer_replica_ = ::FT_Naming::ReplicationManager::_duplicate (replica);

  // Store the provided peer references
  this->naming_svr_->peer_root_context (replica_info.root_context);
  this->naming_svr_->peer_naming_manager (replica_info.naming_manager);

  // Return my references to the peer
  ::FT_Naming::ReplicaInfo* my_info = new ::FT_Naming::ReplicaInfo;

  my_info->root_context = this->naming_svr_->my_root_context ();

  my_info->naming_manager = this->naming_svr_->my_naming_manager ();

  return my_info;
}

void
TAO_FT_Naming_Replication_Manager::notify_updated_object_group (
    const ::FT_Naming::ObjectGroupUpdate & group_info)
{
  ACE_TRACE ( ACE_TEXT ("TAO_FT_Naming_Replication_Manager::")
              ACE_TEXT ("notify_updated_object_group"));

  // Make sure that we have a valid naming server
  ACE_ASSERT (naming_svr_);
  int result = this->naming_svr_->update_object_group (group_info);

  if (result != 0)
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Unable to update object group.\n")));
}

void
TAO_FT_Naming_Replication_Manager::notify_updated_context (
    const ::FT_Naming::NamingContextUpdate & context_info)
{
  ACE_TRACE (ACE_TEXT ("TAO_FT_Naming_Replication_Manager::")
             ACE_TEXT ("notify_updated_context"));
  ACE_ASSERT (naming_svr_);
  int result = this->naming_svr_->update_naming_context (context_info);

  if (result != 0)
  {
    ACE_ERROR ((LM_ERROR,
                ACE_TEXT ("Error while updating naming context.\n")));
  }
}

::FT_Naming::ReplicationManager_ptr
TAO_FT_Naming_Replication_Manager::peer_replica (void)
{
  ACE_TRACE (ACE_TEXT ("TAO_FT_Naming_Replication_Manager::peer_replica"));
  // Return a copy of the stored peer to the requester
  return ::FT_Naming::ReplicationManager::_duplicate (peer_replica_.in ());
}

int
TAO_FT_Naming_Replication_Manager::register_with_peer_replica (
      ::FT_Naming::ReplicationManager_ptr replica,
      CosNaming::NamingContext_ptr nc,
      ::FT_Naming::NamingManager_ptr nm)
{
  ACE_TRACE (ACE_TEXT ("TAO_FT_Naming_Replication_Manager::")
             ACE_TEXT ("register_with_peer_replica"));

  int result = 0;
  ::FT_Naming::ReplicaInfo my_info;
  { // Guard the access to the Replication Manager state
    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        CORBA::INTERNAL ());

    // Store a copy of the peer reference for future access
    this->peer_replica_ =
      ::FT_Naming::ReplicationManager::_duplicate (replica);

    my_info.root_context = CosNaming::NamingContext::_duplicate (nc);
    my_info.naming_manager = ::FT_Naming::NamingManager::_duplicate (nm);
  }

  try {
    ::FT_Naming::ReplicationManager_var my_ref =
      this->reference ();

    // Register with the peer replica
    ::FT_Naming::ReplicaInfo_var peer_info =
      this->peer_replica_->register_replica (my_ref.in (),
                                             my_info);

    ACE_GUARD_THROW_EX (TAO_SYNCH_MUTEX,
                        ace_mon,
                        this->lock_,
                        CORBA::INTERNAL ());

    // Store the returned references locally
    this->naming_svr_->peer_root_context (peer_info->root_context);
    this->naming_svr_->peer_naming_manager (peer_info->naming_manager);
  }
  catch (const CORBA::Exception& ex) {
    // Unable to contact the peer replica.
    if (TAO_debug_level > 1)
      ex._tao_print_exception (
        ACE_TEXT ("TAO_FT_Naming_Replication_Manager::")
        ACE_TEXT ("register_with_peer_replica\n"));
    result = -1;
  }

  return result;

}

::FT_Naming::ReplicationManager_ptr
TAO_FT_Naming_Replication_Manager::reference (void)
{
  ACE_TRACE (ACE_TEXT ("TAO_FT_Naming_Replication_Manager::reference"));
  return ::FT_Naming::ReplicationManager::_duplicate (reference_.in ());
}
