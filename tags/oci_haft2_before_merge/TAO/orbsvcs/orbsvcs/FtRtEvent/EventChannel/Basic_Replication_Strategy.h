// -*- C++ -*-

//=============================================================================
/**
 *  @file   Basic_Replication_Strategy.h
 *
 *  $Id$
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef BASIC_REPLICATION_STRATEGY_H
#define BASIC_REPLICATION_STRATEGY_H
#include "Replication_Strategy.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

class Basic_Replication_Strategy : public Replication_Strategy
{
public:
  Basic_Replication_Strategy();

  virtual void check_validity(ACE_ENV_SINGLE_ARG_DECL);

  virtual void replicate_request(
    const FTRT::State& state,
    RollbackOperation rollback,
    const FtRtecEventChannelAdmin::ObjectId& oid
    ACE_ENV_ARG_DECL);

  virtual int  acquire_read (void);
  virtual int  acquire_write (void);
  virtual int  release (void);

private:
  FTRT::SequenceNumber sequence_num_;
  ACE_Thread_Mutex mutex_;
};

#endif
