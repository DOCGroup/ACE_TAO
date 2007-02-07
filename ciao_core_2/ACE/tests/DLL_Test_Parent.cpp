// $Id$

#include "DLL_Test_Parent.h"
#include "ace/Log_Msg.h"

ACE_RCSID (tests,
           DLL_Test_Parent,
           "$Id$")


Parent::~Parent (void)
{
}

void
Parent::test (void)
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("parent called\n")));
}
