// -*- C++ -*-

//=============================================================================
/**
 *  @file   AMI_Replication_Strategy.h
 *
 *  $Id$
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
  AMI_Replication_Strategy(bool mt);
  ~AMI_Replication_Strategy();
  virtual void replicate_request(const FTRT::State& state,
    RollbackOperation rollback,
    const FtRtecEventChannelAdmin::ObjectId& oid);
  virtual void add_member(const FTRT::ManagerInfo & info,
                          CORBA::ULong object_group_ref_version);
  virtual Replication_Strategy* make_primary_strategy();

  virtual int  acquire_read (void);
  virtual int  acquire_write (void);
  virtual int  release (void);
private:
  bool mt_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
