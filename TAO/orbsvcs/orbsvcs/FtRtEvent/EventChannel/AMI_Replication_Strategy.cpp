// $Id$

#include "AMI_Replication_Strategy.h"
#include "AMI_Primary_Replication_Strategy.h"

ACE_RCSID (EventChannel,
           AMI_Replication_Strategy,
           "$Id$")

AMI_Replication_Strategy::AMI_Replication_Strategy()
{
}

void
AMI_Replication_Strategy::replicate_request(
  const FTRT::State& state,
  RollbackOperation rollback,
  const FtRtecEventChannelAdmin::ObjectId& oid
  ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_UNUSED_ARG(state);
  ACE_UNUSED_ARG(rollback);
  ACE_UNUSED_ARG(oid);
}

void 
AMI_Replication_Strategy::add_member(const FTRT::ManagerInfo & info,
                                     CORBA::ULong object_group_ref_version
                                     ACE_ENV_ARG_DECL_NOT_USED)
{
  ACE_UNUSED_ARG(info);
  ACE_UNUSED_ARG(object_group_ref_version);
}

Replication_Strategy*
AMI_Replication_Strategy::make_primary_strategy()
{
  AMI_Primary_Replication_Strategy* result;
  ACE_NEW_RETURN(result, AMI_Primary_Replication_Strategy, 0);
  auto_ptr<AMI_Primary_Replication_Strategy> holder(result);
  if (result->activate() == 0)
    return holder.release();
  return 0;
}

int  AMI_Replication_Strategy::acquire_read (void)
{
  return 0;
}

int  AMI_Replication_Strategy::acquire_write (void)
{
  return 0;
}

int  AMI_Replication_Strategy::release (void)
{
  return 0;
}

