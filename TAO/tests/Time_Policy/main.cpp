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
#include "ace/Get_Opt.h"

#if (TAO_HAS_TIME_POLICY == 1)

#if defined(TAO_USE_HR_TIME_POLICY_STRATEGY)
bool uses_hr_time = true;
#else
bool uses_hr_time = false;
#endif

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
parse_args (int argc, ACE_TCHAR *argv[])
{
  ACE_Get_Opt get_opts (argc, argv, ACE_TEXT("h"));
  int c;

  while ((c = get_opts ()) != -1)
    switch (c)
      {
      case 'h':
        uses_hr_time = true;
        break;
      case '?':
      default:
        ACE_ERROR_RETURN ((LM_ERROR,
                           "usage:  %s "
                           "-h "
                           "\n"
                           "\t-h\t: uses highres time policy\n",
                           argv [0]),
                          -1);
        break;
      }

  // Indicates successful parsing of the command line
  return 0;
}

int
ACE_TMAIN(int argc, ACE_TCHAR *argv[])
{
  try
    {
      CORBA::ORB_var orb =
        CORBA::ORB_init (argc, argv);

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

      if (parse_args (argc, argv) != 0)
        return 1;

      poa_manager->activate ();

      TestHandler test_handler (orb.in ());

      // trigger in 2 seconds
      if (!test_handler.trigger_in (ACE_Time_Value (2, 0)))
        ACE_ERROR_RETURN ((LM_ERROR,
                           "Unable to schedule trigger.\n"),
                          1);

      // reset system clock 4 seconds backwards
      timespec_t curts;
      ACE_Time_Value curtime = ACE_OS::gettimeofday ();
      curtime -= ACE_Time_Value (4, 0);
      curts = curtime;
      if (ACE_OS::clock_settime (CLOCK_REALTIME, &curts) != 0)
        {
          ACE_DEBUG((LM_INFO,
                     "Unable to reset OS time. Insufficient privileges or not supported.\n"));

          root_poa->destroy (1,  // ethernalize objects
                             0  // wait for completion
                            );

          orb->destroy ();

          return 0;
        }
      else
        {
          // run for max. 4 seconds
          ACE_Time_Value timeout (4, 0);
          orb->run (timeout);

          root_poa->destroy (1,  // ethernalize objects
                             0  // wait for completion
                            );

          orb->destroy ();

          ACE_DEBUG ((LM_DEBUG, "event loop finished\n"));

          // reset system clock to correct time
          curtime = ACE_OS::gettimeofday ();
          curtime += ACE_Time_Value (4, 0);
          curts = curtime;
          ACE_OS::clock_settime (CLOCK_REALTIME, &curts);

          if (!test_handler.timeout_triggered ())
            {
              ACE_DEBUG ((LM_DEBUG, "timer handler did not trigger\n"));
              return uses_hr_time ? 1 : 0;
            }
          else
            {
              return uses_hr_time ? 0 : 1;
            }
        }
    }
  catch (const CORBA::Exception& ex)
    {
      ex._tao_print_exception ("Caught exception:");
      return 1;
    }
}

#else
int
ACE_TMAIN(int , ACE_TCHAR * [])
{
  ACE_DEBUG ((LM_INFO, "TAO built without Time Policy support\n"));
  return 0;
}
#endif /* TAO_HAS_TIME_POLICY != 1 */
