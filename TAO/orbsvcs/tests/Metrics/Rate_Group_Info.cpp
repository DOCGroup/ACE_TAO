// $Id$
#include "Rate_Group_Info.h"

#if !defined (__ACE_INLINE__)
#include "Rate_Group_Info.i"
#endif /* !defined INLINE */

#include "ace/Sched_Params.h"

int TAO_Rate_Group_Info::num_rate_groups_ = 0;
int TAO_Rate_Group_Info::base_corba_priority_ = 0;
int TAO_Rate_Group_Info::priority_spacing_ = 0;
int TAO_Rate_Group_Info::priorities_contiguous_ = 0;

TAO_Rate_Group_Info::TAO_Rate_Group_Info()
: rate_group_priority_(ACE_Sched_Params::priority_max(ACE_SCHED_FIFO))
, rate_in_usec_(25000)

{
}

TAO_Rate_Group_Info::TAO_Rate_Group_Info(RtecScheduler::Period_t rate_in_usec, RtecScheduler::OS_Priority rate_group_priority)
: rate_group_priority_(rate_group_priority)
, rate_in_usec_(rate_in_usec)

{
}

TAO_Rate_Group_Info::~TAO_Rate_Group_Info()
{
}

TAO_Rate_Group_Info::TAO_Rate_Group_Info(const TAO_Rate_Group_Info &right)
: rate_group_priority_(right.rate_group_priority_)
, rate_in_usec_(right.rate_in_usec_)
{
}

TAO_Rate_Group_Info & TAO_Rate_Group_Info::operator=(const TAO_Rate_Group_Info &rate_group_info)
{
   rate_group_priority_ = rate_group_info.rate_group_priority_;
   rate_in_usec_ = rate_group_info.rate_in_usec_;
   
   return *this;
}
