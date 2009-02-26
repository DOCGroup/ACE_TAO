// $Id$

#include <ace/High_Res_Timer.h>
//#include <tao/RTCORBA/RTCORBA.h>
#include "FTClient_Timer_Handler.h"
#include "ciao/CIAO_common.h"
#include "WorkerC.h"
#include "FTClient_exec.h"

namespace CIDL_FTClient_Impl
{

  FTClient_Timer_Handler::FTClient_Timer_Handler (
    CIDL_FTClient_Impl::FTClient_exec_i * client_executor)
    : client_executor_ (client_executor)
  {
  }

  void
  FTClient_Timer_Handler::set_orb (CORBA::ORB_ptr orb)
  {
    orb_ = CORBA::ORB::_duplicate (orb);
  }

  int
  FTClient_Timer_Handler::handle_timeout (const ACE_Time_Value &,
                                          const void *)
  {
    try
      {
        /*
          CORBA::Object_var obj = orb_->resolve_initial_references ("RTCurrent");
          RTCORBA::Current_var rt_current = RTCORBA::Current::_narrow (obj);
          
          rt_current->the_priority (client_executor_->priority ());
        */

        DeCoRAM::Worker_var server = client_executor_->server ();

        if (CORBA::is_nil (server.in ()))
            return 0;

        ACE_hrtime_t start, end;
        start = ACE_OS::gethrtime ();

        // we have to do some profiling first to see how we can achieve
        // the correct execution time.
        server->run_task (client_executor_->execution_time ());
        
        end = ACE_OS::gethrtime ();
        
        CIAO_DEBUG ((LM_DEBUG,
                     "response time for %f was %d.\n",
                     client_executor_->execution_time (),
                     end - start));
      }
    catch (CORBA::Exception & ex)
      {
        CIAO_DEBUG ((LM_WARNING, 
                     "FTClient_Timer_Handler::handle_timeout () - "
                     "caught: %s", ex._info ().c_str ()));
      }

    return 0;
  }
    
} // end namespace CIDL_FTClient_Impl
