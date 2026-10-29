// -*- C++ -*-

//=============================================================================
/**
 *  @file   AMI_Replication_Strategy.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef AMI_REPLICATION_STRATEGY_H
#define AMI_REPLICATION_STRATEGY_H

#include "orbsvcs/FtRtEvent/EventChannel/Replication_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

#include "orbsvcs/FTRT_GroupManagerC.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Task.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class  AMI_Replication_Strategy.
 *
 * @brief Used by backup replicas when AMI calls are used for replicating requests.
 */
class AMI_Replication_Strategy : public Replication_Strategy
{
public:
  /**
   * @param mt Specifies whether multithreaded ORB is used.
   */
  explicit AMI_Replication_Strategy(bool mt);
  ~AMI_Replication_Strategy() override = default;
  void replicate_request(FTRT::State const& state,
    RollbackOperation rollback,
    FtRtecEventChannelAdmin::ObjectId const& oid) override;
  void add_member(FTRT::ManagerInfo const& info,
                  CORBA::ULong object_group_ref_version) override;
  Replication_Strategy* make_primary_strategy() override;

  int acquire_read () override;
  int acquire_write () override;
  int release () override;
private:
  AMI_Replication_Strategy(AMI_Replication_Strategy const&) = delete;
  AMI_Replication_Strategy(AMI_Replication_Strategy&&) = delete;
  AMI_Replication_Strategy& operator=(AMI_Replication_Strategy const&) = delete;
  AMI_Replication_Strategy& operator=(AMI_Replication_Strategy&&) = delete;

  bool mt_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
