// $Id$

#include "Callee_i.h"

void Callee_i::callback (const char * message)
{
  ACE_DEBUG ((LM_DEBUG, "Callee_i::callback: message is %s", message));
}
