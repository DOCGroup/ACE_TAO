
//=============================================================================
/**
 *  @file    main.cpp
 *
 *   This program implements a Web crawler that can be configured to
 *   apply various strategies to URLs that it visits.
 *
 *  @author Doug Schmidt <d.schmidt@vanderbilt.edu>
 */
//=============================================================================


#include "ace/OS_main.h"
#include "ace/Signal.h"
#include "Web_Crawler.h"
#include "Options.h"


void sig_handler (int)
{
  ACE_DEBUG ((LM_DEBUG,
              ACE_TEXT ("aborting!\n")));
  ACE_OS::abort ();
}

int
ACE_TMAIN (int argc, ACE_TCHAR *argv[])
{
  ACE_Sig_Action sa ((ACE_SignalHandler) sig_handler, SIGFPE);
  Web_Crawler crawler;

  if (crawler.open (argc, argv) == -1)
    return 1;
  else if (crawler.run () == -1)
    return 1;
  else
    return 0;
}


