// $Id$

#include <ace/High_Res_Timer.h>
//#include <tao/RTCORBA/RTCORBA.h>
#include "FTClient_Timer_Handler.h"
#include "ciao/CIAO_common.h"
#include "FTClient_exec.h"

namespace CIDL_FTClient_Impl
{

  FTClient_Timer_Handler::FTClient_Timer_Handler (
    CIDL_FTClient_Impl::FTClient_exec_i * client_executor,
    bool logging)
    : client_executor_ (client_executor),
      logging_ (logging),
      count_ (0)
  {
  }

  void
  FTClient_Timer_Handler::set_orb (CORBA::ORB_ptr orb)
  {
    orb_ = CORBA::ORB::_duplicate (orb);
  }

  void
  FTClient_Timer_Handler::set_server (DeCoRAM::Worker_ptr server)
  {
    server_ = DeCoRAM::Worker::_duplicate (server);
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

        CIAO_DEBUG ((LM_TRACE, "s(%d) ",
                     count_));

        if (CORBA::is_nil (server_.in ()))
          {
            // try to get a recent object reference first
            server_ = DeCoRAM::Worker::_duplicate (client_executor_->server ());

            if (CORBA::is_nil (server_.in ()))
              {
                return 0;
              }
          }

        timer_.start ();

        // we have to do some profiling first to see how we can achieve
        // the correct execution time.
        server_->run_task (client_executor_->execution_time ());

        timer_.stop ();
       
        ACE_Time_Value rt;
        timer_.elapsed_time (rt);

        if (logging_)
          history_.push_back (rt.msec ());
      }
    catch (CORBA::Exception & ex)
      {
        CIAO_DEBUG ((LM_WARNING, 
                     "FTClient_Timer_Handler::handle_timeout () - "
                     "caught: %s", ex._info ().c_str ()));
      }

    try
      {
        // this code makes sure the client component stops execution
        if ((client_executor_->iterations () > 0) && 
            (++count_ >= client_executor_->iterations ()))
          {
            server_->stop ();
            
            client_executor_->ccm_passivate ();
            
            orb_->shutdown ();
          }
      }
    catch (CORBA::Exception & ex)
      {
        CIAO_DEBUG ((LM_WARNING, 
                     "FTClient_Timer_Handler::handle_timeout () after run_task - "
                     "caught: %s", ex._info ().c_str ()));
      }


    return 0;
  }
  
  void
  FTClient_Timer_Handler::dump (void)
  {
    if (logging_)
      {
        std::string logfile = client_executor_->name ();
        logfile += "-client.txt";
        std::ofstream out (logfile.c_str ());

        for (TimingList::iterator it = history_.begin ();
             it != history_.end ();
             ++it)
          {
            out << *it << std::endl;
          }

        out.close ();
      }
  }

} // end namespace CIDL_FTClient_Impl
