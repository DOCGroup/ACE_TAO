//$Id$

ACE_INLINE
DD_Servant_i::DD_Servant_i(CORBA::ORB_ptr orb, 
		RTScheduling::Current_ptr current,
		DD_Servant_ptr servant_ptr,
		Task_Stats& task_stats,
		int enable_yield,
		int exec_time)
      	:  orb_ (CORBA::ORB::_duplicate (orb)),
	current_ (RTScheduling::Current::_duplicate (current)),
	servant_ptr_ (DD_Servant::_duplicate (servant_ptr)),
	task_stats_ (task_stats),
	enable_yield_ (enable_yield),
	exec_time_ (exec_time)
{
}
