// -*- C++ -*-

//=============================================================================
/**
 *  @file   AMI_Primary_Replication_Strategy.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef AMI_PRIMARY_REPLICATION_STRATEGY_H
#define AMI_PRIMARY_REPLICATION_STRATEGY_H

#include "orbsvcs/FtRtEvent/EventChannel/UpdateableHandler.h"
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Strategy.h"
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "ace/Task.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class  AMI_Primary_Replication_Strategy.
 *
 * @brief Used by primary replicas when AMI calls are used for replicating requests.
 */

class AMI_Primary_Replication_Strategy : public Replication_Strategy
                               , public ACE_Task_Base
{
public:
  /**
   * @param mt Specifies whether multithreaded ORB is used.
   */
  AMI_Primary_Replication_Strategy(bool mt);
  virtual ~AMI_Primary_Replication_Strategy();
  virtual void replicate_request(const FTRT::State& state,
                                   RollbackOperation rollback,
                                   const FtRtecEventChannelAdmin::ObjectId& oid);
  virtual void add_member(const FTRT::ManagerInfo & info,
                          CORBA::ULong object_group_ref_version);
  virtual int  acquire_read (void);
  virtual int  acquire_write (void);
  virtual int  release (void);
  PortableServer::POA_ptr poa() { return poa_.in(); }
  CORBA::ORB_ptr orb() { return orb_.in();}
private:
  virtual int svc (void);
  CORBA::ORB_var orb_;
  PortableServer::POA_var root_poa_;
  PortableServer::POA_var poa_;
  PortableServer::POAManager_var mgr_;
  bool running_;
  UpdateableHandler handler_;
  ACE_SYNCH_RW_MUTEX* mutex_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif // AMI_PRIMARY_REPLICATION_STRATEGY_H
