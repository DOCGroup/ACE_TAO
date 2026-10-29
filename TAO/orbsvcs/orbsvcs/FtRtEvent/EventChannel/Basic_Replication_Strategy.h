// -*- C++ -*-

//=============================================================================
/**
 *  @file   Basic_Replication_Strategy.h
 *
 *  @author Huang-Ming Huang <hh1@cse.wustl.edu>
 */
//=============================================================================
#ifndef BASIC_REPLICATION_STRATEGY_H
#define BASIC_REPLICATION_STRATEGY_H
#include "orbsvcs/FtRtEvent/EventChannel/Replication_Strategy.h"
#include "ace/Synch.h"

#if !defined (ACE_LACKS_PRAGMA_ONCE)
# pragma once
#endif /* ACE_LACKS_PRAGMA_ONCE */

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/**
 * @class  Basic_Replication_Strategy
 *
 * @brief Use two-way CORBA call to replicate the state to backup replicas.
 */

class Basic_Replication_Strategy : public Replication_Strategy
{
public:
  /**
   * @param mt Specifies whether multithreaded ORB is used.
   */
  explicit Basic_Replication_Strategy(bool mt);
  ~Basic_Replication_Strategy() override;

  void check_validity() override;

  void replicate_request(FTRT::State const& state,
    RollbackOperation rollback,
    FtRtecEventChannelAdmin::ObjectId const& oid) override;
  void add_member(FTRT::ManagerInfo const& info,
                  CORBA::ULong object_group_ref_version) override;

  int acquire_read () override;
  int acquire_write () override;
  int release () override;

private:
  Basic_Replication_Strategy(Basic_Replication_Strategy const&) = delete;
  Basic_Replication_Strategy(Basic_Replication_Strategy&&) = delete;
  Basic_Replication_Strategy& operator=(Basic_Replication_Strategy const&) = delete;
  Basic_Replication_Strategy& operator=(Basic_Replication_Strategy&&) = delete;

  FTRT::SequenceNumber sequence_num_;
  ACE_SYNCH_RECURSIVE_MUTEX* mutex_;
};

TAO_END_VERSIONED_NAMESPACE_DECL

#endif
