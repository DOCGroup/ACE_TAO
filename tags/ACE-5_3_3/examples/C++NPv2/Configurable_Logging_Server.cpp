/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS.h"
#include "ace/Service_Config.h"
#include "ace/Reactor.h"

int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {

  ACE_STATIC_SVC_REGISTER (Reporter_Descriptor);

  ACE_Service_Config::open
    (argc, argv, ACE_DEFAULT_LOGGER_KEY, 0);

  ACE_Reactor::instance ()->run_reactor_event_loop ();
  return 0;
}
