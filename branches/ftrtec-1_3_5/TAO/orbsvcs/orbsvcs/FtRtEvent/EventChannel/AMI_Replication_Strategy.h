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

#include "ace/Task.h"
#include "tao/PortableServer/PortableServerC.h"
#include "orbsvcs/FTRT_GroupManagerC.h"
#include "Replication_Strategy.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class AMI_Replication_Strategy: public Replication_Strategy
{
public:
  AMI_Replication_Strategy();
  virtual void replicate_request(const FTRT::State& state,
                                 RollbackOperation rollback,
                                 const FtRtecEventChannelAdmin::ObjectId& oid
                                 ACE_ENV_ARG_DECL);
  virtual void add_member(const FTRT::ManagerInfo & info,
                          CORBA::ULong object_group_ref_version
                          ACE_ENV_ARG_DECL);
  virtual Replication_Strategy* make_primary_strategy();

  virtual int  acquire_read (void);
  virtual int  acquire_write (void);
  virtual int  release (void);

};


#endif
