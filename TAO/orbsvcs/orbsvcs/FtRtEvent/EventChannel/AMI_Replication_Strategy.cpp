#include "orbsvcs/FtRtEvent/EventChannel/AMI_Replication_Strategy.h"
#include "orbsvcs/FtRtEvent/EventChannel/AMI_Primary_Replication_Strategy.h"

#include <memory>

TAO_BEGIN_VERSIONED_NAMESPACE_DECL

AMI_Replication_Strategy::AMI_Replication_Strategy(bool mt)
  : mt_(mt)
{
}

AMI_Replication_Strategy::~AMI_Replication_Strategy()
{
}

void
AMI_Replication_Strategy::replicate_request(
  const FTRT::State& state,
  RollbackOperation rollback,
  const FtRtecEventChannelAdmin::ObjectId& oid)
{
  ACE_UNUSED_ARG(state);
  ACE_UNUSED_ARG(rollback);
  ACE_UNUSED_ARG(oid);
}

void
AMI_Replication_Strategy::add_member(const FTRT::ManagerInfo & info,
                                     CORBA::ULong object_group_ref_version)
{
  ACE_UNUSED_ARG(info);
  ACE_UNUSED_ARG(object_group_ref_version);
}

Replication_Strategy*
AMI_Replication_Strategy::make_primary_strategy()
{
  AMI_Primary_Replication_Strategy* result;
  ACE_NEW_RETURN(result, AMI_Primary_Replication_Strategy(mt_), 0);
  std::unique_ptr<AMI_Primary_Replication_Strategy> holder(result);
  if (result->activate() == 0)
    return holder.release();
  return 0;
}

int  AMI_Replication_Strategy::acquire_read ()
{
  return 0;
}

int  AMI_Replication_Strategy::acquire_write ()
{
  return 0;
}

int  AMI_Replication_Strategy::release ()
{
  return 0;
}

TAO_END_VERSIONED_NAMESPACE_DECL
