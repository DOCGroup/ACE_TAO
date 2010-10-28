// $Id$

#include "Trace.h"

// Listing 1 code/ch03
void foo (void);

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  TRACE ("main");

  MY_DEBUG (ACE_TEXT ("Hi Mom\n"));
  foo ();
  MY_DEBUG (ACE_TEXT ("Goodnight\n"));

  TRACE_RETURN (0);
}

void foo (void)
{
  TRACE ("foo");
  MY_DEBUG (ACE_TEXT ("Howdy Pardner\n"));
  TRACE_RETURN_VOID ();
}
// Listing 1

