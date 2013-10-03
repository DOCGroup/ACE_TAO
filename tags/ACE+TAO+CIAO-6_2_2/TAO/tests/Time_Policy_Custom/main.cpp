//=============================================================================
/**
 *  @file    main.cpp
 *
 *  $Id$
 *
 *  Implementation of the server.
 *
 *
 *  @author Alexander Babu Arulanthu <alex@cs.wustl.edu>
 *  @author Michael Kircher <Michael.Kircher@mchp.siemens.de>
 */
//=============================================================================

#include "tao/ORB.h"
#include "tao/ORB_Core.h"
#include "tao/PortableServer/PortableServer.h"
#include "ace/Reactor.h"
#include "ace/Timer_Queue.h"
#include "ace/Time_Value.h"
#include "ace/OS_NS_sys_time.h"
#include "ace/High_Res_Timer.h"
#include "ace/Service_Config.h"
#include "ace/ARGV.h"

#if (TAO_HAS_TIME_POLICY == 1)

#include "Custom_Time_Policy_Strategy.h"

class TestHandler
  : public ACE_Event_Handler
{
public:
  TestHandler (CORBA::ORB_ptr orb)
    : orb_ (CORBA::ORB::_duplicate (orb)),
      timeout_triggered_ (false)
  {}

  virtual int handle_timeout (const ACE_Time_Value &tv,
                              const void *arg);

  bool trigger_in(const ACE_Time_Value &delay);

  bool timeout_triggered () { return this->timeout_triggered_; }

private:
  CORBA::ORB_var orb_;
  bool timeout_triggered_;
};

int TestHandler::handle_timeout (const ACE_Time_Value &,
                                 const void *)
{
  ACE_DEBUG ((LM_DEBUG, "TestHandler::handle_timeout - timeout triggered\n"));
  this->timeout_triggered_ = true;
  this->orb_->shutdown (false);
  return 0;
}

bool TestHandler::trigger_in(const ACE_Time_Value &delay)
{
  return -1 != this->orb_->orb_core ()->reactor ()->schedule_timer (this, 0, delay, ACE_Time_Value (0));
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      ACE_ARGV my_argv;
      if (argc>1 && argv[1][0]=='-' && argv[1][1]=='s')
        {
          ACE_DEBUG ((LM_DEBUG, "loading static service\n"));

          ACE_Service_Config::process_directive (ace_svc_desc_Custom_Time_Policy_Strategy);
          my_argv.add(argv[0]);
          for (int i=2; i<argc ;++i)
            my_argv.add (argv[i]);

          my_argv.add (ACE_TEXT ("-ORBSvcConfDirective"));
          my_argv.add (ACE_TEXT ("static Time_Policy_Manager \"-ORBTimePolicyStrategy CUSTOM_TIME_POLICY\""), true);
        }
      else
        {
          for (int i=0; i<argc ;++i)
            my_argv.add (argv[i]);
        }

      int my_argc = my_argv.argc ();
      CORBA::ORB_var orb =
        CORBA::ORB_init (my_argc, my_argv.argv ());

      // check if custom policy installed in ORB
      ACE_Time_Value tv_hr = ACE_High_Res_Timer::gettimeofday_hr ();
      ACE_Time_Value tv_orb = orb->orb_core ()->reactor ()->timer_queue ()->gettimeofday ();
      ACE_Time_Value tv_diff = tv_orb - tv_hr;
      // The custom policy gives an offset of +10s
      if (tv_diff.sec () != 10)
      {
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Incorrect time offset (%us). Time policy doesn't seem to be loaded\n", tv_diff.sec ()),
                          1);
      }

      CORBA::Object_var poa_object =
        orb->resolve_initial_references("RootPOA");

      if (CORBA::is_nil (poa_object.in ()))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to initialize the POA.\n"),
                          1);

      PortableServer::POA_var root_poa =
        PortableServer::POA::_narrow (poa_object.in ());

      PortableServer::POAManager_var poa_manager =
        root_poa->the_POAManager ();

      poa_manager->activate ();

      TestHandler test_handler (orb.in ());

      // trigger in 2 seconds
      if (!test_handler.trigger_in (ACE_Time_Value (2, 0)))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to schedule trigger.\n"),
                          1);

      // run for max. 4 seconds
      ACE_Time_Value timeout (4, 0);
      orb->run (timeout);

      root_poa->destroy (1,  // ethernalize objects
                         0  // wait for completion
                        );

      orb->destroy ();

      ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

      if (!test_handler.timeout_triggered ())
        {
          ACE_DEBUG ((LM_DEBUG, "timer handler did not trigger\n"));
          return 1;
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }

  return 0;
}

#else
int
ACE_TMAIN(int , ACE_TCHAR * [])
{
  ACE_DEBUG ((LM_INFO, "TAO built without Time Policy support\n"));
  return 0;
}
#endif /* TAO_HAS_TIME_POLICY != 1 */
