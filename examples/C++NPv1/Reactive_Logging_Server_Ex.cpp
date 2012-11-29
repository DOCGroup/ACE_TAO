/*
** $Id$
**
** Copyright 2001 Addison Wesley. All Rights Reserved.
*/

#include "ace/Functor.h"
#include "ace/Log_Msg.h"

#include "Reactive_Logging_Server_Ex.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  Reactive_Logging_Server_Ex server;

  if (server.run (argc, argv) == -1)
    ACE_ERROR_RETURN ((LM_ERROR, "%p\n", "server.run()"), 1);
  return 0;
}

