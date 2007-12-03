// $Id$
#ifndef TESTAPPMAIN_H
#define TESTAPPMAIN_H

#include "ace/Log_Msg.h"
#include "ace/SString.h"
#include "tao/Exception.h"
#include "tao/Environment.h"

#define TEST_APP_MAIN(APP_TYPE) \
int \
ACE_TMAIN(int argc, ACE_TCHAR *argv[])\
{ \
  ACE_LOG_MSG->priority_mask(LM_TRACE     | \
                             LM_DEBUG     | \
                             LM_INFO      | \
                             LM_NOTICE    | \
                             LM_WARNING   | \
                             LM_ERROR     | \
                             LM_CRITICAL  | \
                             LM_ALERT     | \
                             LM_EMERGENCY, \
                             ACE_Log_Msg::PROCESS); \
\
  APP_TYPE app; \
\
  try\
  { \
    int ret = app.run(argc,argv); \
\
    return ret; \
  } \
  catch (const CORBA::Exception& ex)\
  { \
     ex._tao_print_exception ("Caught exception:"); \
  } \
  catch (...)\
  { \
    ACE_ERROR((LM_ERROR, \
               "(%P|%t) Unknown (...) exception caught in main() " \
               "for App [%d].\n", app.name())); \
  } \
  \
  return 1; \
}

#endif
