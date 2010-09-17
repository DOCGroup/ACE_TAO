// $Id$

#include "Log_Msg_Alt.h"

void foo (void);

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE ("main");

  // Listing 1 code/ch03
  ACE_DEBUG ((MY_DEBUG ACE_TEXT ("Hi Mom\n")));

  ACE_DEBUG ((MY_DEBUG ACE_TEXT ("Goodnight\n")));
  // Listing 1

  return 0;
}
