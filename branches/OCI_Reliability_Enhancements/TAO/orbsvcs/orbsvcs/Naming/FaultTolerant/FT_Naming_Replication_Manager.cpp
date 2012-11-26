/* $Id$ */
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Replication_Manager.h"
#include "orbsvcs/Naming/FaultTolerant/FT_NamingReplicationC.h"
#include "orbsvcs/Naming/FaultTolerant/FT_Naming_Server.h"
#include "tao/corba.h"


FT_Naming::ReplicationManager_var TAO_FT_Naming_Replication_Manager::peer_replica_;

TAO_FT_Naming_Replication_Manager::TAO_FT_Naming_Replication_Manager(TAO_FT_Naming_Server *naming_svr,
                                                                     const char* repl_mgr_name)
  : naming_svr_ (naming_svr),
    repl_mgr_name_ (ACE_OS::strdup (repl_mgr_name))
{
}


TAO_FT_Naming_Replication_Manager::~TAO_FT_Naming_Replication_Manager(void)
{
}

void
TAO_FT_Naming_Replication_Manager::initialize (CORBA::ORB_ptr orb,
                                               PortableServer::POA_ptr repl_mgr_poa)
{
  ACE_UNUSED_ARG (orb);
  repl_mgr_poa_ = PortableServer::POA::_duplicate (repl_mgr_poa);
  PortableServer::ObjectId_var id = PortableServer::string_to_ObjectId (this->repl_mgr_name_.in ());
  CORBA::Object_var obj = repl_mgr_poa_->id_to_reference (id.in ());
  this->reference_ = FT_Naming::ReplicationManager::_narrow (obj);
}


void
TAO_FT_Naming_Replication_Manager::register_replica (
    ::FT_Naming::ReplicationManager_ptr replica)
{
  ACE_DEBUG ((LM_DEBUG, "Replica registration message received.\n"));

  // Store the provided reference
  peer_replica_ = replica;
}

void
TAO_FT_Naming_Replication_Manager::notify_updated_object_group (
    const FT_Naming::ObjectGroupUpdate & group_info)
{
  ACE_DEBUG ((LM_DEBUG, "Received OG update message\n"));
  // Make sure that we have a valid naming server
  ACE_ASSERT (naming_svr_);
  int result = this->naming_svr_->update_object_group (group_info);

  if (result != 0)
    ACE_ERROR ((LM_ERROR, "Unable to update object group"));
}

void
TAO_FT_Naming_Replication_Manager::notify_updated_context (
    const FT_Naming::NamingContextUpdate & context_info)
{
  ACE_DEBUG ((LM_DEBUG, "Received NC update message\n"));
  ACE_ASSERT (naming_svr_);
  int result = this->naming_svr_->update_naming_context (context_info);
}

FT_Naming::ReplicationManager_ptr
TAO_FT_Naming_Replication_Manager::peer_replica (void)
{
  return peer_replica_._retn ();
}


int
TAO_FT_Naming_Replication_Manager::register_with_peer_replica (
    FT_Naming::ReplicationManager_ptr replica)
{
  ACE_DEBUG ((LM_DEBUG, "Registering with the peer replica.\n"));

  int result = 0;
  try {
    // Store the peer reference for future access
    this->peer_replica_ = FT_Naming::ReplicationManager::_duplicate (replica);

    // Register with the peer replica
    this->peer_replica_->register_replica (this->reference ());
  }
  catch (CORBA::Exception& ex) {
    ex._tao_print_exception ("Exception while attempting to register with peer replica.\n");
    result = -1;
  }

  return result;

}

FT_Naming::ReplicationManager_ptr
TAO_FT_Naming_Replication_Manager::reference (void)
{
  return reference_;
}
