// $Id$

#include "ace/ARGV.h"
#include "ace/Reactor.h"
#include "ace/Signal.h"
#include "ace/Service_Config.h"
#include "ace/Log_Msg.h"

#if !defined (ACE_LACKS_FORK)
static volatile bool bShutdown = false;
static void shutdown_func(int)
{
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) Shutdown requested\n")));
  bShutdown = true;
}
#endif

int
ACE_TMAIN(int, ACE_TCHAR ** argv)
{
  int result = 0;
#if !defined (ACE_LACKS_FORK)
  ACE_Sig_Action sigUSR2((ACE_SignalHandler) shutdown_func, SIGUSR2);
  ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) SIGUSR2 shutdown handler installed\n")));
  ACE_UNUSED_ARG(sigUSR2);

  pid_t pid = -1;
  pid = ACE_OS::fork();
  ACE_Log_Msg::instance ()->sync (argv[0]); // Make %P|%t work right

  if (pid == 0) // child
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) child waiting\n")));
    ACE_OS::sleep(5);
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) signaling parent\n")));
    result = ACE_OS::kill(ACE_OS::getppid(), SIGUSR2);
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) signaled parent\n")));
    //    ACE_OS::sleep (100000);
    return 0;
  }
  else if (pid > 0) // parent
  {
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) parent using ACE_Service_Config, pid=%d\n"), pid));
    ACE_Service_Config serviceConfig;

    ACE_TCHAR signum[64];
    ACE_OS::sprintf(signum, ACE_TEXT("%d"), SIGUSR1);

    ACE_ARGV args;
    args.add(argv[0]);
    args.add(ACE_TEXT("-s"));
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
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Error: serviceConfig.open failed\n")));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) serviceConfig.open done\n")));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) serviceConfig.process_file ...\n")));
#if (ACE_USES_CLASSIC_SVC_CONF == 1)
    result = serviceConfig.process_file(ACE_TEXT("Bug_3251.conf"));
#else
    result = serviceConfig.process_file(ACE_TEXT("Bug_3251.conf.xml"));
#endif
    if(0 != result)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Error: serviceConfig.process_file failed\n")));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) serviceConfig.process_file done\n")));

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
          ACE_TEXT ("(%P|%t) run_event_loop failed (%s, %d)\n"),
          ACE_OS::strerror(ACE_OS::last_error()),
          ACE_OS::last_error()
        ));
      }
    }

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) run_event_loop done\n")));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) serviceConfig.fini_svcs ...\n")));
    result = serviceConfig.fini_svcs();
    if(0 != result)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Error: serviceConfig.fini_svcs failed\n")));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) serviceConfig.fini_svcs done\n")));

    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) serviceConfig.close ...\n")));
    result = serviceConfig.close();
    if(0 != result)
    {
      ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Error: serviceConfig.close failed\n")));
      return result;
    }
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("(%P|%t) serviceConfig.close done\n")));

    return result;
  } /* end of if */
  else // fork failed
  {
    ACE_ERROR ((LM_ERROR, ACE_TEXT ("(%P|%t) Error: fork failed\n")));
    return 1;
  } /* end of else */
#else
  ACE_UNUSED_ARG (argv);
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("Fork not available\n")));
#endif
  return result;
}
