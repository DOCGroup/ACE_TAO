#include "test_config.h"
#include "ace/Service_Config.h"
#include "ace/OS_NS_fcntl.h"
#include "ace/Logging_Strategy.h"

//ACE_STATIC_SVC_REQUIRE (ACE_Logging_Strategy);

int
run_main (int , ACE_TCHAR ** )
{
  ACE_START_TEST (ACE_TEXT ("Missing_Svc_Conf_Test"));

  ACE_HANDLE h = ACE_OS::open (ACE_DEFAULT_SVC_CONF,O_RDONLY);
  if (h != ACE_INVALID_HANDLE)
    {
      ACE_ERROR ((LM_ERROR,ACE_TEXT("svc.conf exists, test unable to run\n")));
      ACE_OS::close(h);
      return -1;
    }

  int argc = 1;
  ACE_TCHAR *argv[] = {const_cast<ACE_TCHAR *>(ACE_TEXT("nosvc")),0,0,0 };
  int failcount = 0;
  int result = ACE_Service_Config::open(argc, argv);
  if (result != -1 || errno != ENOENT)
    {
      ++failcount;
      ACE_DEBUG ((LM_DEBUG, 
                  ACE_TEXT("ERROR: did not get expected ENOENT, %p\n"),
                  ACE_TEXT("ACE_Service_Config::open")));
    }
  else
    ACE_DEBUG ((LM_DEBUG, 
                ACE_TEXT("Success: defaulted Service_Config::open ")
                ACE_TEXT("with missing file got expected ENOENT\n")));
  
  result = ACE_Service_Config::close();
  if (result != 0)
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("%p\n"),
                ACE_TEXT("Service_Config::close")));

  argc = 3;
  argv[1] = const_cast<ACE_TCHAR *>(ACE_TEXT("-S"));
  argv[2] = const_cast<ACE_TCHAR *>(ACE_TEXT("dynamic Logger Service_Object *ACE:_make_ACE_Logging_Strategy() \"\""));

  result = ACE_Service_Config::open(argc, argv);
  if (result != 0)
    {
      ++failcount;
      ACE_DEBUG ((LM_DEBUG, 
                  ACE_TEXT("ERROR: missing svc.conf with ")
                  ACE_TEXT("command line directive, %p\n"),
                  ACE_TEXT("ACE_Service_Config::open")));
    }
  else
    ACE_DEBUG ((LM_DEBUG, 
                ACE_TEXT("Success: Service_Config::open with command line ")
                ACE_TEXT("directive ignored missing svc.conf\n")));

  ACE_END_TEST;
  return failcount;
}
