// $Id$

#include "ace/ARGV.h"
#include "ace/OS.h"
#include "ace/Reactor.h"
#include "ace/Signal.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

static volatile bool bShutdown = false;
static void shutdown(int)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("Shutdown requested\n")));
  bShutdown = true;
}

int
ACE_TMAIN(int argc, ACE_TCHAR ** argv)
{
  int result = 0;
#if defined (ACE_HAS_FORK)
  ACE_Sig_Action sigUSR2((ACE_SignalHandler) shutdown, SIGUSR2);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("SIGUSR2 shutdown handler installed\n")));
  ACE_UNUSED_ARG(sigUSR2);

  pid_t pid = -1;
  pid = ACE_OS::fork();
  if (pid == 0) // child
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("child waiting\n")));
    ACE_OS::sleep(5);
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("signaling parent\n")));
    result = ACE_OS::kill(ACE_OS::getppid(), SIGUSR2);
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("signaled parent\n")));
    return 0;
  }
  else if (pid > 0) // parent
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("parent using ACE_Service_Config\n")));
    ACE_Service_Config serviceConfig;

    char signum[64];
    ACE_OS::sprintf(signum, "%d", SIGUSR1);

    ACE_ARGV args;
    args.add(argv[0]);
    args.add("-s");
    args.add(signum);

    result = serviceConfig.open (
      args.argc(),
      args.argv(),
      ACE_DEFAULT_LOGGER_KEY,
      1, // ignore_static_svcs = 1,
      1, // ignore_default_svc_conf_file = 0,
      0  // ignore_debug_flag = 0
    );
    if(0 != result)
    {
      ACE_ERRROR ((LM_ERROR, ACE_TEXT ("Error: serviceConfig.open failed\n")));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.open done\n")));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.process_file ...\n")));
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    result = serviceConfig.process_file("Bug_3251.conf");
#else
    result = serviceConfig.process_file("Bug_3251.conf.xml");
#endif
    if(0 != result)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: serviceConfig.process_file failed\n")));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.process_file done\n")));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("run_event_loop ...\n")));
    while(!bShutdown)
    {
      ACE_OS::last_error(0);
      result = ACE_Reactor::run_event_loop();
      // reenter loop on EINTR
      if(0 != result && EINTR == ACE_OS::last_error())
      {
        if(bShutdown)
          break;
      }
      else if(0 != result)
      {
        ACE_DEBUG ((
          LM_INFO,
          ACE_TEXT ("run_event_loop failed (%s, %d)\n"),
          ACE_OS::strerror(ACE_OS::last_error()),
          ACE_OS::last_error()
        ));
      }
    }

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("run_event_loop done\n")));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.fini_svcs ...\n")));
    result = serviceConfig.fini_svcs();
    if(0 != result)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: serviceConfig.fini_svcs failed\n")));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.fini_svcs done\n")));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.close ...\n")));
    result = serviceConfig.close();
    if(0 != result)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("Error: serviceConfig.close failed\n")));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("serviceConfig.close done\n")));

    return result;
  } /* end of if */
  else // fork failed
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("fork failed\n")));
    return 1;
  } /* end of else */
#else
  ACE_UNUSED_ARG (argc);
  ACE_UNUSED_ARG (argv);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Fork not available\n")));
#endif
  return result;
}
