// $Id$
#include "ServerApp.h"
#include "ace/Log_Msg.h"
#include "ace/CORBA_macros.h"
#include "tao/Environment.h"
#include "tao/Exception.h"


int
main(int argc, char* argv[])
{
  ACE_LOG_MSG->priority_mask(LM_TRACE     |
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

  ACE_TRY_NEW_ENV 
  { 
    int ret = app.run(argc,argv ACE_ENV_ARG_PARAMETER); 
    ACE_TRY_CHECK; 
    return ret == 1 ? 0 : ret; 
  } 
  ACE_CATCHANY 
  { 
     ACE_PRINT_EXCEPTION (ACE_ANY_EXCEPTION, 
                           "Caught exception:"); 
  } 
  ACE_CATCHALL 
  { 
    ACE_ERROR((LM_ERROR, 
               "(%P|%t) Unknown (...) exception caught in main() " 
               "for ServerApp\n")); 
  } 
  ACE_ENDTRY; 

  return 1;
}
