// $Id$

// ============================================================================
//
// = LIBRARY
//    examples/Web_Crawler
//
// = FILENAME
//    main.cpp
//
// = DESCRIPTION
//     This program implements a Web crawler that can be configured to
//     apply various strategies to URLs that it visits.
//
// = AUTHOR
//    Doug Schmidt <schmidt@cs.wustl.edu>
//
// ============================================================================

#include "Web_Crawler.h"
#include "Options.h"
#include "ace/Signal.h"

ACE_RCSID(Web_Crawler, main, "$Id$")

void sig_handler (int)
{
  ACE_DEBUG ((LM_DEBUG,
              "aborting!\n"));
  ACE_OS::abort ();
}

int 
main (int argc, char *argv[])
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



