// $Id$
#include "Activity.h"

#include "ace/High_Res_Timer.h"
#include "ace/Synch.h"
#include "ace/Timer_Heap.h"
#include "ace/Service_Config.h"
#include "ace/Arg_Shifter.h"
#include "ace/Get_Opt.h"
#include "ace/Argv_Type_Converter.h"
#include "tao/ORB_Core.h"

#include "Thread_Task.h"
#include "Job_i.h"
#include "POA_Holder.h"
#include "Builder.h"

extern "C" void handler (int)
{
  ACE_Service_Config::reconfig_occurred (1);
}

int
main (int argc, char *argv[])
{
  ACE_Service_Config::static_svcs ()->insert (&ace_svc_desc_Builder);

  ACE_Sig_Action sa ((ACE_SignalHandler) handler, SIGHUP);

  ACE_Timer_Heap timer_queue;
  ACE_Reactor::instance ()->set_timer_queue (&timer_queue);

  ACE_TRY_NEW_ENV
    {
      ACTIVITY::instance()->run (argc, argv ACE_ENV_ARG_PARAMETER);
      ACE_TRY_CHECK;
    }
  ACE_CATCHANY
    {
      ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION,
                           "Caught exception:");
      return 1;
    }
  ACE_ENDTRY;

  return 0;
}
