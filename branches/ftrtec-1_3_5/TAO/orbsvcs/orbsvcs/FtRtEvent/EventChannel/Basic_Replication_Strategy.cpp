// $Id$

#include "Basic_Replication_Strategy.h"
#include "GroupInfoPublisher.h"
#include "FTEC_Event_Channel.h"
#include "Request_Context_Repository.h"
#include "../Utils/Log.h"

ACE_RCSID (EventChannel,
           Basic_Replication_Strategy,
           "$Id$")

Basic_Replication_Strategy::Basic_Replication_Strategy()
: sequence_num_(0)
{
}

void
Basic_Replication_Strategy::check_validity(ACE_ENV_SINGLE_ARG_DECL)
{
    FTRT::SequenceNumber seq_no = Request_Context_Repository().get_sequence_number(ACE_ENV_SINGLE_ARG_PARAMETER);
    ACE_CHECK;

    TAO_FTRTEC::Log(1 , "check_validity : sequence no = %d\n", sequence_num_);

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
      ACE_THROW(FTRT::OutOfSequence(exception));
    }
    else
      this->sequence_num_++;
}


void
Basic_Replication_Strategy::replicate_request(
  const FTRT::State& state,
  RollbackOperation rollback,
  const FtRtecEventChannelAdmin::ObjectId& oid
  ACE_ENV_ARG_DECL)
{
  ACE_UNUSED_ARG(rollback);
  ACE_UNUSED_ARG(oid);

  FTRT::TransactionDepth transaction_depth =
    Request_Context_Repository().get_transaction_depth(ACE_ENV_SINGLE_ARG_PARAMETER);
  ACE_CHECK;

  GroupInfoPublisherBase * info_publisher = GroupInfoPublisher::instance();
  FtRtecEventChannelAdmin::EventChannel_var successor = info_publisher->successor();
  if (!CORBA::is_nil(successor.in())) {
    if (info_publisher->is_primary())
      this->sequence_num_++;

    TAO_FTRTEC::Log(1, "replicate_request : sequence no = %d\n", sequence_num_);
    Request_Context_Repository().set_sequence_number(sequence_num_
      ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    Request_Context_Repository().set_transaction_depth(transaction_depth-1 ACE_ENV_ARG_PARAMETER);
    ACE_CHECK;

    if (transaction_depth > 1) {
      successor->set_update(state
        ACE_ENV_ARG_PARAMETER);
    }
    else {
      ACE_TRY {
        successor->oneway_set_update(state
          ACE_ENV_ARG_PARAMETER);
        ACE_TRY_CHECK;
      }
      ACE_CATCHANY {
      }
      ACE_ENDTRY;
    }
  }
  else if (transaction_depth > 1)
    ACE_THROW(FTRT::TransactionDepthTooHigh());
}

void 
Basic_Replication_Strategy::add_member(const FTRT::ManagerInfo & info,
                                       CORBA::ULong object_group_ref_version
                                       ACE_ENV_ARG_DECL_NOT_USED)
{
  FtRtecEventChannelAdmin::EventChannel_var successor = GroupInfoPublisher::instance()->successor();
  successor->add_member(info, object_group_ref_version ACE_ENV_ARG_PARAMETER);
}

int  Basic_Replication_Strategy::acquire_read (void)
{
  return mutex_.acquire_read();
}

int  Basic_Replication_Strategy::acquire_write (void)
{
  return mutex_.acquire_write();
}

int  Basic_Replication_Strategy::release (void)
{
  return mutex_.release();
}

