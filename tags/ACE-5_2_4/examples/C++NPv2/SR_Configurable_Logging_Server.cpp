/*
** $Id$
**
** Copyright 2002 Addison Wesley. All Rights Reserved.
*/

#include "ace/OS.h"
#include "ace/Reactor.h"
#include "ace/Select_Reactor.h"
#include "ace/Service_Config.h"


int ACE_TMAIN (int argc, ACE_TCHAR *argv[]) {
  ACE_Select_Reactor *select_reactor;
  ACE_NEW_RETURN (select_reactor, ACE_Select_Reactor, 1);
  ACE_Reactor *reactor;
  ACE_NEW_RETURN (reactor, ACE_Reactor (select_reactor, 1), 1);
  ACE_Reactor::close_singleton ();
  ACE_Reactor::instance (reactor, 1);

  ACE_Service_Config::open (argc, argv);

  ACE_Reactor::instance ()->run_reactor_event_loop ();
  return 0;
}
