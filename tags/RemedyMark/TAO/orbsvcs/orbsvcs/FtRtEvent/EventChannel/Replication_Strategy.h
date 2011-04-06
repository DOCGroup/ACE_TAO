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

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

namespace FTEC {
  struct ManagerInfo;
}

class TAO_FTEC_Event_Channel_Impl;
class Replication_Strategy
{
public:
    Replication_Strategy();
    virtual ~Replication_Strategy();

  /**
   * Check if the incoming set_update() request is out of sequence. This is only
   * used for basic replication strategy. It throws FTRT::OutOfSequence when the
   * incoming request is not valid.
   */
    virtual void check_validity(void);

    typedef void (FtRtecEventChannelAdmin::EventChannelFacade::*RollbackOperation)
      (const FtRtecEventChannelAdmin::ObjectId&);

  /**
   * Replicate a request.
   *
   * @param state The request to be replicated.
   * @param rollback The rollback operation when the replication failed.
   * @param oid The object id used for rollback operation.
   */
    virtual void replicate_request(const FTRT::State& state,
                                   RollbackOperation rollback,
                                   const FtRtecEventChannelAdmin::ObjectId& oid)=0;

  /**
   * Inform the backup replicas that a new replica has joined the object
   * group.
   */
  virtual void add_member(const FTRT::ManagerInfo & info,
                          CORBA::ULong object_group_ref_version)=0;

  virtual Replication_Strategy* make_primary_strategy();

  virtual int  acquire_read (void)=0;
  virtual int  acquire_write (void)=0;
  virtual int  release (void)=0;

};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif

