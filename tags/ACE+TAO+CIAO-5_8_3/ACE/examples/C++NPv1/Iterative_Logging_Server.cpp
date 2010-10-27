/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#include "ace/Log_Msg.h"
#include "Iterative_Logging_Server.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Iterative_Logging_Server server;

  if (server.run (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.run()"), 1);
  return 0;
}
