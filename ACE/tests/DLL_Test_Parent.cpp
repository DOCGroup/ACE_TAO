#include "DLL_Test_Parent.h"
#include "ace/Log_Msg.h"

Parent::~Parent ()
{
}

void
Parent::test ()
{
  ACE_DEBUG ((LM_DEBUG, ACE_TEXT ("parent called\n")));
}
