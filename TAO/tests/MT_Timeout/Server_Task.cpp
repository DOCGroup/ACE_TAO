
#include "Server_Task.h"

Server_Task::Server_Task (ACE_Thread_Manager *thr_mgr,
                          CORBA::ORB_ptr orb,
                          ACE_Time_Value const& running_time)
  : ACE_Task_Base (thr_mgr)
  , orb_ (CORBA::ORB::_duplicate (orb))
  , running_time_ (running_time)
{
}

int
Server_Task::svc ()
{
  try
    {
      this->orb_->run (this->running_time_);
    }
  catch (CORBA::Exception const&)
    {
      return -1;
    }
  return 0;
}
