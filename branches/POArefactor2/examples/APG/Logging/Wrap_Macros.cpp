// $Id$

#if defined (__GNUC__) && (__GNUC__ >= 3 || __GNUC_MINOR__ > 95)
// The macros in Trace.h only work on g++ 2.96 and later.

// Listing 1 code/ch03
#include "Trace.h"

void foo (void);

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  ACE_TRACE ("main");
  MY_DEBUG (ACE_TEXT ("Hi Mom\n"));
  foo ();
  MY_DEBUG (ACE_TEXT ("Goodnight\n"));
  return 0;
}

void foo (void)
{
  ACE_TRACE ("foo");
  MY_DEBUG (ACE_TEXT ("Howdy Pardner\n"));
}
// Listing 1

#else
#include <stdio.h>

int main (int, char *[])
{
  puts ("This example only works on g++ 2.96 and later.\n");
  return 0;
}

#endif /* __GNUC__ */
