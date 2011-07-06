// $Id$

#include <ace/High_Res_Timer.h>
//#include <tao/RTCORBA/RTCORBA.h>
#include <ciao/CIAO_common.h>
#include "FTClient_Task.h"
#include "WorkerC.h"
#include "FTClient_exec.h"

namespace CIDL_FTClient_Impl
{

  FTClient_Task::FTClient_Task (
    CIDL_FTClient_Impl::FTClient_exec_i * client_executor)
    : client_executor_ (client_executor),
      stop_ (false)
  {
  }

  void
  FTClient_Task::set_orb (CORBA::ORB_ptr orb)
  {
    orb_ = CORBA::ORB::_duplicate (orb);
  }

  void
  FTClient_Task::stop ()
  {
    stop_ = true;
  }

  int
  FTClient_Task::svc (void)
  {
    DeCoRAM::Worker_var server = client_executor_->server ();

    // CORBA::Object_var obj = orb_->resolve_initial_references ("RTCurrent");
    // RTCORBA::Current_var rt_current = RTCORBA::Current::_narrow (obj);
    // rt_current->the_priority (client_executor_->priority ());

    while (!stop_)
      {
        try
          {
            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("FT_Client_Task::svc : before run_task.\n")));
            
            ACE_hrtime_t start, end;
            start = ACE_OS::gethrtime ();
            
            // we have to do some profiling first to see how we can achieve
            // the correct execution time.
            server->run_task (client_executor_->execution_time ());
        
            end = ACE_OS::gethrtime ();

            CIAO_DEBUG ((LM_DEBUG, ACE_TEXT ("FT_Client_Task::svc : after run_task.\n")));
            
            /*        
                      CIAO_DEBUG ((LM_DEBUG,
                      ACE_TEXT ("response time for %f was %d.\n"),
                      client_executor_->execution_time (),
                      end - start));
            */
          }
        catch (CORBA::SystemException & ex)
          {
            CIAO_DEBUG ((LM_WARNING, 
                         ACE_TEXT ("FTClient_Task::svc () - "
                                   "caught: %s"), ex._info ().c_str ()));
          }

        sleep (2);
      }

    return 0;
  }

} // end namespace CIDL_FTClient_Impl
