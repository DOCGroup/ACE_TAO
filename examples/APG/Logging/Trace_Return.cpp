// $Id$

#if defined (__GNUC__)
// The DEBUG stuff only works with g++...

// Listing 1 code/ch03
#include "Trace.h"

void foo (void);

int ACE_TMAIN (int, ACE_TCHAR *[])
{
  TRACE (ACE_TEXT ("main"));
    
  MY_DEBUG (ACE_TEXT ("Hi Mom\n"));
  foo ();
  MY_DEBUG (ACE_TEXT ("Goodnight\n"));

  TRACE_RETURN (0);
}

void foo (void)
{
  TRACE (ACE_TEXT ("foo"));
  MY_DEBUG (ACE_TEXT ("Howdy Pardner\n"));
  TRACE_RETURN_VOID ();
}
// Listing 1

#else
#include <stdio.h>

int main (int, char *[])
{
  puts ("This example only works on g++\n");
  return 0;
}

#endif /* __GNUC__ */
