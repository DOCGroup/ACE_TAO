// -*- C++ -*-

//=============================================================================
/**
 *  @file   Replication_Strategy.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================

#ifndef REPLICATION_STRATEGY_H
#define REPLICATION_STRATEGY_H
#include "orbsvcs/FtRtecEventChannelAdminC.h"
#include "tao/PortableInterceptorC.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */


namespace FTEC {
  struct ManagerInfo;
};

class TAO_FTEC_Event_Channel_Impl;
class Replication_Strategy
{
public:
  Replication_Strategy();
  virtual ~Replication_Strategy();

  virtual void check_validity(ACE_ENV_SINGLE_ARG_DECL);

  typedef void (FtRtecEventChannelAdmin::EventChannelFacade::*RollbackOperation)
    (const FtRtecEventChannelAdmin::ObjectId& ACE_ENV_ARG_DECL_WITH_DEFAULTS);

  virtual void replicate_request(const FTRT::State& state,
                                 RollbackOperation rollback,
                                 const FtRtecEventChannelAdmin::ObjectId& oid
                                 ACE_ENV_ARG_DECL)=0;

  virtual void add_member(const FTRT::ManagerInfo & info,
                          CORBA::ULong object_group_ref_version
                          ACE_ENV_ARG_DECL)=0;

  virtual Replication_Strategy* make_primary_strategy();

  virtual int  acquire_read (void)=0;
  virtual int  acquire_write (void)=0;
  virtual int  release (void)=0;
};

#endif

