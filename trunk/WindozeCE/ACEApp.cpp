// $Id$

#include "stdafx.h"
#include "ace/OS.h"
#include "ACEApp.h"

#if 0
int
main (int argc, ACE_TCHAR *argv[])
{
  Sleep (500);
  for (int i = 0; i < argc; i++, Sleep (700))
    ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("The %d command arg is \"%s\""),
                i, argv [i]));
  return 0;
}
#endif
