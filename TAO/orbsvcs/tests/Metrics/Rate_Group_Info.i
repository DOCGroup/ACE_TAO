// $Id$
ACE_INLINE void
TAO_Rate_Group_Info::set_rate(RtecScheduler::Period_t rate_in_usec)
{
   rate_in_usec_ = rate_in_usec;
}

ACE_INLINE RtecScheduler::Period_t
TAO_Rate_Group_Info::get_rate() const
{
   return rate_in_usec_;
}

ACE_INLINE void 
TAO_Rate_Group_Info::set_rate_group_priority(RtecScheduler::OS_Priority rate_group_priority)
{
   rate_group_priority_ = rate_group_priority;
}

ACE_INLINE RtecScheduler::OS_Priority
TAO_Rate_Group_Info::get_rate_group_priority() const
{
   return rate_group_priority_;
}

ACE_INLINE void 
TAO_Rate_Group_Info::set_num_rate_groups(int num_rate_groups)
{
   num_rate_groups_ = num_rate_groups;
}

ACE_INLINE int 
TAO_Rate_Group_Info::get_num_rate_groups(void)
{
   return num_rate_groups_;
}

ACE_INLINE void 
TAO_Rate_Group_Info::set_corba_base_priority(int base_corba_priority)
{
   base_corba_priority_ = base_corba_priority;
}

ACE_INLINE int 
TAO_Rate_Group_Info::get_corba_base_priority(void)
{
   return base_corba_priority_;
}

ACE_INLINE void 
TAO_Rate_Group_Info::set_priority_spacing(int priority_spacing)
{
   priority_spacing_ = priority_spacing;
}

ACE_INLINE int 
TAO_Rate_Group_Info::get_priority_spacing()
{
   return priority_spacing_;
}

ACE_INLINE void 
TAO_Rate_Group_Info::set_priorities_contiguous(int priorities_contiguous)
{
   priorities_contiguous_ = priorities_contiguous;
}

ACE_INLINE int 
TAO_Rate_Group_Info::get_priorities_contiguous()
{
   return priorities_contiguous_;
}

