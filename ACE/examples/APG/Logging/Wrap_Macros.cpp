#include "Trace.h"

// Listing 1 code/ch03
void foo ();

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE ("main");
  MY_DEBUG (ACE_TEXT ("Hi Mom\n"));
  foo ();
  MY_DEBUG (ACE_TEXT ("Goodnight\n"));
  return 0;
}

void foo ()
{
  ACE_TRACE ("foo");
  MY_DEBUG (ACE_TEXT ("Howdy Pardner\n"));
}
// Listing 1

