/* $Id$ */
#include "FT_Naming_Replication_Manager.h"
#include "FT_NamingReplicationC.h"
#include "FT_Naming_Server.h"
#include "tao/corba.h"


FT_Naming::ReplicationManager_var TAO_FT_Naming_Replication_Manager::peer_replica_;

TAO_FT_Naming_Replication_Manager::TAO_FT_Naming_Replication_Manager(TAO_FT_Naming_Server *naming_svr)
  : naming_svr_ (naming_svr)
{
}


TAO_FT_Naming_Replication_Manager::~TAO_FT_Naming_Replication_Manager(void)
{
}

void
TAO_FT_Naming_Replication_Manager::initialize (CORBA::ORB_ptr orb,
                   PortableServer::POA_ptr root_poa)
{
   // Nothing to do now
  ACE_UNUSED_ARG (orb);
  ACE_UNUSED_ARG (root_poa);
}


void
TAO_FT_Naming_Replication_Manager::register_replica (
    ::FT_Naming::ReplicationManager_ptr replica)
{
  // Store the provided reference
  peer_replica_ = replica;
}

void
TAO_FT_Naming_Replication_Manager::notify_updated_object_group (
    const FT_Naming::ObjectGroupUpdate & group_info)
{
  // Make sure that we have a valid naming server
  ACE_ASSERT (naming_svr_);
  int result = this->naming_svr_->update_object_group (group_info);

}

void
TAO_FT_Naming_Replication_Manager::notify_updated_context (
    const FT_Naming::NamingContextUpdate & context_info)
{
  ACE_ASSERT (naming_svr_);
  int result = this->naming_svr_->update_naming_context (context_info);
}

FT_Naming::ReplicationManager_ptr
TAO_FT_Naming_Replication_Manager::peer_replica (void)
{
  return peer_replica_._retn ();
}
