//$Id$

ACE_INLINE
Simple_Server_i::Simple_Server_i (CORBA::ORB_ptr orb, 
        RTScheduling::Current_ptr current,
        Complex_Server_ptr server_ptr,
        Task_Stats& task_stats,
        int enable_yield)
  :  orb_ (CORBA::ORB::_duplicate (orb)),
     current_ (RTScheduling::Current::_duplicate (current)),
     server_ (Complex_Server::_duplicate (server_ptr)),
     task_stats_ (task_stats),
     enable_yield_ (enable_yield)   
{
}
