// $Id$

#include "orbsvcs/FtRtEvent/EventChannel/Basic_Replication_Strategy.h"
#include "orbsvcs/FtRtEvent/EventChannel/GroupInfoPublisher.h"
#include "orbsvcs/FtRtEvent/EventChannel/FTEC_Event_Channel.h"
#include "orbsvcs/FtRtEvent/EventChannel/Request_Context_Repository.h"
#include "../Utils/Log.h"

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

/// The mutex has to be recursive; otherwise, if the second replicate_request() is
/// called while the first replicate_request() is waiting for reply, we will get
/// a deadlock.
Basic_Replication_Strategy::Basic_Replication_Strategy(bool mt)
  : sequence_num_(0)
  , mutex_(mt ? new ACE_SYNCH_RECURSIVE_MUTEX : 0)
{
}

Basic_Replication_Strategy::~Basic_Replication_Strategy()
{
  delete mutex_;
}

void
Basic_Replication_Strategy::check_validity(void)
{
    FTRT::SequenceNumber seq_no = Request_Context_Repository().get_sequence_number();

    TAO_FTRTEC::Log(1 , ACE_TEXT("check_validity : sequence no = %d\n"), sequence_num_);

    if (this->sequence_num_ == 0) {
      // this is the first set_update received from the primary
      // sync the sequence number with the primary
      this->sequence_num_ = seq_no;
    }
    else if (seq_no != this->sequence_num_+1) {
      // out of sync, we missed some set_update() request already
      // throw exception
      //            client_interceptor_->sequence_num_--;
      FTRT::OutOfSequence exception;
      exception.current = this->sequence_num_;
      TAO_FTRTEC::Log(3, ACE_TEXT("Throwing FTRT::OutOfSequence (old sequence_num_ = %d)\n"), this->sequence_num_);
      throw FTRT::OutOfSequence(exception);
    }
    else
      this->sequence_num_++;
}

void twoway_set_update(FtRtecEventChannelAdmin::EventChannel_var successor,
                       const FTRT::State& state)
{
  bool finished = true;
  do {
    try{
      successor->set_update(state);
    }
    catch (const CORBA::COMM_FAILURE& ex){
      if (ex.minor() == 6)   finished = false;
      else
        throw;
    }
  } while(!finished);
}

void
Basic_Replication_Strategy::replicate_request(
  const FTRT::State& state,
  RollbackOperation rollback,
  const FtRtecEventChannelAdmin::ObjectId& oid)
{
  ACE_UNUSED_ARG(rollback);
  ACE_UNUSED_ARG(oid);

  FTRT::TransactionDepth transaction_depth =
    Request_Context_Repository().get_transaction_depth();

  GroupInfoPublisherBase * info_publisher = GroupInfoPublisher::instance();
  FtRtecEventChannelAdmin::EventChannel_var successor = info_publisher->successor();
  if (!CORBA::is_nil(successor.in())) {
    if (info_publisher->is_primary())
      this->sequence_num_++;

    TAO_FTRTEC::Log(1, ACE_TEXT("replicate_request : sequence no = %d\n"), sequence_num_);
    Request_Context_Repository().set_sequence_number(sequence_num_);

    Request_Context_Repository().set_transaction_depth(transaction_depth-1);

    if (transaction_depth > 1) {
      twoway_set_update(successor, state);
    }
    else {
      try{
        successor->oneway_set_update(state);
      }
      catch (const CORBA::Exception&){
      }
    }
  }
  else if (transaction_depth > 1) {
    TAO_FTRTEC::Log(3, ACE_TEXT("Throwing FTRT::TransactionDepthTooHigh\n"));
    throw FTRT::TransactionDepthTooHigh();
  }
}

void
Basic_Replication_Strategy::add_member(const FTRT::ManagerInfo & info,
                                       CORBA::ULong object_group_ref_version)
{
  FtRtecEventChannelAdmin::EventChannel_var successor = GroupInfoPublisher::instance()->successor();
  bool finished = true;
  do {
    try{
      successor->add_member(info, object_group_ref_version);
    }
    catch (const CORBA::COMM_FAILURE& ex){
      if (ex.minor() == 6) finished = false;
      else throw;
    }
  } while (!finished);
}

int  Basic_Replication_Strategy::acquire_read (void)
{
  return mutex_ ? mutex_->acquire_read() : 0;
}

int  Basic_Replication_Strategy::acquire_write (void)
{
  return mutex_ ? mutex_->acquire_write() : 0;
}

int  Basic_Replication_Strategy::release (void)
{
  return mutex_ ? mutex_->release() : 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
