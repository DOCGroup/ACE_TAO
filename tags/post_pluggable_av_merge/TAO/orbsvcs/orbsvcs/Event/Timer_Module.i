//
// $Id$
//

ACE_INLINE TAO_EC_RPT_Timer_Module::ReactorTask*
TAO_EC_RPT_Timer_Module::GetReactorTask(RtecScheduler::Preemption_Priority_t priority)
{
  if (reactorTasks[priority] == 0)
    {
      this->activate ();
      //ACE_ERROR_RETURN ((LM_ERROR,
      //"%p no reactor task for priority %d.\n", 
      //"TAO_EC_RPT_Timer_Module::GetReactor",
      //priority), 0);
   }
   
  return reactorTasks[priority];
}

ACE_INLINE ACE_RT_Thread_Manager*
TAO_EC_RPT_Timer_Module::ThrMgr()
{
  return &thr_mgr;
}

// ****************************************************************

ACE_INLINE
TAO_EC_Timeout_Handler::TAO_EC_Timeout_Handler (void)
{
}

