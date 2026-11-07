#include "ServerApp.h"
#include "ace/Log_Msg.h"
#include "ace/CORBA_macros.h"
#include "tao/Environment.h"
#include "tao/Exception.h"

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_LOG_MSG->priority_mask (LM_TRACE     |
                             LM_DEBUG     |
                             LM_INFO      |
                             LM_NOTICE    |
                             LM_WARNING   |
                             LM_ERROR     |
                             LM_CRITICAL  |
                             LM_ALERT     |
                             LM_EMERGENCY,
                             ACE_Log_Msg::PROCESS);

  ServerApp app;

  try
    {
      int const result = app.run (argc, argv);
      return result == 1 ? 0 : result;
    }
  catch (CORBA::Exception const &ex)
    {
      ex._tao_print_exception ("Caught exception:");
    }
  catch (...)
    {
      ACE_ERROR ((LM_ERROR,
                  ACE_TEXT ("(%P|%t) Unknown (...) exception caught in main() ")
                  ACE_TEXT ("for ServerApp\n")));
    }

  return 1;
}
