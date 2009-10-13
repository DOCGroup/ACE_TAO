// $Id$

#include "ace/Log_Msg.h"
#include "ace/Service_Config.h"

/*
  Put the following in your svc.conf:

  dynamic Logger Service_Object * ACE:_make_ACE_Logging_Strategy() "-s log.out -f STDERR|OSTREAM -p DEBUG|NOTICE"

  There seems to be a bug in ACE_Logging_Strategy in that the priority
  flags are not propagated to spawned threads.
*/

// Listing 1 code/ch03
int ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  if (ACE_Service_Config::open (argc,
                                argv,
                                ACE_DEFAULT_LOGGER_KEY,
                                1,
                                0,
                                1) < 0)
    ACE_ERROR_RETURN ((LM_ERROR, ACE_TEXT ("%p\n"),
                       ACE_TEXT ("Service Config open")),
                      1);
    ACE_TRACE ("main");
    ACE_DEBUG ((LM_NOTICE, ACE_TEXT ("%t%IHowdy Pardner\n")));
    ACE_DEBUG ((LM_INFO, ACE_TEXT ("%t%IGoodnight\n")));

    return 0;
}
// Listing 1
